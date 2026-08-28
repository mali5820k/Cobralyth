#!/usr/bin/env sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD="$ROOT/lsp/.build"
BIN="$BUILD/bin"
DIST="${CLYTH_LSP_DIST:-$ROOT/dist/lsp}"
rm -rf "$BUILD/packages" "$DIST"
mkdir -p "$BUILD/packages" "$DIST"
"$ROOT/scripts/build-lsp.sh"

make_unix_package() {
    editor=$1
    source_dir=$2
    package_dir="$BUILD/packages/$editor-clyth-lsp"
    mkdir -p "$package_dir/editor" "$package_dir/bin"
    cp -R "$source_dir"/. "$package_dir/editor/"
    cp "$ROOT/LICENSE" "$package_dir/LICENSE"
    cp "$source_dir/THIRD_PARTY_NOTICES.md" "$package_dir/THIRD_PARTY_NOTICES.md"
    cp -R "$BIN/linux-x64" "$BIN/linux-arm64" "$BIN/darwin-x64" "$BIN/darwin-arm64" "$package_dir/bin/"
    cat > "$package_dir/install-lsp.sh" <<'EOS'
#!/usr/bin/env sh
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
os=$(uname -s | tr '[:upper:]' '[:lower:]')
arch=$(uname -m)
case "$os" in linux) os=linux ;; darwin) os=darwin ;; *) echo "Unsupported OS: $os" >&2; exit 1 ;; esac
case "$arch" in x86_64|amd64) arch=x64 ;; aarch64|arm64) arch=arm64 ;; *) echo "Unsupported architecture: $arch" >&2; exit 1 ;; esac
src="$HERE/bin/$os-$arch/clyth-lsp"
dest="${XDG_BIN_HOME:-$HOME/.local/bin}"
mkdir -p "$dest"
cp "$src" "$dest/clyth-lsp"
chmod +x "$dest/clyth-lsp"
printf 'Installed clyth-lsp to %s/clyth-lsp\n' "$dest"
case ":$PATH:" in *":$dest:"*) ;; *) printf 'Add %s to PATH if it is not already present.\n' "$dest" ;; esac
EOS
    chmod +x "$package_dir/install-lsp.sh"
}

# Fresh
make_unix_package fresh "$ROOT/lsp/editors/fresh"
FRESH="$BUILD/packages/fresh-clyth-lsp"
cat > "$FRESH/install.sh" <<'EOF_FRESH'
#!/usr/bin/env sh
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
"$HERE/install-lsp.sh"
config="${XDG_CONFIG_HOME:-$HOME/.config}/fresh/grammars/clyth"
mkdir -p "$(dirname "$config")"
rm -rf "$config"
cp -R "$HERE/editor" "$config"
printf 'Installed Clyth Fresh language pack to %s\n' "$config"
EOF_FRESH
chmod +x "$FRESH/install.sh"
tar -C "$BUILD/packages" -czf "$DIST/fresh-clyth-lsp.tar.gz" fresh-clyth-lsp

# Vim
make_unix_package vim "$ROOT/lsp/editors/vim"
VIM="$BUILD/packages/vim-clyth-lsp"
cat > "$VIM/install.sh" <<'EOF_VIM'
#!/usr/bin/env sh
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
"$HERE/install-lsp.sh"
dest="${VIM_CONFIG_HOME:-$HOME/.vim}"
mkdir -p "$dest/ftdetect" "$dest/syntax" "$dest/plugin"
cp -R "$HERE/editor/ftdetect/." "$dest/ftdetect/"
cp -R "$HERE/editor/syntax/." "$dest/syntax/"
cp -R "$HERE/editor/plugin/." "$dest/plugin/"
printf 'Installed Clyth Vim support to %s\n' "$dest"
EOF_VIM
chmod +x "$VIM/install.sh"
tar -C "$BUILD/packages" -czf "$DIST/vim-clyth-lsp.tar.gz" vim-clyth-lsp

# Neovim
make_unix_package nvim "$ROOT/lsp/editors/neovim"
NVIM="$BUILD/packages/nvim-clyth-lsp"
mkdir -p "$NVIM/editor/syntax"
cp "$ROOT/lsp/editors/vim/syntax/clyth.vim" "$NVIM/editor/syntax/clyth.vim"
cat > "$NVIM/install.sh" <<'EOF_NVIM'
#!/usr/bin/env sh
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
"$HERE/install-lsp.sh"
dest="${XDG_CONFIG_HOME:-$HOME/.config}/nvim"
mkdir -p "$dest/ftdetect" "$dest/lsp" "$dest/syntax"
cp -R "$HERE/editor/ftdetect/." "$dest/ftdetect/"
cp -R "$HERE/editor/lsp/." "$dest/lsp/"
cp -R "$HERE/editor/syntax/." "$dest/syntax/"
printf 'Installed Clyth Neovim support to %s\n' "$dest"
printf "Enable it with: vim.lsp.enable('clyth')\n"
EOF_NVIM
chmod +x "$NVIM/install.sh"
tar -C "$BUILD/packages" -czf "$DIST/nvim-clyth-lsp.tar.gz" nvim-clyth-lsp

