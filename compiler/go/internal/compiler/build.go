package compiler

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/ast"
)

type BuildOptions struct {
	Input           string
	Output          string
	EmitASTJSON     bool
	EmitLLVMIR      bool
	EmitSymbolsJSON bool
	Target          string
	Zig             string
	LLC             string
	LLD             string
	MuslSysroot     string
	Mecc            bool
	Verbose         bool
}

func ParseBuildArgs(args []string) (BuildOptions, error) {
	var o BuildOptions
	for i := 0; i < len(args); i++ {
		a := args[i]
		switch a {
		case "-o":
			if i+1 >= len(args) {
				return o, errors.New("-o requires an output path")
			}
			i++
			o.Output = args[i]
		case "--emit-ast-json":
			o.EmitASTJSON = true
		case "--emit-llvm-ir":
			o.EmitLLVMIR = true
		case "--emit-symbols-json":
			o.EmitSymbolsJSON = true
		case "--verbose":
			o.Verbose = true
		case "--mecc=true":
			o.Mecc = true
		case "--mecc=false":
			o.Mecc = false
		case "--target", "--zig", "--llc", "--lld", "--musl-sysroot", "--mecc":
			if i+1 >= len(args) {
				return o, fmt.Errorf("%s requires a value", a)
			}
			i++
			switch a {
			case "--target":
				o.Target = args[i]
			case "--zig":
				o.Zig = args[i]
			case "--llc":
				o.LLC = args[i]
			case "--lld":
				o.LLD = args[i]
			case "--musl-sysroot":
				o.MuslSysroot = args[i]
			case "--mecc":
				if args[i] == "true" {
					o.Mecc = true
				} else if args[i] == "false" {
					o.Mecc = false
				} else {
					return o, errors.New("--mecc requires true or false")
				}
			}
		default:
			if strings.HasPrefix(a, "-") {
				return o, fmt.Errorf("unknown build option %q", a)
			}
			if o.Input != "" {
				return o, errors.New("build accepts exactly one input path")
			}
			o.Input = a
		}
	}
	if o.Input == "" {
		cwd, _ := os.Getwd()
		current, _ := filepath.Abs(cwd)
		for {
			manifest_path := filepath.Join(current, "clyth_project.json")
			if data, read_err := os.ReadFile(manifest_path); read_err == nil {
				var manifest struct {
					Entry string `json:"entry"`
				}
				if json_err := json.Unmarshal(data, &manifest); json_err != nil {
					return o, json_err
				}
				if manifest.Entry == "" {
					return o, errors.New("clyth_project.json requires entry")
				}
				o.Input = filepath.Join(current, manifest.Entry)
				break
			}
			parent := filepath.Dir(current)
			if parent == current {
				return o, errors.New("usage: clyth build [file] [--emit-ast-json] [-o output]; no clyth_project.json found")
			}
			current = parent
		}
	}
	modes := 0
	if o.EmitASTJSON {
		modes++
	}
	if o.EmitLLVMIR {
		modes++
	}
	if o.EmitSymbolsJSON {
		modes++
	}
	if modes > 1 {
		return o, errors.New("select only one emit mode")
	}
	return o, nil
}

