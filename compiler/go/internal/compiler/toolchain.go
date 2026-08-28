package compiler

import (
	"errors"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strconv"
	"strings"
)

type ToolchainOptions struct {
	Target      string
	Zig         string
	LLC         string
	LLD         string
	MuslSysroot string
}

type ResolvedToolchain struct {
	Target      string
	LLVMTriple  string
	Zig         string
	LLC         string
	LLD         string
	MuslSysroot string
}

func resolve_toolchain(options ToolchainOptions, require_zig bool) (*ResolvedToolchain, error) {
	target := options.Target
	if target == "" {
		switch runtime.GOARCH {
		case "amd64":
			target = "x86_64-linux-musl"
		case "arm64":
			target = "aarch64-linux-musl"
		default:
			target = runtime.GOARCH + "-linux-musl"
		}
	}
	resolved := &ResolvedToolchain{Target: target, LLVMTriple: llvm_triple_for_target(target)}
	var err error
	if require_zig {
		resolved.Zig, err = resolve_tool(options.Zig, "CLYTH_ZIG", []string{"zig"})
		if err != nil {
			return nil, err
		}
	} else {
		resolved.Zig, _ = resolve_tool(options.Zig, "CLYTH_ZIG", []string{"zig"})
	}
	resolved.LLC, err = resolve_tool(options.LLC, "CLYTH_LLC", []string{"llc"})
	if err != nil {
		return nil, err
	}
	resolved.LLD, err = resolve_tool(options.LLD, "CLYTH_LLD", []string{"ld.lld", "lld"})
	if err != nil {
		return nil, err
	}
	resolved.MuslSysroot = options.MuslSysroot
	if resolved.MuslSysroot == "" {
		resolved.MuslSysroot = os.Getenv("CLYTH_MUSL_SYSROOT")
	}
	if resolved.MuslSysroot == "" {
		return nil, errors.New("Clyth musl sysroot is required (--musl-sysroot or CLYTH_MUSL_SYSROOT)")
	}
	resolved.MuslSysroot, err = filepath.Abs(resolved.MuslSysroot)
	if err != nil {
		return nil, err
	}
	if err := validate_musl_sysroot(resolved.MuslSysroot); err != nil {
		return nil, err
	}
	return resolved, nil
}

func resolve_tool(explicit string, env_name string, candidates []string) (string, error) {
	if explicit != "" {
		absolute, err := filepath.Abs(explicit)
		if err != nil {
			return "", err
		}
		if info, err := os.Stat(absolute); err == nil && !info.IsDir() {
			return absolute, nil
		}
		return "", fmt.Errorf("tool %q does not exist", absolute)
	}
	if value := os.Getenv(env_name); value != "" {
		return resolve_tool(value, "", nil)
	}
	for _, candidate := range candidates {
		if path, err := exec.LookPath(candidate); err == nil {
			return path, nil
		}
	}
	return "", fmt.Errorf("required tool not found: %s", strings.Join(candidates, " or "))
}

func validate_musl_sysroot(root string) error {
	required := []string{"include", filepath.Join("lib", "libc.a"), filepath.Join("lib", "crt1.o"), filepath.Join("lib", "crti.o"), filepath.Join("lib", "crtn.o")}
	for _, relative := range required {
		if _, err := os.Stat(filepath.Join(root, relative)); err != nil {
			return fmt.Errorf("invalid Clyth musl sysroot %q: missing %s", root, relative)
		}
	}
	return nil
}

func llvm_triple_for_target(target string) string {
	switch target {
	case "x86_64-linux-musl", "x86_64-unknown-linux-musl":
		return "x86_64-unknown-linux-musl"
	case "aarch64-linux-musl", "aarch64-unknown-linux-musl":
		return "aarch64-unknown-linux-musl"
	case "arm-linux-musleabihf", "armv7-linux-musleabihf":
		return "armv7-unknown-linux-musleabihf"
	default:
		return target
	}
}

func compile_llvm_object(toolchain *ResolvedToolchain, ir_path string, object_path string) error {
	command := exec.Command(toolchain.LLC, "-filetype=obj", "-mtriple="+toolchain.LLVMTriple, "-o", object_path, ir_path)
	output, err := command.CombinedOutput()
	if err != nil {
		return fmt.Errorf("llc failed: %w\n%s", err, output)
	}
	return nil
}

