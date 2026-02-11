# Build and Testing Lessons

- Full test suite (`ctest`) takes ~40 minutes in Debug. Run the unit test executable directly for quick verification (~600ms): `build/windows-x64-msvc-17/src/vanillapdf.unittest/Debug/vanillapdf.unittest.exe`
- Never run the full test suite in Debug — it is extremely slow. Always use `-R "FilterPattern"` to target relevant tests. If the full suite is truly needed, run it in Release.
- Always run benchmarks in Release mode (`--build-config Release` / `--config Release`). Debug builds produce meaningless timings.
- Clang with `-Werror,-Wswitch` requires all enum values handled in switch statements (including `Undefined`). MSVC does not warn. Always add a `default:` case when switching on enums.

## Performance Improvement Workflow

Every performance change must follow this workflow to justify the change with data:

1. **Add benchmark** — write a Google Benchmark test covering the hot path before making any changes
2. **Measure baseline** — build and run the benchmark in Release mode, record results
3. **Commit baseline** — commit the benchmark with baseline results in a comment
4. **Implement the change** — modify the code
5. **Measure impact** — run the same benchmark in Release mode, record new results
6. **Commit with comparison** — commit the change with both before/after results in the benchmark comment
