package packagecmd

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sort"
)

// ProjectLicense records both the machine-readable SPDX identifier and the
// license text that must be copied into a distributable.
type ProjectLicense struct {
	SPDX string `json:"spdx"`
	File string `json:"file"`
}

// ProjectManifest is the human-authored declaration of project intent.
type ProjectManifest struct {
	ProjectName  string            `json:"project-name"`
	Version      string            `json:"version"`
	ProjectType  string            `json:"project-type"`
	License      ProjectLicense    `json:"license"`
	Entry        string            `json:"entry"`
	Dependencies map[string]string `json:"dependencies"`
}

func find_project_root(start string) (string, error) {
	current, _ := filepath.Abs(start)
	for {
		if _, err := os.Stat(filepath.Join(current, "clyth_project.json")); err == nil {
			return current, nil
		}
		parent := filepath.Dir(current)
		if parent == current {
			return "", errors.New("clyth_project.json not found")
		}
		current = parent
	}
}

func init_project(args []string, out io.Writer) error {
	if len(args) != 1 {
		return errors.New("usage: clyth init <project-name>")
	}
	root, err := filepath.Abs(args[0])
	if err != nil {
		return err
	}
	if err := os.MkdirAll(filepath.Join(root, "src"), 0o755); err != nil {
		return err
	}
	manifest := ProjectManifest{ProjectName: filepath.Base(root), Version: "1.0.0", ProjectType: "program", License: ProjectLicense{SPDX: "UNLICENSED", File: "LICENSE.md"}, Entry: "src/main.clyth", Dependencies: map[string]string{}}
	if err := writeJSON(filepath.Join(root, "clyth_project.json"), manifest); err != nil {
		return err
	}
	if err := os.WriteFile(filepath.Join(root, "src", "main.clyth"), []byte("int32 main() { return 0 }\n"), 0o644); err != nil {
		return err
	}
	if err := os.WriteFile(filepath.Join(root, "LICENSE.md"), []byte("Replace this file with the project license text.\n"), 0o644); err != nil {
		return err
	}
	fmt.Fprintln(out, "Initialized Clyth V1.0.0 project", manifest.ProjectName)
	return nil
}

func load_project(root string) (ProjectManifest, error) {
	var p ProjectManifest
	err := readJSON(filepath.Join(root, "clyth_project.json"), &p)
	return p, err
}
func save_project(root string, p ProjectManifest) error {
	return writeJSON(filepath.Join(root, "clyth_project.json"), p)
}

func list_packages(out io.Writer) error {
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
	for _, n := range names {
		e := lock.Packages[n]
		fmt.Fprintf(out, "%s\t%s\t%s\t%s\n", n, e.Version, e.Registry, e.InstallKind)
	}
	return nil
}

func package_info(args []string, out io.Writer) error {
	if len(args) != 1 {
		return errors.New("usage: clyth info <module>")
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
	e, ok := lock.Packages[args[0]]
	if !ok {
		return fmt.Errorf("package %q is not locked", args[0])
	}
	b, _ := json.MarshalIndent(e, "", "  ")
	fmt.Fprintln(out, string(b))
	return nil
}

func debug_project(out io.Writer) error {
	cwd, _ := os.Getwd()
	fmt.Fprintln(out, "Clyth V1.0.0 bootstrap diagnostics")
	fmt.Fprintln(out, "clyth-home:", clythHome())
	if root, err := find_project_root(cwd); err == nil {
		fmt.Fprintln(out, "project:", root)
		if p, e := load_project(root); e == nil {
			fmt.Fprintf(out, "project-name: %s\nproject-type: %s\nversion: %s\nlicense: %s (%s)\n", p.ProjectName, p.ProjectType, p.Version, p.License.SPDX, p.License.File)
		}
	} else {
		fmt.Fprintln(out, "project: none")
	}
	regs, err := loadRegistries()
	if err != nil {
		return err
	}
	fmt.Fprintln(out, "registries:", len(regs.Registries))
	return nil
}
