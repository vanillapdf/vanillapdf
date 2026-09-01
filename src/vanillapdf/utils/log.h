#ifndef _LOG_H
#define _LOG_H

#include "utils/exceptions.h"

#include <spdlog/spdlog.h>

#define LOG_ERROR_AND_THROW_GENERAL(...) \
    do { \
        auto log_error_and_throw_message = fmt::format(__VA_ARGS__); \
        spdlog::error(log_error_and_throw_message); \
        throw GeneralException(log_error_and_throw_message); \
    } while (0)

#define LOG_ERROR_AND_THROW(ExceptionType, ...) \
    do { \
        auto log_error_and_throw_message = fmt::format(__VA_ARGS__); \
        spdlog::error(log_error_and_throw_message); \
        throw ExceptionType(log_error_and_throw_message); \
    } while (0)

#endif /* _LOG_H */