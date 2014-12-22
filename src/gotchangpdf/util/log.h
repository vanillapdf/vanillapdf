#ifndef _LOG_H
#define _LOG_H

#include "file.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sinks/text_multifile_backend.hpp>

namespace gotchangpdf
{
	namespace log
	{
		namespace logging = boost::log;
		namespace src = boost::log::sources;
		namespace sinks = boost::log::sinks;
		namespace keywords = boost::log::keywords;
		namespace expr = boost::log::expressions;

		enum class Severity
		{
			debug,
			info,
			warning,
			error,
			fatal
		};

		typedef src::severity_channel_logger_mt<
			Severity,
			std::string
		> file_logger_mt;

		BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(file_logger, file_logger_mt)
		{
			file_logger_mt lg;

			//logging::add_common_attributes();
			//logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);

			//logging::add_file_log
			//	(
			//	keywords::file_name = "sample_%N.log",                                        /*< file name pattern >*/
			//	keywords::rotation_size = 10 * 1024 * 1024,                                   /*< rotate files every 10 MiB... >*/
			//	keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), /*< ...or at midnight >*/
			//	keywords::format = "[%TimeStamp%]: %Message%"                                 /*< log record format >*/
			//	);

			auto core = logging::core::get();

			auto backend = boost::make_shared<sinks::text_multifile_backend>();

			// Set up the file naming pattern
			backend->set_file_name_composer(sinks::file::as_file_name_composer(expr::stream << "logs/" << expr::attr<std::string>("Filename") << ".log"));

			// Wrap it into the frontend and register in the core.
			// The backend requires synchronization in the frontend.
			typedef sinks::synchronous_sink<sinks::text_multifile_backend> sink_t;
			boost::shared_ptr<sink_t> sink(new sink_t(backend));

			// Set the formatter
			sink->set_formatter
				(
				expr::stream
				<< "[Filename: " << expr::attr<std::string>("Filename")
				<< "] " << expr::smessage
				);

			core->add_sink(sink);

			return lg;
		}
	}
}

#define LOG_DEBUG BOOST_LOG_SEV(gotchangpdf::log::file_logger::get(), gotchangpdf::log::Severity::debug)
#define LOG_INFO  BOOST_LOG_SEV(gotchangpdf::log::file_logger::get(), gotchangpdf::log::Severity::info)
#define LOG_WARN  BOOST_LOG_SEV(gotchangpdf::log::file_logger::get(), gotchangpdf::log::Severity::warning)
#define LOG_ERROR BOOST_LOG_SEV(gotchangpdf::log::file_logger::get(), gotchangpdf::log::Severity::error)
#define LOG_FATAL BOOST_LOG_SEV(gotchangpdf::log::file_logger::get(), gotchangpdf::log::Severity::fatal)

//int main(int, char*[])
//{
//	using namespace boost::log::trivial;
//
//	BOOST_LOG_SEV(gotchangpdf::file_logger::get(), warning) << "Hello, world!";
//	BOOST_LOG_CHANNEL(gotchangpdf::file_logger::get(), "net");
//		//<< logging::add_value("RemoteAddress", m_remote_addr)
//		//<< "Connection shut down";
//	/*
//	BOOST_LOG_SEV(lg, trace) << "A trace severity message";
//	BOOST_LOG_SEV(lg, debug) << "A debug severity message";
//	BOOST_LOG_SEV(lg, info) << "An informational severity message";
//	BOOST_LOG_SEV(lg, warning) << "A warning severity message";
//	BOOST_LOG_SEV(lg, error) << "An error severity message";
//	BOOST_LOG_SEV(lg, fatal) << "A fatal severity message";
//	*/
//	return 0;
//}

#endif /* _LOG_H */
