# Legacy / Untracked Samples

These files were previously present in `sample-clyth-programs/`, but they are not part of the current release regression suite.

To promote one into the supported suite:

1. Confirm it represents current Clyth syntax and runtime behavior.
2. Rename it using `NN_0_X_feature_name.clyth`.
3. Add it to `run_language_samples.sh` with a stable expected output marker.
4. Document it in `REGRESSION_SUITE.md`.
