package packagecmd

import (
	"archive/tar"
	"compress/gzip"
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"io/fs"
	"net/http"
	"os"
	"os/exec"
	"path/filepath"
	"sort"
	"strings"
)

type moduleInfo struct {
	Name         string   `json:"name"`
	Version      string   `json:"version"`
	Dependencies []string `json:"dependencies"`
}

type registry struct {
	Type     string `json:"type"`
	Location string `json:"location"`
}

type registryFile struct {
	Registries map[string]registry `json:"registries"`
}

type registryPackage struct {
	File           string `json:"file"`
	SHA256         string `json:"sha256"`
	SourceURL      string `json:"source-url,omitempty"`
	SourceRevision string `json:"source-revision,omitempty"`
	LicenseSPDX    string `json:"license-spdx,omitempty"`
	LicenseFile    string `json:"license-file,omitempty"`
}

type registryIndex struct {
	Packages map[string]map[string]registryPackage `json:"packages"`
}

type lockEntry struct {
	Version        string `json:"version"`
	Registry       string `json:"registry,omitempty"`
	SHA256         string `json:"sha256"`
	Package        string `json:"package"`
	Source         string `json:"source,omitempty"`
	SourceRevision string `json:"source-revision,omitempty"`
	InstallKind    string `json:"install-kind"`
	LicenseSPDX    string `json:"license-spdx,omitempty"`
	LicenseFile    string `json:"license-file,omitempty"`
}

type lockFile struct {
	Packages map[string]lockEntry `json:"packages"`
}

func Run(command string, args []string, out, errOut io.Writer) error {
	switch command {
	case "package":
		if len(args) != 1 {
			return errors.New("usage: clyth package <module-directory>")
		}
		archive, err := Package(args[0])
		if err == nil {
			fmt.Fprintln(out, archive)
		}
		return err
	case "add-registry", "update-registry":
		return setRegistry(command == "update-registry", args, out)
	case "remove-registry":
		if len(args) != 1 {
			return errors.New("usage: clyth remove-registry <name>")
		}
		return removeRegistry(args[0], out)
	case "list-registries":
		return listRegistries(out)
	case "registry-info":
		return registryInfo(args, out)
	case "list":
		return list_packages(out)
	case "info":
		return package_info(args, out)
	case "init":
		return init_project(args, out)
	case "debug":
		return debug_project(out)
	case "update":
		return updatePackages(args, out)
	case "install":
		return install(args, out)
	case "remove":
		if len(args) != 1 {
			return errors.New("usage: clyth remove <package>")
		}
		return removePackage(args[0], out)
	case "refresh":
		return refresh(out)
	default:
		return fmt.Errorf("unsupported package command %q", command)
	}
}

func clythHome() string {
	if v := os.Getenv("CLYTH_HOME"); v != "" {
		return v
	}
	home, _ := os.UserHomeDir()
	return filepath.Join(home, ".clyth")
}

func readJSON(path string, v any) error {
	b, err := os.ReadFile(path)
	if err != nil {
		return err
	}
	return json.Unmarshal(b, v)
}

func writeJSON(path string, v any) error {
	if err := os.MkdirAll(filepath.Dir(path), 0o755); err != nil {
		return err
	}
	b, err := json.MarshalIndent(v, "", "  ")
	if err != nil {
		return err
	}
	return os.WriteFile(path, append(b, '\n'), 0o644)
}

func loadRegistries() (registryFile, error) {
	r := registryFile{Registries: map[string]registry{}}
	path := filepath.Join(clythHome(), "registries.json")
	if err := readJSON(path, &r); err != nil && !errors.Is(err, os.ErrNotExist) {
		return r, err
	}
	if r.Registries == nil {
		r.Registries = map[string]registry{}
	}
	return r, nil
}

func saveRegistries(r registryFile) error {
	return writeJSON(filepath.Join(clythHome(), "registries.json"), r)
}

