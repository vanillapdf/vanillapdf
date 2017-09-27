#include "precompiled.h"

#include "utils/log.h"
#include "utils/time_utils.h"
#include "utils/other_utils.h"

#include <map>
#include <mutex>
#include <thread>
#include <fstream>
#include <iomanip>

namespace gotchangpdf {

static const char general_failure_filename[] = "general.log";
static const char date_format[] = "%d-%m-%Y %H:%M:%S";

#pragma region NullBuf

template<typename Ch, typename Traits = std::char_traits<Ch> >
struct basic_nullbuf : std::basic_streambuf<Ch, Traits> {
	typedef std::basic_streambuf<Ch, Traits> base_type;
	typedef typename base_type::int_type int_type;
	typedef typename base_type::traits_type traits_type;

	virtual int_type overflow(int_type c) {
		return traits_type::not_eof(c);
	}
};

#pragma endregion

#pragma region OutputWriter

class OutputWriter : public IOutputWriter {
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

	virtual std::ostream& GetOutputStream() const override;
	virtual ~OutputWriter() override;

private:
	std::shared_ptr<std::mutex> m_mutex;
	std::shared_ptr<std::ostream> m_output_stream;
	int m_line;
	const char * m_file;
	const char * m_function;
};

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
	m_function(function) {
	*m_output_stream << '[';
	*m_output_stream << TimeUtils::GetCurrentTimeString(date_format);
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

std::ostream& OutputWriter::GetOutputStream() const {
	return *m_output_stream;
}

OutputWriter::~OutputWriter() {
	try {
		*m_output_stream << ' ';
		*m_output_stream << '{';
		*m_output_stream << ' ';
		*m_output_stream << ExtractFilename(m_file);
		*m_output_stream << ':';
		*m_output_stream << std::dec << m_line;
		*m_output_stream << " in ";
		*m_output_stream << m_function;
		*m_output_stream << ' ';
		*m_output_stream << '}';
		*m_output_stream << std::endl;

		m_output_stream->flush();

		// All output writers except /dev/null
		// shall have a mutex
		if (m_mutex) {
			m_mutex->unlock();
		}
	} catch (...) {
		assert(!"Exception in logging destructor");
	}
}

IOutputWriter::~IOutputWriter() {

}

#pragma endregion

#pragma region Logger

class Logger : public ILogger {
public:
	static std::shared_ptr<ILogger> GetInstance(void);

public:
	std::unique_ptr<IOutputWriter> GetScopedWriter(Severity severity, int line, const char * scope, const char * file, const char * function) override;
	std::unique_ptr<IOutputWriter> GetScopedWriter(Severity severity, int line, const std::string& scope, const char * file, const char * function) override;
	std::unique_ptr<IOutputWriter> GetGeneralWriter(Severity severity, int line, const char * file, const char * function) override;

	bool IsEnabled(void) const noexcept override {
		return m_enabled;
	}

	void SetEnabled(bool enabled = true) noexcept override {
		m_enabled = enabled;
	}

	Severity GetSeverity(void) const noexcept override {
		return m_severity;
	}

	void SetSeverity(Severity severity) noexcept override {
		m_severity = severity;
	}

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

	std::unique_ptr<IOutputWriter> GetDevNull(Severity severity, int line, const char * file, const char * function);
};

// Initialize empty instance property
std::shared_ptr<Logger> Logger::m_instance;

std::shared_ptr<ILogger> Logger::GetInstance(void) {
	if (m_instance) {
		return m_instance;
	}

	static std::mutex instance_mutex;
	std::lock_guard<std::mutex> locker(instance_mutex);
	if (!m_instance) {
		m_instance = std::shared_ptr<Logger>(new Logger());
	}

	return m_instance;
}

Logger::Logger() {
	m_enabled = false;
	m_severity = Severity::Warning;
	m_general = std::make_shared<std::mutex>();
	m_devnull = std::make_shared<std::ostream>(&m_devnullbuf);
}

std::unique_ptr<IOutputWriter> Logger::GetDevNull(Severity severity, int line, const char * file, const char * function) {
	auto fake_mutex = std::shared_ptr<std::mutex>(nullptr);
	return make_unique<OutputWriter>(fake_mutex, m_devnull, severity, line, file, function);
}

std::unique_ptr<IOutputWriter> Logger::GetGeneralWriter(Severity severity, int line, const char * file, const char * function) {
	if (!m_enabled) {
		return GetDevNull(severity, line, file, function);
	}

	if (severity < m_severity) {
		return GetDevNull(severity, line, file, function);
	}

	auto output_stream = std::make_shared<std::ofstream>();
	output_stream->open(general_failure_filename, std::ios::app);
	if (!output_stream->good()) {
		assert(!"Problem opening general log file");
		return GetDevNull(severity, line, file, function);
	}

	m_general->lock();
	return make_unique<OutputWriter>(m_general, output_stream, severity, line, file, function);
}

std::unique_ptr<IOutputWriter> Logger::GetScopedWriter(Severity severity, int line, const std::string& scope, const char * file, const char * function) {
	if (!m_enabled) {
		return GetDevNull(severity, line, file, function);
	}

	if (severity < m_severity) {
		return GetDevNull(severity, line, file, function);
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
	} else {
		scoped_mutex = found->second;
	}

	auto output_stream = std::make_shared<std::ofstream>();
	output_stream->open(scope + ".log", std::ios::app);
	if (!output_stream->good()) {
		assert(!"Problem opening scoped log file");
		return GetGeneralWriter(severity, line, file, function);
	}

	scoped_mutex->lock();
	return make_unique<OutputWriter>(scoped_mutex, output_stream, severity, line, file, function);
}

std::unique_ptr<IOutputWriter> Logger::GetScopedWriter(Severity severity, int line, const char * scope, const char * file, const char * function) {
	if (nullptr == scope) {
		return GetScopedWriter(severity, line, std::string(), file, function);
	}

	return GetScopedWriter(severity, line, std::string(scope), file, function);
}

#pragma endregion

#pragma region LogManager

std::shared_ptr<ILogger> LogManager::GetLoggerInstance(void) {
	return Logger::GetInstance();
}

#pragma endregion

} // gotchangpdf
