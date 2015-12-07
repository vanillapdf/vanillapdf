#include "precompiled.h"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace gotchangpdf
{
	namespace log
	{
		std::string convert_posix_time(const boost::posix_time::ptime& time)
		{
			static const char* time_format = "%Y-%m-%d %H:%M:%S";
			static const std::locale loc(std::cout.getloc(),
				new boost::posix_time::time_facet(time_format));

			std::stringstream ss;
			ss.imbue(loc);
			ss << time;
			return ss.str();
		}

		const Severity max_level = Severity::warning;
		void log_formatter(logging::record_view const& rec, logging::formatting_ostream& strm)
		{
			auto posix_tm = logging::extract<boost::posix_time::ptime>("TimeStamp", rec);
			auto time_converted = convert_posix_time(*posix_tm);

			strm << "[" << time_converted << "] ";
			strm << "(" << logging::extract<attrs::current_thread_id::value_type>("ThreadID", rec) << ") ";
			strm << "<" << logging::extract<Severity>("Severity", rec) << ">: ";
			strm << rec[expr::smessage];

			auto full_path = logging::extract< std::string >("File", rec);
			strm << " { "
				<< boost::filesystem::path(full_path.get()).filename().string()
				<< ": " << logging::extract<long>("Line", rec)
				<< " in " << logging::extract<std::string>("Function", rec)
				<< " }";
		}
	}
}