#ifndef _LOG_H
#define _LOG_H

#include "syntax_fwd.h"

#include <mutex>
#include <memory>
#include <map>

namespace gotchangpdf
{
	enum class Severity
	{
		debug,
		info,
		warning,
		error,
		fatal
	};

	template <typename CharT, typename TraitsT>
	std::basic_ostream<CharT, TraitsT>& operator<< (std::basic_ostream<CharT, TraitsT>& strm, const Severity level)
	{
		switch (level) {
		case Severity::debug:
			strm << "debug"; break;
		case Severity::info:
			strm << "info"; break;
		case Severity::warning:
			strm << "warning"; break;
		case Severity::error:
			strm << "error"; break;
		case Severity::fatal:
			strm << "fatal"; break;
		}

		return strm;
	}

	class OutputWriter
	{
	public:
		template <typename T>
		friend std::unique_ptr<OutputWriter> operator<<(std::unique_ptr<OutputWriter> strm, const T& value);

	public:
		OutputWriter(
			std::shared_ptr<std::mutex> locked_mutex,
			std::shared_ptr<std::ostream> output_stream,
			Severity severity,
			int line,
			const char * file,
			const char * function
			);

		OutputWriter(const OutputWriter&) = delete;
		OutputWriter(OutputWriter&&) = default;
		~OutputWriter();

	private:
		std::shared_ptr<std::mutex> m_mutex;
		std::shared_ptr<std::ostream> m_output_stream;
		int m_line;
		const char * m_file;
		const char * m_function;

		std::string GetLocalTime() const;
	};

	template <typename T>
	std::unique_ptr<OutputWriter> operator<<(std::unique_ptr<OutputWriter> strm, const T& value)
	{
		*strm->m_output_stream << value;
		return strm;
	}

	template<typename Ch, typename Traits = std::char_traits<Ch> >
	struct basic_nullbuf : std::basic_streambuf<Ch, Traits> {
		typedef std::basic_streambuf<Ch, Traits> base_type;
		typedef typename base_type::int_type int_type;
		typedef typename base_type::traits_type traits_type;

		virtual int_type overflow(int_type c) {
			return traits_type::not_eof(c);
		}
	};

	class Logger
	{
	public:
		static std::unique_ptr<OutputWriter> GetScopedWriter(Severity severity, int line, const char * scope, const char * file, const char * function);
		static std::unique_ptr<OutputWriter> GetScopedWriter(Severity severity, int line, const std::weak_ptr<syntax::File>& scope, const char * file, const char * function);
		static std::unique_ptr<OutputWriter> GetScopedWriter(Severity severity, int line, const std::string& scope, const char * file, const char * function);
		static std::unique_ptr<OutputWriter> GetGeneralWriter(Severity severity, int line, const char * file, const char * function);

	private:
		Logger() = default;

		static std::map<std::string, std::shared_ptr<std::mutex>> m_mutexes;
		static std::mutex m_master;
		static std::shared_ptr<std::mutex> m_general;
		static basic_nullbuf<char> m_devnullbuf;
		static std::shared_ptr<std::ostream> m_devnull;
		static Severity m_severity;
	};
}

#define __LOG_INTERNAL_HELPER__(Scope, Severity)       gotchangpdf::Logger::GetScopedWriter(Severity, __LINE__, Scope, __FILE__, __FUNCTION__)

#define LOG_DEBUG(Scope)		__LOG_INTERNAL_HELPER__(Scope, gotchangpdf::Severity::debug)
#define LOG_INFO(Scope)			__LOG_INTERNAL_HELPER__(Scope, gotchangpdf::Severity::info)
#define LOG_WARNING(Scope)		__LOG_INTERNAL_HELPER__(Scope, gotchangpdf::Severity::warning)
#define LOG_ERROR(Scope)		__LOG_INTERNAL_HELPER__(Scope, gotchangpdf::Severity::error)
#define LOG_FATAL(Scope)		__LOG_INTERNAL_HELPER__(Scope, gotchangpdf::Severity::fatal)

#define LOG_DEBUG_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, gotchangpdf::Severity::debug)
#define LOG_INFO_GLOBAL			__LOG_INTERNAL_HELPER__(nullptr, gotchangpdf::Severity::info)
#define LOG_WARNING_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, gotchangpdf::Severity::warning)
#define LOG_ERROR_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, gotchangpdf::Severity::error)
#define LOG_FATAL_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, gotchangpdf::Severity::fatal)

#endif /* _LOG_H */