func RunBuild(args []string, out, errOut io.Writer) error {
	o, err := ParseBuildArgs(args)
	if err != nil {
		return err
	}
	info, err := os.Stat(o.Input)
	if err != nil {
		return err
	}
	if info.IsDir() {
		manifest_path := filepath.Join(o.Input, "clyth_project.json")
		data, read_err := os.ReadFile(manifest_path)
		if read_err != nil {
			return fmt.Errorf("directory build requires %s: %w", manifest_path, read_err)
		}
		var manifest struct {
			Entry string `json:"entry"`
		}
		if json_err := json.Unmarshal(data, &manifest); json_err != nil {
			return fmt.Errorf("reading %s: %w", manifest_path, json_err)
		}
		if manifest.Entry == "" {
			return fmt.Errorf("%s requires entry", manifest_path)
		}
		o.Input = filepath.Join(o.Input, manifest.Entry)
	}
	if filepath.Ext(o.Input) != ".clyth" {
		return fmt.Errorf("input %q is not a .clyth source file", o.Input)
	}
	program, frontErrs, err := ParseFile(o.Input)
	if err != nil {
		return err
	}
	if len(frontErrs) > 0 {
		for _, e := range frontErrs {
			fmt.Fprintln(errOut, e.Error())
		}
		return fmt.Errorf("frontend failed with %d error(s)", len(frontErrs))
	}
	if o.EmitASTJSON {
		data, err := ASTJSON(program)
		if err != nil {
			return err
		}
		data = append(data, '\n')
		if o.Output != "" {
			return os.WriteFile(o.Output, data, 0o644)
		}
		_, err = out.Write(data)
		return err
	}
	result, semantic_errs, err := analyze_file_with_options(o.Input, program, SemanticOptions{mecc_enabled: o.Mecc})
	if err != nil {
		return err
	}
	if len(semantic_errs) > 0 {
		for _, semantic_err := range semantic_errs {
			fmt.Fprintln(errOut, semantic_err.Error())
		}
		return fmt.Errorf("semantic analysis failed with %d error(s)", len(semantic_errs))
	}
	print_compiler_diagnostics(result, o.Verbose, errOut)
	if o.EmitSymbolsJSON {
		data, err := symbols_json(result)
		if err != nil {
			return err
		}
		data = append(data, '\n')
		if o.Output != "" {
			return os.WriteFile(o.Output, data, 0o644)
		}
		_, err = out.Write(data)
		return err
	}
	target := llvm_triple_for_target(o.Target)
	if target == "" {
		target = llvm_triple_for_target("")
	}
	ir, err := lower_llvm_ir(program, result, target)
	if err != nil {
		return err
	}
	if o.EmitLLVMIR {
		if o.Output != "" {
			return os.WriteFile(o.Output, ir, 0o644)
		}
		_, err = out.Write(ir)
		return err
	}

	toolchain, err := resolve_toolchain(ToolchainOptions{Target: o.Target, Zig: o.Zig, LLC: o.LLC, LLD: o.LLD, MuslSysroot: o.MuslSysroot}, false)
	if err != nil {
		return err
	}
	output_path := o.Output
	if output_path == "" {
		base := strings.TrimSuffix(filepath.Base(o.Input), filepath.Ext(o.Input))
		output_path = filepath.Join(filepath.Dir(o.Input), base)
	}
	temporary_dir, err := os.MkdirTemp("", "clyth-build-*")
	if err != nil {
		return err
	}
	if os.Getenv("CLYTH_KEEP_TEMP") == "" {
		defer os.RemoveAll(temporary_dir)
	} else {
		fmt.Fprintln(errOut, "temporary build directory:", temporary_dir)
	}

	repository_root := find_repository_root(o.Input)
	units, err := collect_build_translation_units(o.Input, program, repository_root, result.specializations)
	if err != nil {
		return err
	}
	unit_paths := make([]string, 0, len(units))
	for path := range units {
		unit_paths = append(unit_paths, path)
	}
	sort_strings(unit_paths)
	object_paths := []string{}
	mecc_runtime_required := o.Mecc
	for index, unit_path := range unit_paths {
		unit_program := units[unit_path]
		unit_result, unit_semantic_errors, err := analyze_file_with_options(unit_path, unit_program, SemanticOptions{mecc_enabled: o.Mecc})
		if err != nil {
			return err
		}
		if len(unit_semantic_errors) != 0 {
			return fmt.Errorf("semantic analysis failed for import %s with %d error(s)", unit_path, len(unit_semantic_errors))
		}
		// Imported generic units may be specialized with concrete aggregate types
		// declared in the root translation unit. Make those external struct layouts
		// visible during unit lowering so generic aggregate parameters can be cloned
		// with their real concrete shape instead of as opaque pointers.
		for namespaceKey, namespace := range result.namespaces {
			if _, present := unit_result.namespaces[namespaceKey]; !present {
				unit_result.namespaces[namespaceKey] = namespace
			}
		}
		for _, allocation := range unit_result.allocations {
			if allocation != nil && allocation.mecc {
				mecc_runtime_required = true
				break
			}
		}
		unit_ir, err := lower_llvm_ir_with_specializations(unit_program, unit_result, toolchain.LLVMTriple, result.specializations)
		if err != nil {
			return err
		}
		ir_path := filepath.Join(temporary_dir, fmt.Sprintf("unit-%03d.ll", index))
		object_path := filepath.Join(temporary_dir, fmt.Sprintf("unit-%03d.o", index))
		if err := os.WriteFile(ir_path, unit_ir, 0o644); err != nil {
			return err
		}
		if err := compile_llvm_object(toolchain, ir_path, object_path); err != nil {
			return fmt.Errorf("%s: %w", unit_path, err)
		}
		object_paths = append(object_paths, object_path)
	}

	extra_archives := []string{}
	compiler_runtime, err := build_compiler_runtime(toolchain, temporary_dir)
	if err != nil {
		return err
	}
	if compiler_runtime != "" {
		extra_archives = append(extra_archives, compiler_runtime)
	}
	if mecc_runtime_required {
		mecc_runtime, mecc_err := build_mecc_runtime(toolchain, temporary_dir)
		if mecc_err != nil {
			return mecc_err
		}
		if mecc_runtime != "" {
			extra_archives = append(extra_archives, mecc_runtime)
		}
	}
	for _, module_name := range collect_module_imports(units) {
		archive, err := build_runtime_module_archive(toolchain, repository_root, module_name, temporary_dir)
		if err != nil {
			return err
		}
		if archive != "" {
			extra_archives = append(extra_archives, archive)
		}
	}
	if err := link_static_musl(toolchain, object_paths, output_path, extra_archives); err != nil {
		return err
	}
	return nil
}