func link_static_musl(toolchain *ResolvedToolchain, object_paths []string, output_path string, extra_archives []string) error {
	lib := filepath.Join(toolchain.MuslSysroot, "lib")
	arguments := []string{"-static", "-o", output_path, filepath.Join(lib, "crt1.o"), filepath.Join(lib, "crti.o")}
	arguments = append(arguments, object_paths...)
	arguments = append(arguments, extra_archives...)
	arguments = append(arguments, filepath.Join(lib, "libc.a"), filepath.Join(lib, "crtn.o"))
	command := exec.Command(toolchain.LLD, arguments...)
	output, err := command.CombinedOutput()
	if err != nil {
		return fmt.Errorf("lld failed: %w\n%s", err, output)
	}
	return nil
}

func build_compiler_runtime(toolchain *ResolvedToolchain, output_dir string) (string, error) {
	if toolchain.Target != "aarch64-linux-musl" && toolchain.Target != "aarch64-unknown-linux-musl" && toolchain.Target != "arm-linux-musleabihf" && toolchain.Target != "armv7-linux-musleabihf" {
		return "", nil
	}
	if toolchain.Zig == "" {
		return "", fmt.Errorf("target %s requires compiler runtime builtins; provide Zig with --zig or CLYTH_ZIG", toolchain.Target)
	}
	command := exec.Command(toolchain.Zig, "env")
	data, err := command.Output()
	if err != nil {
		return "", fmt.Errorf("zig env failed: %w", err)
	}
	lib_dir := ""
	for _, line := range strings.Split(string(data), "\n") {
		trimmed := strings.TrimSpace(line)
		if strings.HasPrefix(trimmed, ".lib_dir = ") {
			value := strings.TrimSpace(strings.TrimPrefix(trimmed, ".lib_dir = "))
			value = strings.TrimSuffix(value, ",")
			if unquoted, parse_err := strconv.Unquote(value); parse_err == nil {
				lib_dir = unquoted
			}
			break
		}
	}
	if lib_dir == "" {
		return "", errors.New("zig env did not report lib_dir")
	}
	source := filepath.Join(lib_dir, "compiler_rt.zig")
	archive := filepath.Join(output_dir, "libclyth_compiler_rt.a")
	build := exec.Command(toolchain.Zig, "build-lib", source, "-target", toolchain.Target, "-O", "ReleaseFast", "-femit-bin="+archive)
	combined, err := build.CombinedOutput()
	if err != nil {
		return "", fmt.Errorf("building Zig compiler runtime failed: %w\n%s", err, combined)
	}
	return archive, nil
}

// compile_native_wrapper uses Zig only as a compiler driver. The include search
// path is replaced with the Clyth-managed musl headers so Zig's bundled libc can
// never silently define the runtime ABI of a Clyth module.
func compile_native_wrapper(toolchain *ResolvedToolchain, source string, output string, cxx bool) error {
	if toolchain.Zig == "" {
		return errors.New("Zig is required to compile native module wrappers")
	}
	mode := "cc"
	if cxx {
		mode = "c++"
	}
	arguments := []string{mode, "-target", toolchain.Target, "-O2", "-fno-sanitize=all", "-nostdinc", "-isystem", filepath.Join(toolchain.MuslSysroot, "include"), "-c", source, "-o", output}
	command := exec.Command(toolchain.Zig, arguments...)
	data, err := command.CombinedOutput()
	if err != nil {
		return fmt.Errorf("zig %s failed: %w\n%s", mode, err, data)
	}
	return nil
}

func find_repository_root(start string) string {
	current := start
	if info, err := os.Stat(current); err == nil && !info.IsDir() {
		current = filepath.Dir(current)
	}
	current, _ = filepath.Abs(current)
	for {
		if info, err := os.Stat(filepath.Join(current, "module-src")); err == nil && info.IsDir() {
			return current
		}
		parent := filepath.Dir(current)
		if parent == current {
			return ""
		}
		current = parent
	}
}

