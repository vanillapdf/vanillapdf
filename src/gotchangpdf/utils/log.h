#ifndef _LOG_H
#define _LOG_H

#include <ostream>
#include <mutex>
#include <memory>
#include <map>

namespace gotchangpdf
{
	enum class Severity
	{
		Debug,
		Info,
		Warning,
		Error,
		Fatal
	};

	template <typename CharT, typename TraitsT>
	std::basic_ostream<CharT, TraitsT>& operator<< (std::basic_ostream<CharT, TraitsT>& strm, const Severity level)
	{
		switch (level) {
		case Severity::Debug:
			strm << "Debug"; break;
		case Severity::Info:
			strm << "Info"; break;
		case Severity::Warning:
			strm << "Warning"; break;
		case Severity::Error:
			strm << "Error"; break;
		case Severity::Fatal:
			strm << "Fatal"; break;
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
		static std::shared_ptr<Logger> GetInstance(void);

	public:
		std::unique_ptr<OutputWriter> GetScopedWriter(Severity severity, int line, const char * scope, const char * file, const char * function);
		std::unique_ptr<OutputWriter> GetScopedWriter(Severity severity, int line, const std::string& scope, const char * file, const char * function);
		std::unique_ptr<OutputWriter> GetGeneralWriter(Severity severity, int line, const char * file, const char * function);

		bool IsEnabled(void) const noexcept { return m_enabled; }
		void SetEnabled(bool enabled = true) noexcept { m_enabled = enabled; }

		Severity GetSeverity(void) const noexcept { return m_severity; }
		void SetSeverity(Severity severity) noexcept { m_severity = severity; }

	private:
		Logger();

		// Singleton
		static std::shared_ptr<Logger> m_instance;

		std::map<std::string, std::shared_ptr<std::mutex>> m_mutexes;
		std::mutex m_master;
		std::shared_ptr<std::mutex> m_general;
		basic_nullbuf<char> m_devnullbuf;
		std::shared_ptr<std::ostream> m_devnull;
		Severity m_severity;
		bool m_enabled;

		std::unique_ptr<OutputWriter> GetDevNull(Severity severity, int line, const char * file, const char * function);
	};
}

#define __LOG_INTERNAL_HELPER__(Scope, Severity)       gotchangpdf::Logger::GetInstance()->GetScopedWriter(Severity, __LINE__, Scope, __FILE__, __FUNCTION__)

#define LOG_DEBUG(Scope)		__LOG_INTERNAL_HELPER__(Scope, gotchangpdf::Severity::Debug)
#define LOG_INFO(Scope)			__LOG_INTERNAL_HELPER__(Scope, gotchangpdf::Severity::Info)
#define LOG_WARNING(Scope)		__LOG_INTERNAL_HELPER__(Scope, gotchangpdf::Severity::Warning)
#define LOG_ERROR(Scope)		__LOG_INTERNAL_HELPER__(Scope, gotchangpdf::Severity::Error)
#define LOG_FATAL(Scope)		__LOG_INTERNAL_HELPER__(Scope, gotchangpdf::Severity::Fatal)

#define LOG_DEBUG_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, gotchangpdf::Severity::Debug)
#define LOG_INFO_GLOBAL			__LOG_INTERNAL_HELPER__(nullptr, gotchangpdf::Severity::Info)
#define LOG_WARNING_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, gotchangpdf::Severity::Warning)
#define LOG_ERROR_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, gotchangpdf::Severity::Error)
#define LOG_FATAL_GLOBAL		__LOG_INTERNAL_HELPER__(nullptr, gotchangpdf::Severity::Fatal)

#endif /* _LOG_H */
