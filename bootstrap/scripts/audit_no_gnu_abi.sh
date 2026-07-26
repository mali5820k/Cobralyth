#!/usr/bin/env bash
# Inspect ELF files in a compiler distribution and reject GNU/Linux ABI or
# libstdc++ dependencies. This protects the Zig + musl + libc++ boundary.
set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
DIST="${1:-$PROJECT_ROOT/0.6.0-compiler/dist}"

[[ -d "$DIST" ]] || {
  printf 'missing distribution: %s\n' "$DIST" >&2
  exit 1
}
command -v file >/dev/null 2>&1 || { printf 'missing required tool: file\n' >&2; exit 1; }
command -v readelf >/dev/null 2>&1 || { printf 'missing required tool: readelf\n' >&2; exit 1; }

status=0
elf_count=0
while IFS= read -r -d '' candidate; do
  file "$candidate" | grep -q ELF || continue
  elf_count=$((elf_count + 1))

  audit_text="$({
    readelf -d "$candidate" 2>/dev/null || true
    readelf --version-info "$candidate" 2>/dev/null || true
    readelf -Ws "$candidate" 2>/dev/null || true
    readelf -l "$candidate" 2>/dev/null || true
  } | LC_ALL=C cat)"

  if grep -Eq 'libstdc\+\+|libgcc_s|GLIBCXX_|CXXABI_|GLIBC_[0-9]|ld-linux|linux-gnu' <<<"$audit_text"; then
    printf 'GNU ABI dependency or symbol found: %s\n' "$candidate" >&2
    grep -E 'libstdc\+\+|libgcc_s|GLIBCXX_|CXXABI_|GLIBC_[0-9]|ld-linux|linux-gnu' \
      <<<"$audit_text" | head -20 >&2 || true
    status=1
  fi
done < <(find "$DIST" -type f -print0)

[[ "$elf_count" -gt 0 ]] || {
  printf 'GNU ABI audit found no ELF files under: %s\n' "$DIST" >&2
  exit 1
}

if [[ "$status" -ne 0 ]]; then
  printf 'GNU ABI audit FAILED: %s\n' "$DIST" >&2
  exit "$status"
fi
printf 'GNU ABI audit passed (%d ELF files): %s\n' "$elf_count" "$DIST"