func print_compiler_diagnostics(result *AnalysisResult, verbose bool, output io.Writer) {
	if result == nil {
		return
	}
	for _, diagnostic := range result.compiler_diagnostics {
		if diagnostic.level == diagnostic_info || diagnostic.level == diagnostic_warning {
			location := ""
			if diagnostic.source_path != "" && diagnostic.source_span.Start.Line > 0 {
				location = fmt.Sprintf("%s:%d:%d: ", diagnostic.source_path, diagnostic.source_span.Start.Line, diagnostic.source_span.Start.Column)
			}
			fmt.Fprintf(output, "%s%s [%s]: %s\n", location, diagnostic.level, diagnostic.code, diagnostic.message)
		}
	}
	if !verbose {
		return
	}
	for _, action := range result.lifetime_actions {
		info := result.allocations[action.allocation]
		location := ""
		if info != nil {
			location = fmt.Sprintf("%s:%d:%d: ", info.source_path, info.source_span.Start.Line, info.source_span.Start.Column)
		}
		switch action.kind {
		case lifetime_cleanup:
			fmt.Fprintf(output, "%sinfo [LIFETIME001]: compiler inserted destruction for allocation #%d (%s)\n", location, action.allocation, action.reason)
		case lifetime_explicit_free:
			fmt.Fprintf(output, "%sinfo [LIFETIME002]: explicit free accepted for allocation #%d; compiler cleanup suppressed\n", location, action.allocation)
		case lifetime_mecc:
			fmt.Fprintf(output, "%sinfo [MECC_VERBOSE]: allocation #%d uses an estate (compacting=%t; %s)\n", location, action.allocation, action.policy.compacting, action.reason)
		}
	}
}

// collect_local_translation_units walks only quoted source imports. Module imports
// remain package identities and are resolved separately by the package/toolchain layer.
func collect_local_translation_units(root_path string, root_program *ast.Program) (map[string]*ast.Program, error) {
	units := map[string]*ast.Program{}
	var visit func(string, *ast.Program) error
	visit = func(path string, program *ast.Program) error {
		absolute, err := filepath.Abs(path)
		if err != nil {
			return err
		}
		if units[absolute] != nil {
			return nil
		}
		units[absolute] = program
		for _, declaration := range program.Declarations {
			import_decl, ok := declaration.(*ast.ImportDecl)
			if !ok || import_decl.Kind != ast.ImportFile {
				continue
			}
			child := import_decl.Target
			if !filepath.IsAbs(child) {
				child = filepath.Join(filepath.Dir(absolute), child)
			}
			child_program, frontend_errors, err := ParseFile(child)
			if err != nil {
				return err
			}
			if len(frontend_errors) != 0 {
				return fmt.Errorf("import %s has %d frontend error(s)", child, len(frontend_errors))
			}
			if err := visit(child, child_program); err != nil {
				return err
			}
		}
		return nil
	}
	if err := visit(root_path, root_program); err != nil {
		return nil, err
	}
	return units, nil
}

