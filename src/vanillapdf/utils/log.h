#ifndef _LOG_H
#define _LOG_H

#include "utils/exceptions.h"

#include <spdlog/spdlog.h>

// The message is formatted once: arguments with side effects, such as
// CryptoUtils::GetLastOpensslError() popping the error queue, must not
// be evaluated separately for the log and for the exception.
#define LOG_ERROR_AND_THROW_GENERAL(...) \
    do { \
        auto vanillapdf_error_message = fmt::format(__VA_ARGS__); \
        spdlog::error(vanillapdf_error_message); \
        throw GeneralException(vanillapdf_error_message); \
    } while (false)

#define LOG_ERROR_AND_THROW(ExceptionType, ...) \
    do { \
        auto vanillapdf_error_message = fmt::format(__VA_ARGS__); \
        spdlog::error(vanillapdf_error_message); \
        throw ExceptionType(vanillapdf_error_message); \
    } while (false)

#endif /* _LOG_H */
