# Shared bootstrap license files

The `0.5.0-compiler` seed compiler and the `0.5.1-compiler` bootstrap compiler use the same static project and external-library license manifests. Each compiler directory links directly to the files in this directory so that bootstrap licensing has one maintained source.

The final `0.6.0-compiler` is intentionally excluded. Its external-library manifest belongs to the final compiler and is generated from the modules included in its distribution.