// build_runtime_module_archive compiles one repository-native module binding with
// Zig while explicitly using the selected Clyth musl headers. Vendor-backed modules
// remain the responsibility of the package/module build step until their pinned
// source fetch metadata is integrated into the Go package manager.
func build_runtime_module_archive(toolchain *ResolvedToolchain, repository_root string, module_name string, temporary_dir string) (string, error) {
	if repository_root == "" {
		return "", nil
	}
	binding := filepath.Join(repository_root, "module-src", "runtime", module_name, "c-binding")
	source_dir := filepath.Join(binding, "src")
	if info, err := os.Stat(source_dir); err != nil || !info.IsDir() {
		return "", nil
	}
	if toolchain.Zig == "" {
		return "", fmt.Errorf("module %s has native bindings and requires Zig (--zig or CLYTH_ZIG)", module_name)
	}
	vendor_root := ""
	if _, err := os.Stat(filepath.Join(binding, "fetch-vendors.sh")); err == nil {
		command := exec.Command(filepath.Join(binding, "fetch-vendors.sh"))
		command.Env = append(os.Environ(), "CLYTH_VENDOR_CACHE="+filepath.Join(repository_root, "module-src", "vendor"))
		data, fetch_err := command.CombinedOutput()
		if fetch_err != nil {
			return "", fmt.Errorf("resolving pinned vendor source for module %s: %w\n%s", module_name, fetch_err, data)
		}
		vendor_root = strings.TrimSpace(string(data))
	}
	entries, err := os.ReadDir(source_dir)
	if err != nil {
		return "", err
	}
	module_dir := filepath.Join(temporary_dir, "native-"+sanitize_llvm_name(module_name))
	if err := os.MkdirAll(module_dir, 0o755); err != nil {
		return "", err
	}
	objects := []string{}
	for _, entry := range entries {
		if entry.IsDir() || filepath.Ext(entry.Name()) != ".c" {
			continue
		}
		source := filepath.Join(source_dir, entry.Name())
		object := filepath.Join(module_dir, strings.TrimSuffix(entry.Name(), ".c")+".o")
		arguments := []string{"cc", "-target", toolchain.Target, "-O2", "-fno-sanitize=all", "-nostdinc", "-isystem", filepath.Join(toolchain.MuslSysroot, "include")}
		if info, err := os.Stat(filepath.Join(binding, "include")); err == nil && info.IsDir() {
			arguments = append(arguments, "-I", filepath.Join(binding, "include"))
		}
		if vendor_root != "" {
			for _, include_dir := range []string{vendor_root, filepath.Join(vendor_root, "include"), filepath.Join(vendor_root, "src")} {
				if info, stat_err := os.Stat(include_dir); stat_err == nil && info.IsDir() {
					arguments = append(arguments, "-I", include_dir)
				}
			}
		}
		arguments = append(arguments, "-std=gnu11", "-fPIC", "-c", source, "-o", object)
		command := exec.Command(toolchain.Zig, arguments...)
		data, err := command.CombinedOutput()
		if err != nil {
			return "", fmt.Errorf("building native module %s: %w\n%s", module_name, err, data)
		}
		objects = append(objects, object)
	}
	if vendor_root != "" {
		vendor_sources, _ := filepath.Glob(filepath.Join(vendor_root, "src", "*.c"))
		for _, source := range vendor_sources {
			object := filepath.Join(module_dir, "vendor-"+strings.TrimSuffix(filepath.Base(source), ".c")+".o")
			arguments := []string{"cc", "-target", toolchain.Target, "-O2", "-fno-sanitize=all", "-nostdinc", "-isystem", filepath.Join(toolchain.MuslSysroot, "include"), "-I", vendor_root, "-I", filepath.Join(vendor_root, "src"), "-std=gnu11", "-fPIC", "-c", source, "-o", object}
			command := exec.Command(toolchain.Zig, arguments...)
			data, compile_err := command.CombinedOutput()
			if compile_err != nil {
				return "", fmt.Errorf("building vendor source for native module %s: %w\n%s", module_name, compile_err, data)
			}
			objects = append(objects, object)
		}
	}
	if len(objects) == 0 {
		return "", nil
	}
	archive := filepath.Join(module_dir, "lib"+module_name+"_binding.a")
	arguments := []string{"ar", "rcs", archive}
	arguments = append(arguments, objects...)
	command := exec.Command(toolchain.Zig, arguments...)
	data, err := command.CombinedOutput()
	if err != nil {
		return "", fmt.Errorf("archiving native module %s: %w\n%s", module_name, err, data)
	}
	return archive, nil
}

