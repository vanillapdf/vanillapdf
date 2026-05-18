#ifndef _VANILLAPDF_FUZZER_H
#define _VANILLAPDF_FUZZER_H

#include "vanillapdf/c_vanillapdf_api.h"

/**
 * Initialize the vanillapdf library for fuzzing.
 *
 * Disables logging to avoid noise during fuzzing runs.
 * Call once from LLVMFuzzerInitialize or at the start of each target.
 */
inline void fuzzer_init() {
    Logging_SetSeverity(LoggingSeverity_Off);
}

#endif /* _VANILLAPDF_FUZZER_H */
