#ifndef _LOG_H
#define _LOG_H

#include "spdlog/spdlog.h"
//
//#include "utils/exceptions.h"
//
//#include <ostream>
//#include <memory>
//
//namespace vanillapdf {
//
//enum class Severity {
//	Undefined = 0,
//	Debug,
//	Info,
//	Warning,
//	Error,
//	Fatal
//};
//
//template <typename CharT, typename TraitsT>
//std::basic_ostream<CharT, TraitsT>& operator<< (std::basic_ostream<CharT, TraitsT>& strm, const Severity level) {
//	switch (level) {
//		case Severity::Debug:
//			strm << "Debug"; break;
//		case Severity::Info:
//			strm << "Info"; break;
//		case Severity::Warning:
//			strm << "Warning"; break;
//		case Severity::Error:
//			strm << "Error"; break;
//		case Severity::Fatal:
//			strm << "Fatal"; break;
//		default:
//			throw GeneralException("Invalid logging severity specified: " + std::to_string((int) level));
//	}
//
//	return strm;
//}
//
//class IOutputWriter {
//public:
//	virtual std::ostream& GetOutputStream() const = 0;
//	virtual ~IOutputWriter() = 0;
//};
//
//template <typename T>
//std::unique_ptr<IOutputWriter> operator<<(std::unique_ptr<IOutputWriter> strm, const T& value) {
//	auto& output_stream = strm->GetOutputStream();
//	output_stream << value;
//	return strm;
//}
//
//class ILogger {
//public:
//	virtual std::unique_ptr<IOutputWriter> GetScopedWriter(Severity severity, int line, const char * scope, const char * file, const char * function) = 0;
//	virtual std::unique_ptr<IOutputWriter> GetScopedWriter(Severity severity, int line, const std::string& scope, const char * file, const char * function) = 0;
//	virtual std::unique_ptr<IOutputWriter> GetGeneralWriter(Severity severity, int line, const char * file, const char * function) = 0;
//
//	virtual bool IsEnabled(void) const noexcept = 0;
//	virtual void SetEnabled(bool enabled = true) noexcept = 0;
//
//	virtual Severity GetSeverity(void) const noexcept = 0;
//	virtual void SetSeverity(Severity severity) noexcept = 0;
//};
//
//class LogManager {
//public:
//	static std::shared_ptr<ILogger> GetLoggerInstance(void);
//
//private:
//	LogManager();
//};
//
//} // vanillapdf
//
//#define __LOG_INTERNAL_HELPER__(Scope, Severity)       vanillapdf::LogManager::GetLoggerInstance()->GetScopedWriter(Severity, __LINE__, Scope, __FILE__, __FUNCTION__)
//
//#define LOG_DEBUG(Scope)		__LOG_INTERNAL_HELPER__(Scope, vanillapdf::Severity::Debug)
//#define LOG_INFO(Scope)			__LOG_INTERNAL_HELPER__(Scope, vanillapdf::Severity::Info)
//#define LOG_WARNING(Scope)		__LOG_INTERNAL_HELPER__(Scope, vanillapdf::Severity::Warning)
//#define LOG_ERROR(Scope)		__LOG_INTERNAL_HELPER__(Scope, vanillapdf::Severity::Error)
//#define LOG_FATAL(Scope)		__LOG_INTERNAL_HELPER__(Scope, vanillapdf::Severity::Fatal)
//
//#define LOG_DEBUG_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, vanillapdf::Severity::Debug)
//#define LOG_INFO_GLOBAL			__LOG_INTERNAL_HELPER__(nullptr, vanillapdf::Severity::Info)
//#define LOG_WARNING_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, vanillapdf::Severity::Warning)
//#define LOG_ERROR_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, vanillapdf::Severity::Error)
//#define LOG_FATAL_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, vanillapdf::Severity::Fatal)
//
#endif /* _LOG_H */
