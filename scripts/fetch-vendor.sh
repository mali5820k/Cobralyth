#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
LOCK="${CLYTH_VENDOR_LOCK:-$ROOT/third-party.lock}"
CACHE="${CLYTH_VENDOR_CACHE:-$ROOT/module-src/vendor}"
NAME="${1:-}"

[[ -n "$NAME" ]] || {
  printf 'usage: %s <vendor-name>\n' "$0" >&2
  exit 2
}
[[ -f "$LOCK" ]] || { printf 'error: vendor lock file not found: %s\n' "$LOCK" >&2; exit 1; }
command -v git >/dev/null 2>&1 || { printf 'error: git is required to fetch %s\n' "$NAME" >&2; exit 1; }

line="$(awk -F'|' -v name="$NAME" '$1 == name { print; exit }' "$LOCK")"
[[ -n "$line" ]] || { printf 'error: unknown vendor dependency: %s\n' "$NAME" >&2; exit 1; }
IFS='|' read -r name url commit version license <<<"$line"

dest="$CACHE/$name/$commit"
marker="$dest/.clyth-vendor-commit"
if [[ -d "$dest" && -f "$marker" && "$(cat "$marker")" == "$commit" ]]; then
  printf '%s\n' "$dest"
  exit 0
fi

rm -rf "$dest"
mkdir -p "$(dirname "$dest")"
tmp="$dest.tmp.$$"
rm -rf "$tmp"
mkdir -p "$tmp"
git -C "$tmp" init -q
git -C "$tmp" remote add origin "$url"
if ! git -C "$tmp" fetch -q --depth 1 origin "$commit"; then
  rm -rf "$tmp"
  printf 'error: failed to fetch pinned %s commit %s\n' "$name" "$commit" >&2
  exit 1
fi
git -C "$tmp" checkout -q --detach FETCH_HEAD
actual="$(git -C "$tmp" rev-parse HEAD)"
[[ "$actual" == "$commit" ]] || {
  rm -rf "$tmp"
  printf 'error: %s resolved to %s, expected %s\n' "$name" "$actual" "$commit" >&2
  exit 1
}
printf '%s\n' "$commit" > "$tmp/.clyth-vendor-commit"
mv "$tmp" "$dest"
printf '%s\n' "$dest"
