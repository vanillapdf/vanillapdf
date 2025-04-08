#include "precompiled.h"
#include "utils/log.h"

#include "vanillapdf/utils/c_logging.h"
#include "implementation/c_helper.h"

#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <mutex>

template<typename Mutex>
class custom_callback_sink : public spdlog::sinks::base_sink <Mutex> {
public:
    custom_callback_sink(
        Sink_Log_Function sink_callback,
        Sink_Flush_Function sink_flush,
        void* user_data) : m_sink_callback(sink_callback), m_sink_flush(sink_flush), m_user_data(user_data) {
    }

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        LoggingSeverity msg_severity_level = LoggingSeverity_Undefined;
        //auto msg_time = std::chrono::system_clock::to_time_t(msg.time);

        switch (msg.level)
        {
        case spdlog::level::trace:
            msg_severity_level = LoggingSeverity_Trace; break;
        case spdlog::level::debug:
            msg_severity_level = LoggingSeverity_Debug; break;
        case spdlog::level::info:
            msg_severity_level = LoggingSeverity_Info; break;
        case spdlog::level::warn:
            msg_severity_level = LoggingSeverity_Warning; break;
        case spdlog::level::err:
            msg_severity_level = LoggingSeverity_Error; break;
        case spdlog::level::critical:
            msg_severity_level = LoggingSeverity_Critical; break;
        case spdlog::level::off:
        case spdlog::level::n_levels:
            msg_severity_level = LoggingSeverity_Off; break;
        }

        // TODO: sourceline, sourcefile, sourcefunc

        m_sink_callback(m_user_data, msg_severity_level, msg.payload.data(), msg.payload.size());
    }

    void flush_() override
    {
        m_sink_flush(m_user_data);
    }

private:
    Sink_Log_Function m_sink_callback = nullptr;
    Sink_Flush_Function m_sink_flush = nullptr;
    void* m_user_data = nullptr;;
};

using custom_callback_sink_mt = custom_callback_sink<std::mutex>;
using custom_callback_sink_st = custom_callback_sink<spdlog::details::null_mutex>;

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION Logging_SetCallbackLogger(
    Sink_Log_Function sink_callback,
    Sink_Flush_Function sink_flush_callback,
    void* user_data)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(sink_callback);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(sink_flush_callback);

    try
    {
        auto callback_logger = spdlog::create<custom_callback_sink_mt>("callback_logger", sink_callback, sink_flush_callback, user_data);
        spdlog::set_default_logger(callback_logger);

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_SetRotatingFileLogger(string_type filename, integer_type max_file_size, integer_type max_files)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);

    try
    {
        auto file_logger = spdlog::rotating_logger_mt("file_logger", filename, max_file_size, max_files);
        spdlog::set_default_logger(file_logger);

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_SetPattern(string_type pattern)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(pattern);

    try
    {
        spdlog::set_pattern(pattern);

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_GetSeverity(LoggingSeverity* level)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(level);

    try
    {
        auto result = spdlog::get_level();
        switch (result)
        {
        case spdlog::level::trace:
            *level = LoggingSeverity_Trace; break;
        case spdlog::level::debug:
            *level = LoggingSeverity_Debug; break;
        case spdlog::level::info:
            *level = LoggingSeverity_Info; break;
        case spdlog::level::warn:
            *level = LoggingSeverity_Warning; break;
        case spdlog::level::err:
            *level = LoggingSeverity_Error; break;
        case spdlog::level::critical:
            *level = LoggingSeverity_Critical; break;
        case spdlog::level::off:
            *level = LoggingSeverity_Off; break;
        default:
            return VANILLAPDF_ERROR_GENERAL;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_SetSeverity(LoggingSeverity level)
{
    try
    {
        switch (level)
        {
        case LoggingSeverity_Trace:
            spdlog::set_level(spdlog::level::trace); break;
        case LoggingSeverity_Debug:
            spdlog::set_level(spdlog::level::debug); break;
        case LoggingSeverity_Info:
            spdlog::set_level(spdlog::level::info); break;
        case LoggingSeverity_Warning:
            spdlog::set_level(spdlog::level::warn); break;
        case LoggingSeverity_Error:
            spdlog::set_level(spdlog::level::err); break;
        case LoggingSeverity_Critical:
            spdlog::set_level(spdlog::level::critical); break;
        case LoggingSeverity_Off:
            spdlog::set_level(spdlog::level::off); break;
        default:
            return VANILLAPDF_ERROR_GENERAL;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_Shutdown()
{
    try
    {
        spdlog::shutdown();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}
