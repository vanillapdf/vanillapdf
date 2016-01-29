#include "precompiled.h"
#include "log.h"
#include "file.h"

#include <fstream>
#include <chrono>
#include <iomanip>
#include <memory>

namespace gotchangpdf
{
	static const char general_failure_filename[] = "general.log";

	Severity Logger::m_severity = Severity::warning;

	std::mutex Logger::m_master;
	std::shared_ptr<std::mutex> Logger::m_general = std::make_shared<std::mutex>();
	std::map<std::string, std::shared_ptr<std::mutex>> Logger::m_mutexes;

	basic_nullbuf<char> Logger::m_devnullbuf;
	std::shared_ptr<std::ostream> Logger::m_devnull = std::make_shared<std::ostream>(&Logger::m_devnullbuf);

	OutputWriter::OutputWriter(
		std::shared_ptr<std::mutex> locked_mutex,
		std::shared_ptr<std::ostream> output_stream,
		Severity severity,
		int line,
		const char * file,
		const char * function
		) : m_mutex(locked_mutex),
		m_output_stream(output_stream),
		m_line(line),
		m_file(file),
		m_function(function)
	{
		std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
		auto rawtime = std::chrono::system_clock::to_time_t(today);

		struct tm timeinfo;
		auto err = localtime_s(&timeinfo, &rawtime);
		assert(err == 0); (void)err;

		char buffer[128];
		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &timeinfo);
		std::string date(buffer);

		*m_output_stream << '[';
		*m_output_stream << date;
		*m_output_stream << ']';
		*m_output_stream << ' ';
		*m_output_stream << '(';
		*m_output_stream << "0x" << std::hex << std::setfill('0') << std::setw(8) << std::this_thread::get_id();
		*m_output_stream << ')';
		*m_output_stream << ' ';
		*m_output_stream << '<';
		*m_output_stream << severity;
		*m_output_stream << '>';
		*m_output_stream << ':';
		*m_output_stream << ' ';
	}

	OutputWriter::~OutputWriter()
	{
		try
		{
			*m_output_stream << ' ';
			*m_output_stream << '{';
			*m_output_stream << ' ';
			*m_output_stream << extract_filename(m_file);
			*m_output_stream << ':';
			*m_output_stream << std::dec << m_line;
			*m_output_stream << " in ";
			*m_output_stream << m_function;
			*m_output_stream << ' ';
			*m_output_stream << '}';
			*m_output_stream << std::endl;

			m_output_stream->flush();
			if (m_mutex) m_mutex->unlock();
		}
		catch (...) {
			assert(!"Exception in logging destructor");
		}
	}

	std::unique_ptr<OutputWriter> Logger::GetGeneralWriter(Severity severity, int line, const char * file, const char * function)
	{
		if (severity < m_severity) {
			auto fake_mutex = std::shared_ptr<std::mutex>(nullptr);
			return std::make_unique<OutputWriter>(fake_mutex, m_devnull, severity, line, file, function);
		}

		auto output_stream = std::make_shared<std::ofstream>();
		output_stream->open(general_failure_filename, std::ios::app);
		if (!output_stream->good()) {
			assert(!"Problem opening general log file");
			auto fake_mutex = std::shared_ptr<std::mutex>(nullptr);
			return std::make_unique<OutputWriter>(fake_mutex, m_devnull, severity, line, file, function);
		}

		m_general->lock();
		return std::make_unique<OutputWriter>(m_general, output_stream, severity, line, file, function);
	}

	std::unique_ptr<OutputWriter> Logger::GetScopedWriter(Severity severity, int line, const std::weak_ptr<syntax::File>& scope, const char * file, const char * function)
	{
		auto locked_file = scope.lock();
		if (!locked_file) return GetGeneralWriter(severity, line, file, function);
		auto filename = locked_file->GetFilename();
		return GetScopedWriter(severity, line, filename.c_str(), file, function);
	}

	std::unique_ptr<OutputWriter> Logger::GetScopedWriter(Severity severity, int line, const std::string& scope, const char * file, const char * function)
	{
		if (severity < m_severity) {
			auto fake_mutex = std::shared_ptr<std::mutex>(nullptr);
			return std::make_unique<OutputWriter>(fake_mutex, m_devnull, severity, line, file, function);
		}

		if (scope.empty() || (scope == general_failure_filename)) {
			return GetGeneralWriter(severity, line, file, function);
		}

		std::shared_ptr<std::mutex> scoped_mutex;
		auto found = m_mutexes.find(scope);
		if (found == m_mutexes.end()) {
			std::lock_guard<std::mutex> locker(m_master);
			found = m_mutexes.find(scope);
			if (found == m_mutexes.end()) {
				scoped_mutex = std::make_shared<std::mutex>();
				m_mutexes[scope] = scoped_mutex;
			}
		}
		else {
			scoped_mutex = found->second;
		}

		auto output_stream = std::make_shared<std::ofstream>();
		output_stream->open(scope + ".log", std::ios::app);
		if (!output_stream->good()) {
			assert(!"Problem opening scoped log file");
			return GetGeneralWriter(severity, line, file, function);
		}

		scoped_mutex->lock();
		return std::make_unique<OutputWriter>(scoped_mutex, output_stream, severity, line, file, function);
	}

	std::unique_ptr<OutputWriter> Logger::GetScopedWriter(Severity severity, int line, const char * scope, const char * file, const char * function)
	{
		if (nullptr == scope) {
			return GetScopedWriter(severity, line, std::string(), file, function);
		}

		return GetScopedWriter(severity, line, std::string(scope), file, function);
	}
}