# Build and Testing Lessons

- Full test suite (`ctest`) takes ~40 minutes in Debug. Run the unit test executable directly for quick verification (~600ms): `build/windows-x64-msvc-17/src/vanillapdf.unittest/Debug/vanillapdf.unittest.exe`
- Never run the full test suite in Debug — it is extremely slow. Always use `-R "FilterPattern"` to target relevant tests. If the full suite is truly needed, run it in Release.
- Always run benchmarks in Release mode (`--build-config Release` / `--config Release`). Debug builds produce meaningless timings.
