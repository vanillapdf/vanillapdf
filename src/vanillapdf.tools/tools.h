#ifndef _VANILLAPDF_TOOLS_H
#define _VANILLAPDF_TOOLS_H

#include "vanillapdf/c_vanillapdf_api.h"

#include "handle_guard.h"

#include <CLI/CLI.hpp>

#include <memory>
#include <string>
#include <vector>

#include <stdio.h>
#include <assert.h>

#if defined(DEBUG) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
#endif /* DEBUG && COMPILER_MICROSOFT_VISUAL_STUDIO */

constexpr int VANILLAPDF_TOOLS_ERROR_SUCCESS = 0;
constexpr int VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS = 1;
constexpr int VANILLAPDF_TOOLS_ERROR_FAILURE = 255;

// Each command registers its CLI11 subcommand with the application. The
// subcommand callback runs the command and stores its result in exit_code,
// which main returns once parsing has finished.
void register_merge(CLI::App& app, int& exit_code);
void register_filter(CLI::App& app, int& exit_code);
void register_extract(CLI::App& app, int& exit_code);
void register_extract_object(CLI::App& app, int& exit_code);
void register_sign(CLI::App& app, int& exit_code);
void register_sign_custom(CLI::App& app, int& exit_code);
void register_encrypt(CLI::App& app, int& exit_code);
void register_decrypt(CLI::App& app, int& exit_code);
void register_write_custom(CLI::App& app, int& exit_code);
void register_read(CLI::App& app, int& exit_code);
void register_resave(CLI::App& app, int& exit_code);
void register_remove_page(CLI::App& app, int& exit_code);
void register_verify(CLI::App& app, int& exit_code);
void register_validate(CLI::App& app, int& exit_code);
void register_generate(CLI::App& app, int& exit_code);

// Some parameters to functions are unused
#define UNUSED(x) (void)(x)

// Every handle is owned by a HandleGuard, so returning early here releases
// everything acquired so far - no cleanup block is needed at the call site
#define RETURN_ERROR_IF_NOT_SUCCESS(fn) \
do { \
    error_type __result__ = (fn); \
    if (VANILLAPDF_ERROR_SUCCESS != __result__) \
    { \
        printf("Function call \"%s\" has failed with result %u { %s:%d }\n", \
        #fn, __result__, __FILE__, __LINE__); \
        assert(!"Operation failed"); \
        return VANILLAPDF_TOOLS_ERROR_FAILURE; \
    } \
} while(0)

#endif /* _VANILLAPDF_TOOLS_H */