func setRegistry(update bool, args []string, out io.Writer) error {
	if len(args) != 3 || (args[1] != "--path" && args[1] != "--url") {
		return errors.New("usage: clyth add-registry <name> (--path <directory> | --url <remote-url>)")
	}
	r, err := loadRegistries()
	if err != nil {
		return err
	}
	_, exists := r.Registries[args[0]]
	if update && !exists {
		return fmt.Errorf("registry %q does not exist", args[0])
	}
	if !update && exists {
		return fmt.Errorf("registry %q already exists", args[0])
	}
	location := args[2]
	registry_type := "remote"
	if args[1] == "--path" {
		abs, path_err := filepath.Abs(args[2])
		if path_err != nil {
			return path_err
		}
		location = abs
		registry_type = "filesystem"
	}
	r.Registries[args[0]] = registry{Type: registry_type, Location: location}
	if err := saveRegistries(r); err != nil {
		return err
	}
	fmt.Fprintf(out, "%s registry %s -> %s\n", map[bool]string{true: "Updated", false: "Added"}[update], args[0], location)
	return nil
}

func removeRegistry(name string, out io.Writer) error {
	r, err := loadRegistries()
	if err != nil {
		return err
	}
	if _, ok := r.Registries[name]; !ok {
		return fmt.Errorf("registry %q does not exist", name)
	}
	delete(r.Registries, name)
	if err := saveRegistries(r); err != nil {
		return err
	}
	fmt.Fprintln(out, "Removed registry", name)
	return nil
}

func listRegistries(out io.Writer) error {
	r, err := loadRegistries()
	if err != nil {
		return err
	}
	names := make([]string, 0, len(r.Registries))
	for n := range r.Registries {
		names = append(names, n)
	}
	sort.Strings(names)
	for _, n := range names {
		fmt.Fprintf(out, "%s\t%s\n", n, r.Registries[n].Location)
	}
	return nil
}

func Package(dir string) (string, error) {
	abs, err := filepath.Abs(dir)
	if err != nil {
		return "", err
	}
	var info moduleInfo
	if err := readJSON(filepath.Join(abs, "module-info.json"), &info); err != nil {
		return "", fmt.Errorf("read module-info.json: %w", err)
	}
	if info.Name == "" || info.Version == "" {
		return "", errors.New("module-info.json requires name and version")
	}
	buildDir := filepath.Join(abs, "build")
	if err := os.MkdirAll(buildDir, 0o755); err != nil {
		return "", err
	}
	archive := filepath.Join(buildDir, fmt.Sprintf("%s-%s.tar.gz", info.Name, info.Version))
	f, err := os.Create(archive)
	if err != nil {
		return "", err
	}
	defer f.Close()
	gz := gzip.NewWriter(f)
	defer gz.Close()
	tw := tar.NewWriter(gz)
	defer tw.Close()
	rootName := fmt.Sprintf("%s-%s", info.Name, info.Version)
	skip := map[string]bool{"build": true, ".git": true}
	err = filepath.WalkDir(abs, func(path string, d fs.DirEntry, walkErr error) error {
		if walkErr != nil {
			return walkErr
		}
		rel, err := filepath.Rel(abs, path)
		if err != nil {
			return err
		}
		if rel == "." {
			return nil
		}
		first := strings.Split(rel, string(filepath.Separator))[0]
		if skip[first] {
			if d.IsDir() {
				return filepath.SkipDir
			}
			return nil
		}
		info, err := d.Info()
		if err != nil {
			return err
		}
		hdr, err := tar.FileInfoHeader(info, "")
		if err != nil {
			return err
		}
		hdr.Name = filepath.ToSlash(filepath.Join(rootName, rel))
		if err := tw.WriteHeader(hdr); err != nil {
			return err
		}
		if d.Type().IsRegular() {
			rf, err := os.Open(path)
			if err != nil {
				return err
			}
			_, cpErr := io.Copy(tw, rf)
			rf.Close()
			return cpErr
		}
		return nil
	})
	if err != nil {
		return "", err
	}
	if err := tw.Close(); err != nil {
		return "", err
	}
	if err := gz.Close(); err != nil {
		return "", err
	}
	return archive, nil
}

func shaFile(path string) (string, error) {
	f, err := os.Open(path)
	if err != nil {
		return "", err
	}
	defer f.Close()
	h := sha256.New()
	if _, err := io.Copy(h, f); err != nil {
		return "", err
	}
	return hex.EncodeToString(h.Sum(nil)), nil
}

