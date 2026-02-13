# Exception Usage Lessons

- `NotSupportedException` is only for legitimately valid input where functionality is not implemented (e.g., "compiled without OpenSSL", "TIFF predictor not supported"). Do not use it for unknown/unexpected values.
- Unknown values read from PDF files → `ParseException`
- Invalid user-supplied values or internal invariants → `InvalidParameterException`
- Files in `semantics/` need `#include "syntax/exceptions/syntax_exceptions.h"` and the `syntax::` prefix to use `ParseException`, unless `using namespace syntax;` is in scope.
- `InvalidParameterException` and other base exception classes from `utils/exceptions.h` are available everywhere via `precompiled.h` — no extra include needed.
- When deprecating a C API constant (e.g., `VANILLAPDF_DEPRECATED` on an error code), update all internal references to that constant — `c_errors.cpp` error name lookup, `utils.c` integration test smoke tests, and any other usage. Clang `-Werror,-Wdeprecated-declarations` will fail the CI build otherwise.
