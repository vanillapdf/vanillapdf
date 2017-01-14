#include "precompiled.h"

#include "utils/log.h"
#include "utils/util.h"

#include <fstream>
#include <chrono>
#include <iomanip>
#include <memory>
#include <thread>

namespace gotchangpdf {

static const char general_failure_filename[] = "general.log";

// Initialize empty instance property
std::shared_ptr<Logger> Logger::m_instance;

std::shared_ptr<Logger> Logger::GetInstance(void) {
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
	*m_output_stream << GetLocalTime();
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

std::string OutputWriter::GetLocalTime() const {
	// localtime function may not be thread-safe
	static std::mutex m_chrono;

	std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
	auto rawtime = std::chrono::system_clock::to_time_t(today);

	std::lock_guard<std::mutex> locker(m_chrono);
	auto timeinfo = std::localtime(&rawtime);
	if (timeinfo == nullptr) {
		throw GeneralException("Could not get current time");
	}

	char buffer[128];
	size_t result = strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
	if (result == 0) {
		throw GeneralException("Could not convert localtime to string");
	}

	return std::string(buffer);
}

OutputWriter::~OutputWriter() {
	try {
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

		// All output writers except /dev/null
		// shall have a mutex
		if (m_mutex) {
			m_mutex->unlock();
		}
	} catch (...) {
		assert(!"Exception in logging destructor");
	}
}

std::unique_ptr<OutputWriter> Logger::GetDevNull(Severity severity, int line, const char * file, const char * function) {
	auto fake_mutex = std::shared_ptr<std::mutex>(nullptr);
	return make_unique<OutputWriter>(fake_mutex, m_devnull, severity, line, file, function);
}

std::unique_ptr<OutputWriter> Logger::GetGeneralWriter(Severity severity, int line, const char * file, const char * function) {
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

std::unique_ptr<OutputWriter> Logger::GetScopedWriter(Severity severity, int line, const std::string& scope, const char * file, const char * function) {
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

std::unique_ptr<OutputWriter> Logger::GetScopedWriter(Severity severity, int line, const char * scope, const char * file, const char * function) {
	if (nullptr == scope) {
		return GetScopedWriter(severity, line, std::string(), file, function);
	}

	return GetScopedWriter(severity, line, std::string(scope), file, function);
}

} // gotchangpdf