func loadLock(root string) (lockFile, error) {
	l := lockFile{Packages: map[string]lockEntry{}}
	err := readJSON(filepath.Join(root, "clyth_package.json"), &l)
	if err != nil && !errors.Is(err, os.ErrNotExist) {
		return l, err
	}
	if l.Packages == nil {
		l.Packages = map[string]lockEntry{}
	}
	return l, nil
}

func saveLock(root string, l lockFile) error {
	return writeJSON(filepath.Join(root, "clyth_package.json"), l)
}

func install(args []string, out io.Writer) error { return install_internal(args, out, true) }

func install_internal(args []string, out io.Writer, direct bool) error {
	cwd, _ := os.Getwd()
	project_root := cwd
	if discovered, discover_err := find_project_root(cwd); discover_err == nil {
		project_root = discovered
	}
	if len(args) == 2 && args[0] == "-f" {
		return install_local_archive(project_root, args[1], out)
	}
	if len(args) == 0 {
		return refresh(out)
	}
	name := args[0]
	selected := ""
	build_from_source := false
	for _, a := range args[1:] {
		if strings.HasPrefix(a, "--from-registry=") {
			selected = strings.TrimPrefix(a, "--from-registry=")
		}
		if a == "--build-from-source" {
			build_from_source = true
		}
	}
	regs, err := loadRegistries()
	if err != nil {
		return err
	}
	names := make([]string, 0, len(regs.Registries))
	for n := range regs.Registries {
		if selected == "" || selected == n {
			names = append(names, n)
		}
	}
	sort.Strings(names)
	if selected != "" {
		if _, ok := regs.Registries[selected]; !ok {
			return fmt.Errorf("unknown registry %q", selected)
		}
	}
	for _, rn := range names {
		reg := regs.Registries[rn]
		idx, index_err := readRegistryIndex(reg)
		if index_err != nil {
			continue
		}
		versions := idx.Packages[name]
		if len(versions) == 0 {
			continue
		}
		vs := make([]string, 0, len(versions))
		for v := range versions {
			vs = append(vs, v)
		}
		sort.Strings(vs)
		v := vs[len(vs)-1]
		meta := versions[v]
		if build_from_source {
			if err := buildFromSource(project_root, name, v, rn, meta, out); err != nil {
				return err
			}
			if direct {
				return record_project_dependency(project_root, name, v)
			}
			return nil
		}
		if reg.Type == "filesystem" {
			if err := installArchive(project_root, filepath.Join(reg.Location, meta.File), rn, out); err != nil {
				return err
			}
			if direct {
				return record_project_dependency(project_root, name, v)
			}
			return nil
		}
		artifact_url := strings.TrimRight(reg.Location, "/") + "/" + strings.TrimLeft(meta.File, "/")
		temp_file, temp_err := os.CreateTemp("", "clyth-package-*.tar.gz")
		if temp_err != nil {
			return temp_err
		}
		temp_path := temp_file.Name()
		temp_file.Close()
		defer os.Remove(temp_path)
		resp, fetch_err := http.Get(artifact_url)
		if fetch_err != nil {
			return fetch_err
		}
		defer resp.Body.Close()
		if resp.StatusCode != http.StatusOK {
			return fmt.Errorf("package download returned %s", resp.Status)
		}
		target, _ := os.Create(temp_path)
		_, copy_err := io.Copy(target, resp.Body)
		target.Close()
		if copy_err != nil {
			return copy_err
		}
		if err := installArchive(project_root, temp_path, rn, out); err != nil {
			return err
		}
		if direct {
			return record_project_dependency(project_root, name, v)
		}
		return nil
	}
	return fmt.Errorf("package %q was not found in configured registries", name)
}

