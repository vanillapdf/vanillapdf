#include "precompiled.h"
#include "date.h"

#include <regex>

namespace gotchangpdf
{
	namespace semantics
	{
		Date::Date(syntax::StringObjectPtr root) : HighLevelObject(root)
		{
			auto str = root->Value()->ToString();

			//(D:YYYYMMDDHHmmSSOHH'mm)
			std::regex header_regex(
				"D:" // date header
				"(\\d{4})" // year
				"(\\d{2})" // month
				"(\\d{2})" // day
				"(\\d{2})" // hour
				"(\\d{2})" // minute
				"(\\d{2})" // second
				"([+-Z])"  // offset type
				"(\\d{2})" // offset hour
				"'"        // offset separator
				"(\\d{2})" // offset minutes
				".*");     // ignore junk

			std::smatch sm;
			if (!std::regex_match(str, sm, header_regex))
				throw GeneralException("Could not parse date: " + str);

			assert(sm.size() == 10);
			auto year = std::stoi(sm[1]);
			auto month = std::stoi(sm[2]);
			auto day = std::stoi(sm[3]);
			auto hour = std::stoi(sm[4]);
			auto minute = std::stoi(sm[5]);
			auto second = std::stoi(sm[6]);
			auto offset = sm[7];
			//auto hour_offset = std::stoi(sm[8]);
			//auto minute_offset = std::stoi(sm[9]);

			tm time;
			time.tm_year = year - 1900;
			time.tm_mon = month;
			time.tm_mday = day;
			time.tm_hour = hour;
			time.tm_min = minute;
			time.tm_sec = second;
			m_time = mktime(&time);
		}

		time_t Date::Value(void) { return m_time; }
	}
}
