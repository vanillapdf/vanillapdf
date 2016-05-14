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
				"'");     // this apostrophe is not mentioned, but is included in all examples

			std::smatch sm;
			if (!std::regex_match(str, sm, header_regex))
				throw GeneralException("Could not parse datetime: " + str);

			assert(sm.size() == 10);
			m_year = std::stoi(sm[1]);
			m_month = std::stoi(sm[2]);
			m_day = std::stoi(sm[3]);
			m_hour = std::stoi(sm[4]);
			m_minute = std::stoi(sm[5]);
			m_second = std::stoi(sm[6]);
			m_hour_offset = std::stoi(sm[8]);
			m_minute_offset = std::stoi(sm[9]);

			if (sm[7].str() == "+") {
				m_timezone = TimezoneType::Later;
			}
			else if (sm[7].str() == "-") {
				m_timezone = TimezoneType::Earlier;
			}
			else if (sm[7].str() == "Z") {
				m_timezone = TimezoneType::UTC;
			}
			else {
				assert(false && "Error in regex above");
				throw GeneralException("Could not parse datetime: " + str);
			}

			if (m_month < 1 || m_month > 12) throw GeneralException("Month is out of range: " + std::to_string(m_month));
			if (m_day < 1 || m_day > 31) throw GeneralException("Day is out of range: " + std::to_string(m_day));
			if (m_hour < 0 || m_hour > 23) throw GeneralException("Hour is out of range: " + std::to_string(m_hour));
			if (m_minute < 0 || m_minute > 59) throw GeneralException("Minute is out of range: " + std::to_string(m_minute));
			if (m_second < 0 || m_second > 59) throw GeneralException("Second is out of range: " + std::to_string(m_second));
			if (m_hour_offset < 0 || m_hour_offset > 23) throw GeneralException("Hour offset is out of range: " + std::to_string(m_hour_offset));
			if (m_minute_offset < 0 || m_minute_offset > 59) throw GeneralException("Minute offset is out of range: " + std::to_string(m_minute_offset));
		}
	}
}