func installArchive(projectRoot, archive, registryName string, out io.Writer) error {
	abs, err := filepath.Abs(archive)
	if err != nil {
		return err
	}
	hash, err := shaFile(abs)
	if err != nil {
		return err
	}
	f, err := os.Open(abs)
	if err != nil {
		return err
	}
	defer f.Close()
	gz, err := gzip.NewReader(f)
	if err != nil {
		return err
	}
	defer gz.Close()
	tr := tar.NewReader(gz)
	var info moduleInfo
	var payload []struct {
		name string
		mode fs.FileMode
		data []byte
		dir  bool
	}
	for {
		h, err := tr.Next()
		if errors.Is(err, io.EOF) {
			break
		}
		if err != nil {
			return err
		}
		parts := strings.Split(filepath.ToSlash(h.Name), "/")
		if len(parts) < 2 {
			continue
		}
		rel := filepath.Join(parts[1:]...)
		if strings.Contains(rel, "..") {
			return errors.New("unsafe archive path")
		}
		if h.FileInfo().IsDir() {
			payload = append(payload, struct {
				name string
				mode fs.FileMode
				data []byte
				dir  bool
			}{rel, h.FileInfo().Mode(), nil, true})
			continue
		}
		data, err := io.ReadAll(tr)
		if err != nil {
			return err
		}
		payload = append(payload, struct {
			name string
			mode fs.FileMode
			data []byte
			dir  bool
		}{rel, h.FileInfo().Mode(), data, false})
		if filepath.ToSlash(rel) == "module-info.json" {
			if err := json.Unmarshal(data, &info); err != nil {
				return err
			}
		}
	}
	if info.Name == "" || info.Version == "" {
		return errors.New("package archive lacks valid module-info.json")
	}
	dest := filepath.Join(projectRoot, "modules", info.Name, info.Version)
	if err := os.RemoveAll(dest); err != nil {
		return err
	}
	for _, p := range payload {
		path := filepath.Join(dest, p.name)
		if p.dir {
			if err := os.MkdirAll(path, 0o755); err != nil {
				return err
			}
			continue
		}
		if err := os.MkdirAll(filepath.Dir(path), 0o755); err != nil {
			return err
		}
		if err := os.WriteFile(path, p.data, p.mode.Perm()); err != nil {
			return err
		}
	}
	l, err := loadLock(projectRoot)
	if err != nil {
		return err
	}
	l.Packages[info.Name] = lockEntry{Version: info.Version, Registry: registryName, SHA256: hash, Package: filepath.Base(abs), Source: abs, InstallKind: map[bool]string{true: "registry-prebuilt", false: "local-file"}[registryName != ""]}
	if err := saveLock(projectRoot, l); err != nil {
		return err
	}
	fmt.Fprintf(out, "Installed %s@%s\n", info.Name, info.Version)
	for _, dep := range info.Dependencies {
		if dep != info.Name {
			if err := install_internal([]string{dep}, out, false); err != nil {
				return fmt.Errorf("install dependency %s: %w", dep, err)
			}
		}
	}
	return nil
}

func removePackage(name string, out io.Writer) error {
	cwd, _ := os.Getwd()
	root, root_err := find_project_root(cwd)
	if root_err != nil {
		root = cwd
	}
	l, err := loadLock(root)
	if err != nil {
		return err
	}
	e, ok := l.Packages[name]
	if !ok {
		return fmt.Errorf("package %q is not installed", name)
	}
	if err := os.RemoveAll(filepath.Join(root, "modules", name, e.Version)); err != nil {
		return err
	}
	delete(l.Packages, name)
	if manifest, manifest_err := load_project(root); manifest_err == nil {
		delete(manifest.Dependencies, name)
		if save_err := save_project(root, manifest); save_err != nil {
			return save_err
		}
	}
	if err := saveLock(root, l); err != nil {
		return err
	}
	fmt.Fprintln(out, "Removed", name)
	return nil
}