func module_source_is_requested(program *ast.Program, source_count int, requested_specializations []GenericSpecialization) bool {
	if source_count <= 1 {
		return true
	}
	requested := map[string]bool{}
	for _, specialization := range requested_specializations {
		if specialization_is_concrete(specialization) {
			requested[specialization.base_name] = true
		}
	}
	for _, declaration := range program.Declarations {
		if item, ok := declaration.(*ast.StructDecl); ok && requested[item.Name] {
			return true
		}
	}
	return false
}

// collect_build_translation_units expands both quoted source imports and the Clyth
// source portions of unquoted modules. Native C/C++ bindings are still built by
// build_runtime_module_archive; this function only adds module-owned .clyth code
// to the ordinary LLVM translation-unit pipeline.
func collect_build_translation_units(root_path string, root_program *ast.Program, repository_root string, requested_specializations []GenericSpecialization) (map[string]*ast.Program, error) {
	units, err := collect_local_translation_units(root_path, root_program)
	if err != nil {
		return nil, err
	}

	// Clyth Core is compiler-owned and implicitly participates in every native
	// build. Its source is visible/auditable like any other Clyth module, but it
	// cannot be shadowed by a project import. Compile-time-only declarations
	// naturally lower to no object code; runtime intrinsic methods are linked
	// exactly like other Clyth translation units.
	coreDir := filepath.Join(repository_root, "module-src", "core", "clyth-core", "src")
	coreFiles, _ := filepath.Glob(filepath.Join(coreDir, "*.clyth"))
	sort_strings(coreFiles)
	for _, sourceFile := range coreFiles {
		absolute, absErr := filepath.Abs(sourceFile)
		if absErr != nil {
			return nil, absErr
		}
		if units[absolute] != nil {
			continue
		}
		coreProgram, frontendErrors, parseErr := ParseFile(absolute)
		if parseErr != nil {
			return nil, parseErr
		}
		if len(frontendErrors) != 0 {
			return nil, fmt.Errorf("Clyth Core source %s has %d frontend error(s)", absolute, len(frontendErrors))
		}
		units[absolute] = coreProgram
	}

	visited_modules := map[string]bool{"clyth-core": true}
	for {
		changed := false
		for _, module_name := range collect_module_imports(units) {
			if visited_modules[module_name] {
				continue
			}
			visited_modules[module_name] = true
			module_dir := ""
			for _, candidate := range []string{filepath.Join(repository_root, "module-src", "runtime", module_name), filepath.Join(repository_root, "module-src", "core", module_name)} {
				if info, stat_err := os.Stat(candidate); stat_err == nil && info.IsDir() {
					module_dir = candidate
					break
				}
			}
			if module_dir == "" {
				continue
			}
			source_files, _ := filepath.Glob(filepath.Join(module_dir, "src", "*.clyth"))
			sort_strings(source_files)
			for _, source_file := range source_files {
				absolute, abs_err := filepath.Abs(source_file)
				if abs_err != nil {
					return nil, abs_err
				}
				if units[absolute] != nil {
					continue
				}
				module_program, frontend_errors, parse_err := ParseFile(absolute)
				if parse_err != nil {
					return nil, parse_err
				}
				if len(frontend_errors) != 0 {
					return nil, fmt.Errorf("module source %s has %d frontend error(s)", absolute, len(frontend_errors))
				}
				if !module_source_is_requested(module_program, len(source_files), requested_specializations) {
					continue
				}
				local_units, local_err := collect_local_translation_units(absolute, module_program)
				if local_err != nil {
					return nil, local_err
				}
				for local_path, local_program := range local_units {
					if units[local_path] == nil {
						units[local_path] = local_program
						changed = true
					}
				}
			}
		}
		if !changed {
			break
		}
	}
	return units, nil
}

func collect_module_imports(units map[string]*ast.Program) []string {
	seen := map[string]bool{}
	for _, program := range units {
		for _, declaration := range program.Declarations {
			if import_decl, ok := declaration.(*ast.ImportDecl); ok && import_decl.Kind == ast.ImportModule {
				seen[import_decl.Target] = true
			}
		}
	}
	result := make([]string, 0, len(seen))
	for name := range seen {
		result = append(result, name)
	}
	sort_strings(result)
	return result
}
