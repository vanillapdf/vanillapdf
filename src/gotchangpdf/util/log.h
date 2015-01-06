#ifndef _LOG_H
#define _LOG_H

#include "file.h"

#pragma warning(push)
#pragma warning(disable: 4996)

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_multifile_backend.hpp>

#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/support/date_time.hpp>

#pragma warning(pop)

namespace gotchangpdf
{
	namespace log
	{
		namespace logging = boost::log;
		namespace src = boost::log::sources;
		namespace sinks = boost::log::sinks;
		namespace expr = boost::log::expressions;
		namespace attrs = boost::log::attributes;

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

		typedef src::severity_logger_mt<Severity> file_logger_mt;
		BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(file_logger, file_logger_mt)
		{
			file_logger_mt lg;

			logging::add_common_attributes();

			// Add per file scope
			auto core = logging::core::get();
			core->add_global_attribute("Scope", attrs::constant<std::string>("general"));
			core->set_filter(expr::attr<Severity>("Severity") >= Severity::warning);

			// One log file for every scope
			auto backend = boost::make_shared<sinks::text_multifile_backend>();

			// Log destination directory
			auto filename = expr::stream << "log/" << expr::attr<std::string>("Scope") << ".log";
			auto composer = sinks::file::as_file_name_composer(filename);

			// Set up the file naming pattern
			backend->set_file_name_composer(composer);

			// Wrap it into the frontend and register in the core.
			// The backend requires synchronization in the frontend.
			typedef sinks::synchronous_sink<sinks::text_multifile_backend> sink_t;
			boost::shared_ptr<sink_t> sink(new sink_t(backend));

			// Set the formatter
			sink->set_formatter(
				expr::format("[%1%] (%2%) <%3%> : %4%")
				% expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
				% expr::attr<attrs::current_thread_id::value_type>("ThreadID")
				% expr::attr<Severity>("Severity")
				% expr::smessage
				);

			core->add_sink(sink);

			return lg;
		}
	}
}

//BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
//BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
//BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)

#define LOG_SCOPE(name) BOOST_LOG_SCOPED_THREAD_ATTR("Scope", boost::log::attributes::constant<std::string>(name))
#define LOG_DEBUG       BOOST_LOG_SEV(gotchangpdf::log::file_logger::get(), gotchangpdf::log::Severity::debug)
#define LOG_INFO        BOOST_LOG_SEV(gotchangpdf::log::file_logger::get(), gotchangpdf::log::Severity::info)
#define LOG_WARN        BOOST_LOG_SEV(gotchangpdf::log::file_logger::get(), gotchangpdf::log::Severity::warning)
#define LOG_ERROR       BOOST_LOG_SEV(gotchangpdf::log::file_logger::get(), gotchangpdf::log::Severity::error)
#define LOG_FATAL       BOOST_LOG_SEV(gotchangpdf::log::file_logger::get(), gotchangpdf::log::Severity::fatal)

#endif /* _LOG_H */
