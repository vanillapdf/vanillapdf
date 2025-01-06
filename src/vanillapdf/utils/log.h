#ifndef _LOG_H
#define _LOG_H

#include "utils/exceptions.h"

#include <spdlog/spdlog.h>

#define LOG_ERROR_AND_THROW_GENERAL(...) \
	spdlog::error(__VA_ARGS__); \
	throw GeneralException(fmt::format(__VA_ARGS__))

#endif /* _LOG_H */