// build_mecc_runtime builds the small compiler-owned estate runtime only when a
// translation unit actually requires MECC. The first V1.0.0 runtime is
// deliberately conservative: adopted allocations are registered in an estate
// table, explicit releases can reclaim them early, and any remaining estate
// allocations are reclaimed at process shutdown. Compiler-emitted edge/root
// instrumentation can refine that destruction frontier without changing this ABI.
func build_mecc_runtime(toolchain *ResolvedToolchain, output_dir string) (string, error) {
	if toolchain.Zig == "" {
		return "", errors.New("MECC runtime requires Zig to compile its native support")
	}
	source := filepath.Join(output_dir, "clyth_mecc_runtime.c")
	object := filepath.Join(output_dir, "clyth_mecc_runtime.o")
	archive := filepath.Join(output_dir, "libclyth_mecc.a")
	code := `
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef void (*ClythMeccDestructor)(void *);

typedef struct ClythMeccNode {
    void *pointer;
    bool compacting;
    bool marked;
    ClythMeccDestructor destructor;
    struct ClythMeccNode *next;
} ClythMeccNode;

typedef struct ClythMeccReference {
    void *owner;
    void *slot;
    void *target;
    struct ClythMeccReference *next;
} ClythMeccReference;

static ClythMeccNode *clyth_mecc_nodes = NULL;
static ClythMeccReference *clyth_mecc_references = NULL;
static bool clyth_mecc_atexit_registered = false;

static ClythMeccNode *clyth_mecc_find(void *pointer) {
    ClythMeccNode *node = clyth_mecc_nodes;
    while (node != NULL) {
        if (node->pointer == pointer) return node;
        node = node->next;
    }
    return NULL;
}

static ClythMeccReference *clyth_mecc_find_reference(void *slot) {
    ClythMeccReference *reference = clyth_mecc_references;
    while (reference != NULL) {
        if (reference->slot == slot) return reference;
        reference = reference->next;
    }
    return NULL;
}

static void clyth_mecc_remove_reference_slot(void *slot) {
    ClythMeccReference **cursor = &clyth_mecc_references;
    while (*cursor != NULL) {
        ClythMeccReference *reference = *cursor;
        if (reference->slot == slot) {
            *cursor = reference->next;
            free(reference);
            return;
        }
        cursor = &reference->next;
    }
}

static void clyth_mecc_remove_owner_references(void *owner) {
    ClythMeccReference **cursor = &clyth_mecc_references;
    while (*cursor != NULL) {
        ClythMeccReference *reference = *cursor;
        if (reference->owner == owner) {
            *cursor = reference->next;
            free(reference);
            continue;
        }
        cursor = &reference->next;
    }
}

static void clyth_mecc_remove_target_references(void *target) {
    ClythMeccReference **cursor = &clyth_mecc_references;
    while (*cursor != NULL) {
        ClythMeccReference *reference = *cursor;
        if (reference->target == target) {
            *cursor = reference->next;
            free(reference);
            continue;
        }
        cursor = &reference->next;
    }
}

static void clyth_mecc_destroy_node(ClythMeccNode *node) {
    if (node == NULL) return;
    clyth_mecc_remove_owner_references(node->pointer);
    clyth_mecc_remove_target_references(node->pointer);
    if (node->destructor != NULL) node->destructor(node->pointer);
    free(node->pointer);
    free(node);
}

static void clyth_mecc_mark(ClythMeccNode *node) {
    if (node == NULL || node->marked) return;
    node->marked = true;
    ClythMeccReference *reference = clyth_mecc_references;
    while (reference != NULL) {
        if (reference->owner == node->pointer) {
            clyth_mecc_mark(clyth_mecc_find(reference->target));
        }
        reference = reference->next;
    }
}

void clyth_mecc_collapse(void) {
    ClythMeccNode *node = clyth_mecc_nodes;
    while (node != NULL) {
        node->marked = false;
        node = node->next;
    }

    ClythMeccReference *reference = clyth_mecc_references;
    while (reference != NULL) {
        if (reference->owner == NULL || clyth_mecc_find(reference->owner) == NULL) {
            clyth_mecc_mark(clyth_mecc_find(reference->target));
        }
        reference = reference->next;
    }

    ClythMeccNode **cursor = &clyth_mecc_nodes;
    while (*cursor != NULL) {
        ClythMeccNode *current = *cursor;
        if (!current->marked) {
            *cursor = current->next;
            clyth_mecc_destroy_node(current);
            continue;
        }
        cursor = &current->next;
    }
}

static void clyth_mecc_shutdown(void) {
    ClythMeccReference *reference = clyth_mecc_references;
    while (reference != NULL) {
        ClythMeccReference *next = reference->next;
        free(reference);
        reference = next;
    }
    clyth_mecc_references = NULL;

    ClythMeccNode *node = clyth_mecc_nodes;
    clyth_mecc_nodes = NULL;
    while (node != NULL) {
        ClythMeccNode *next = node->next;
        if (node->destructor != NULL) node->destructor(node->pointer);
        free(node->pointer);
        free(node);
        node = next;
    }
}

void *clyth_mecc_adopt(void *pointer, bool compacting, ClythMeccDestructor destructor) {
    if (pointer == NULL) return NULL;
    ClythMeccNode *existing = clyth_mecc_find(pointer);
    if (existing != NULL) {
        if (existing->destructor == NULL) existing->destructor = destructor;
        return pointer;
    }
    ClythMeccNode *node = (ClythMeccNode *)calloc(1, sizeof(ClythMeccNode));
    if (node == NULL) return pointer;
    node->pointer = pointer;
    node->compacting = compacting;
    node->destructor = destructor;
    node->next = clyth_mecc_nodes;
    clyth_mecc_nodes = node;
    if (!clyth_mecc_atexit_registered) {
        atexit(clyth_mecc_shutdown);
        clyth_mecc_atexit_registered = true;
    }
    return pointer;
}

// Store one managed pointer reference. slot is the stable address containing
// the pointer. If owner is a managed node the reference is an estate edge;
// otherwise it is an external/root reference. Storing null or an unmanaged
// pointer removes any previous MECC reference for that slot.
void clyth_mecc_store(void *owner, void *slot, void *target) {
    if (slot == NULL) return;
    ClythMeccNode *target_node = clyth_mecc_find(target);
    if (target_node == NULL) {
        clyth_mecc_remove_reference_slot(slot);
        return;
    }
    ClythMeccReference *reference = clyth_mecc_find_reference(slot);
    if (reference == NULL) {
        reference = (ClythMeccReference *)calloc(1, sizeof(ClythMeccReference));
        if (reference == NULL) return;
        reference->slot = slot;
        reference->next = clyth_mecc_references;
        clyth_mecc_references = reference;
    }
    reference->owner = owner;
    reference->target = target;
}

void clyth_mecc_owner_release(void *owner) {
    if (owner == NULL) return;
    clyth_mecc_remove_owner_references(owner);
}

// Explicit Clyth free remains authoritative. This removes the object from the
// estate immediately even if stale aliases still exist; semantic analysis is
// responsible for diagnosing provable UAF/double-free and warning on stale
// container references.
void clyth_mecc_release(void *pointer) {
    if (pointer == NULL) return;
    ClythMeccNode **cursor = &clyth_mecc_nodes;
    while (*cursor != NULL) {
        ClythMeccNode *node = *cursor;
        if (node->pointer == pointer) {
            *cursor = node->next;
            clyth_mecc_destroy_node(node);
            return;
        }
        cursor = &node->next;
    }
}
`
	if err := os.WriteFile(source, []byte(code), 0o644); err != nil {
		return "", err
	}
	if err := compile_native_wrapper(toolchain, source, object, false); err != nil {
		return "", err
	}
	command := exec.Command(toolchain.Zig, "ar", "rcs", archive, object)
	data, err := command.CombinedOutput()
	if err != nil {
		return "", fmt.Errorf("archiving MECC runtime failed: %w\n%s", err, data)
	}
	return archive, nil
}