func refresh(out io.Writer) error {
	cwd, _ := os.Getwd()
	root, err := find_project_root(cwd)
	if err != nil {
		return err
	}
	locked, err := loadLock(root)
	if err != nil {
		return err
	}
	if len(locked.Packages) == 0 {
		return errors.New("no packages are locked in clyth_package.json")
	}
	regs, err := loadRegistries()
	if err != nil {
		return err
	}
	candidate := locked
	candidate.Packages = make(map[string]lockEntry, len(locked.Packages))
	for name, entry := range locked.Packages {
		candidate.Packages[name] = entry
	}
	var failures []string
	names := make([]string, 0, len(locked.Packages))
	for name := range locked.Packages {
		names = append(names, name)
	}
	sort.Strings(names)
	for _, name := range names {
		entry := locked.Packages[name]
		if entry.Registry == "" || entry.InstallKind == "local-file" {
			continue
		}
		reg, ok := regs.Registries[entry.Registry]
		if !ok {
			failures = append(failures, fmt.Sprintf("%s@%s: registry %q is not configured", name, entry.Version, entry.Registry))
			continue
		}
		idx, index_err := readRegistryIndex(reg)
		if index_err != nil {
			failures = append(failures, fmt.Sprintf("%s@%s: %v", name, entry.Version, index_err))
			continue
		}
		meta, ok := idx.Packages[name][entry.Version]
		if !ok {
			failures = append(failures, fmt.Sprintf("%s@%s: exact locked version not found in registry %q", name, entry.Version, entry.Registry))
			continue
		}
		if entry.SHA256 != "" && meta.SHA256 != "" && entry.SHA256 != meta.SHA256 {
			failures = append(failures, fmt.Sprintf("%s@%s: registry checksum differs from locked checksum", name, entry.Version))
			continue
		}
		if reg.Type == "filesystem" {
			entry.Source = filepath.Join(reg.Location, meta.File)
		} else {
			entry.Source = strings.TrimRight(reg.Location, "/") + "/" + strings.TrimLeft(meta.File, "/")
		}
		entry.Package = meta.File
		entry.LicenseSPDX = meta.LicenseSPDX
		entry.LicenseFile = meta.LicenseFile
		candidate.Packages[name] = entry
	}
	if len(failures) > 0 {
		return fmt.Errorf("refresh failed; clyth_package.json was not modified:\n  %s", strings.Join(failures, "\n  "))
	}
	if err := saveLock(root, candidate); err != nil {
		return err
	}
	fmt.Fprintf(out, "Refreshed %d locked package locations\n", len(candidate.Packages))
	return nil
}

// WriteRegistry builds the filesystem registry index expected by clyth install.
func WriteRegistry(registryDir string) error {
	entries, err := os.ReadDir(registryDir)
	if err != nil {
		return err
	}
	idx := registryIndex{Packages: map[string]map[string]registryPackage{}}
	for _, e := range entries {
		if e.IsDir() || !strings.HasSuffix(e.Name(), ".tar.gz") {
			continue
		}
		path := filepath.Join(registryDir, e.Name())
		f, err := os.Open(path)
		if err != nil {
			return err
		}
		gz, err := gzip.NewReader(f)
		if err != nil {
			f.Close()
			return err
		}
		tr := tar.NewReader(gz)
		var mi moduleInfo
		for {
			h, er := tr.Next()
			if errors.Is(er, io.EOF) {
				break
			}
			if er != nil {
				gz.Close()
				f.Close()
				return er
			}
			if strings.HasSuffix(filepath.ToSlash(h.Name), "/module-info.json") {
				b, er := io.ReadAll(tr)
				if er != nil {
					return er
				}
				if er = json.Unmarshal(b, &mi); er != nil {
					return er
				}
				break
			}
		}
		gz.Close()
		f.Close()
		if mi.Name == "" || mi.Version == "" {
			continue
		}
		hash, err := shaFile(path)
		if err != nil {
			return err
		}
		if idx.Packages[mi.Name] == nil {
			idx.Packages[mi.Name] = map[string]registryPackage{}
		}
		idx.Packages[mi.Name][mi.Version] = registryPackage{File: e.Name(), SHA256: hash}
	}
	return writeJSON(filepath.Join(registryDir, "clyth-package-registry.json"), idx)
}

func registryInfo(args []string, out io.Writer) error {
	if len(args) != 1 {
		return errors.New("usage: clyth registry-info <registry-name>")
	}
	regs, err := loadRegistries()
	if err != nil {
		return err
	}
	reg, ok := regs.Registries[args[0]]
	if !ok {
		return fmt.Errorf("registry %q does not exist", args[0])
	}
	fmt.Fprintf(out, "%s\ntype: %s\nlocation: %s\n", args[0], reg.Type, reg.Location)
	return nil
}

