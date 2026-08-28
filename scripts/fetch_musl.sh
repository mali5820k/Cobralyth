#!/usr/bin/env bash
set -Eeuo pipefail

MODULE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
VERSION="1.2.6"
ARCHIVE="musl-${VERSION}.tar.gz"
URL="https://musl.libc.org/releases/${ARCHIVE}"
SHA256="d585fd3b613c66151fc3249e8ed44f77020cb5e6c1e635a616d3f9f82460512a"
DOWNLOAD_DIR="${MUSL_DOWNLOAD_DIR:-$MODULE_DIR/.downloads}"
SOURCE_PARENT="${MUSL_SOURCE_PARENT:-$MODULE_DIR/third_party}"
SOURCE_DIR="$SOURCE_PARENT/musl"
ARCHIVE_PATH="$DOWNLOAD_DIR/$ARCHIVE"

mkdir -p "$DOWNLOAD_DIR" "$SOURCE_PARENT"

if [[ ! -f "$ARCHIVE_PATH" ]]; then
  if command -v curl >/dev/null 2>&1; then
    curl --fail --location --retry 3 --output "$ARCHIVE_PATH" "$URL"
  elif command -v wget >/dev/null 2>&1; then
    wget --tries=3 --output-document="$ARCHIVE_PATH" "$URL"
  else
    printf 'error: curl or wget is required to download musl %s\n' "$VERSION" >&2
    exit 1
  fi
fi

printf '%s  %s\n' "$SHA256" "$ARCHIVE_PATH" | sha256sum --check --status || {
  printf 'error: SHA-256 verification failed for %s\n' "$ARCHIVE_PATH" >&2
  exit 1
}

rm -rf "$SOURCE_DIR"
mkdir -p "$SOURCE_DIR"
tar -xzf "$ARCHIVE_PATH" --strip-components=1 -C "$SOURCE_DIR"

[[ -f "$SOURCE_DIR/VERSION" ]] || { printf 'error: extracted musl source has no VERSION file\n' >&2; exit 1; }
[[ "$(tr -d '[:space:]' < "$SOURCE_DIR/VERSION")" == "$VERSION" ]] || {
  printf 'error: extracted musl version does not match %s\n' "$VERSION" >&2
  exit 1
}
[[ -f "$SOURCE_DIR/COPYRIGHT" ]] || { printf 'error: extracted musl source has no COPYRIGHT file\n' >&2; exit 1; }

printf 'Fetched and verified musl %s: %s\n' "$VERSION" "$SOURCE_DIR"