# Notepad++: Windows x64 package with syntax/LSP configuration and server binary.
NPP="$BUILD/packages/notepadpp-clyth-lsp"
mkdir -p "$NPP/bin"
cp -R "$ROOT/lsp/editors/notepadpp"/. "$NPP/"
cp "$BIN/win32-x64/clyth-lsp.exe" "$NPP/bin/clyth-lsp.exe"
cp "$ROOT/LICENSE" "$NPP/LICENSE"
python3 - "$NPP" "$DIST/notepadpp-clyth-lsp.zip" <<'PY_NPP'
import pathlib, sys, zipfile
root = pathlib.Path(sys.argv[1])
out = pathlib.Path(sys.argv[2])
with zipfile.ZipFile(out, 'w', zipfile.ZIP_DEFLATED) as archive:
    for path in root.rglob('*'):
        if path.is_file():
            archive.write(path, pathlib.Path(root.name) / path.relative_to(root))
PY_NPP

# VS Code: self-contained VSIX, no npm/vsce dependency.
VS="$BUILD/packages/vscode-clyth-lsp"
mkdir -p "$VS/extension/bin"
cp "$ROOT/lsp/editors/vscode/package.json" "$ROOT/lsp/editors/vscode/extension.js" "$ROOT/lsp/editors/vscode/language-configuration.json" "$VS/extension/"
cp "$ROOT/LICENSE" "$VS/extension/LICENSE"
cp "$ROOT/lsp/editors/vscode/THIRD_PARTY_NOTICES.md" "$VS/extension/THIRD_PARTY_NOTICES.md"
cp -R "$ROOT/lsp/editors/vscode/syntaxes" "$VS/extension/"
cp -R "$BIN/linux-x64" "$BIN/linux-arm64" "$BIN/darwin-x64" "$BIN/darwin-arm64" "$BIN/win32-x64" "$VS/extension/bin/"
python3 - "$VS" "$DIST/vscode-clyth-lsp.VSIX" <<'PY'
import json, pathlib, sys, zipfile
root=pathlib.Path(sys.argv[1]); out=pathlib.Path(sys.argv[2])
pkg=json.loads((root/'extension/package.json').read_text())
(root/'extension.vsixmanifest').write_text(f'''<?xml version="1.0" encoding="utf-8"?>
<PackageManifest Version="2.0.0" xmlns="http://schemas.microsoft.com/vsx-schema/2011">
  <Metadata>
    <Identity Language="en-US" Id="{pkg['name']}" Version="{pkg['version']}" Publisher="{pkg['publisher']}" />
    <DisplayName>{pkg['displayName']}</DisplayName>
    <Description xml:space="preserve">{pkg['description']}</Description>
    <Tags>clyth,lsp,programming language</Tags>
  </Metadata>
  <Installation><InstallationTarget Id="Microsoft.VisualStudio.Code" Version="[1.95.0,)" /></Installation>
  <Dependencies />
  <Assets><Asset Type="Microsoft.VisualStudio.Code.Manifest" Path="extension/package.json" Addressable="true" /></Assets>
</PackageManifest>''')
(root/'[Content_Types].xml').write_text('''<?xml version="1.0" encoding="utf-8"?>
<Types xmlns="http://schemas.openxmlformats.org/package/2006/content-types">
  <Default Extension="json" ContentType="application/json" />
  <Default Extension="js" ContentType="application/javascript" />
  <Default Extension="xml" ContentType="text/xml" />
  <Default Extension="" ContentType="application/octet-stream" />
  <Override PartName="/extension.vsixmanifest" ContentType="text/xml" />
</Types>''')
with zipfile.ZipFile(out,'w',zipfile.ZIP_DEFLATED) as z:
    for p in root.rglob('*'):
        if p.is_file(): z.write(p,p.relative_to(root).as_posix())
PY

printf '\nLSP/editor release artifacts:\n'
for f in "$DIST"/*; do printf '  %s\n' "$f"; done