func readRegistryIndex(reg registry) (registryIndex, error) {
	var idx registryIndex
	if reg.Type == "filesystem" {
		return idx, readJSON(filepath.Join(reg.Location, "clyth-package-registry.json"), &idx)
	}
	url := strings.TrimRight(reg.Location, "/") + "/clyth-package-registry.json"
	resp, err := http.Get(url)
	if err != nil {
		return idx, err
	}
	defer resp.Body.Close()
	if resp.StatusCode != http.StatusOK {
		return idx, fmt.Errorf("registry returned %s", resp.Status)
	}
	err = json.NewDecoder(resp.Body).Decode(&idx)
	return idx, err
}

func updatePackages(args []string, out io.Writer) error {
	build_from_source := false
	var requested string
	for _, arg := range args {
		if arg == "--build-from-source" {
			build_from_source = true
		} else if requested == "" {
			requested = arg
		} else {
			return errors.New("usage: clyth update [module] [--build-from-source]")
		}
	}
	if requested != "" {
		install_args := []string{requested}
		if build_from_source {
			install_args = append(install_args, "--build-from-source")
		}
		return install(install_args, out)
	}
	cwd, _ := os.Getwd()
	root, err := find_project_root(cwd)
	if err != nil {
		return err
	}
	lock, err := loadLock(root)
	if err != nil {
		return err
	}
	names := make([]string, 0, len(lock.Packages))
	for n := range lock.Packages {
		names = append(names, n)
	}
	sort.Strings(names)
	for _, name := range names {
		install_args := []string{name}
		if build_from_source {
			install_args = append(install_args, "--build-from-source")
		}
		if err := install(install_args, out); err != nil {
			return fmt.Errorf("update %s: %w", name, err)
		}
	}
	return nil
}

func buildFromSource(project_root, name, version, registry_name string, meta registryPackage, out io.Writer) error {
	if meta.SourceURL == "" || meta.SourceRevision == "" {
		return fmt.Errorf("%s@%s does not publish a pinned source URL and revision", name, version)
	}
	temp_dir, err := os.MkdirTemp("", "clyth-source-build-*")
	if err != nil {
		return err
	}
	defer os.RemoveAll(temp_dir)
	source_dir := filepath.Join(temp_dir, name)
	clone := exec.Command("git", "clone", "--quiet", meta.SourceURL, source_dir)
	if output, err := clone.CombinedOutput(); err != nil {
		return fmt.Errorf("clone source: %w: %s", err, output)
	}
	checkout := exec.Command("git", "-C", source_dir, "checkout", "--quiet", meta.SourceRevision)
	if output, err := checkout.CombinedOutput(); err != nil {
		return fmt.Errorf("checkout source revision: %w: %s", err, output)
	}
	archive, err := Package(source_dir)
	if err != nil {
		return fmt.Errorf("package locally-built source: %w", err)
	}
	if err := installArchive(project_root, archive, registry_name, out); err != nil {
		return err
	}
	lock, err := loadLock(project_root)
	if err != nil {
		return err
	}
	entry := lock.Packages[name]
	entry.InstallKind = "registry-source-build"
	entry.Source = meta.SourceURL
	entry.SourceRevision = meta.SourceRevision
	entry.LicenseSPDX = meta.LicenseSPDX
	entry.LicenseFile = meta.LicenseFile
	lock.Packages[name] = entry
	return saveLock(project_root, lock)
}

func record_project_dependency(root, name, version string) error {
	manifest, err := load_project(root)
	if err != nil {
		return nil
	}
	if manifest.Dependencies == nil {
		manifest.Dependencies = map[string]string{}
	}
	manifest.Dependencies[name] = version
	return save_project(root, manifest)
}

func install_local_archive(project_root, archive string, out io.Writer) error {
	before, _ := loadLock(project_root)
	if err := installArchive(project_root, archive, "", out); err != nil {
		return err
	}
	after, err := loadLock(project_root)
	if err != nil {
		return err
	}
	for name, entry := range after.Packages {
		if _, existed := before.Packages[name]; !existed || entry.InstallKind == "local-file" {
			return record_project_dependency(project_root, name, entry.Version)
		}
	}
	return nil
}
