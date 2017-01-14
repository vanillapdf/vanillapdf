#include "precompiled.h"
#include "semantics/objects/date.h"

#include <regex>

namespace gotchangpdf
{
	namespace semantics
	{
		Date::Date(syntax::StringObjectPtr root) : HighLevelObject(root)
		{
			auto str = root->GetValue()->ToString();

			//(D:YYYYMMDDHHmmSSOHH'mm)
			std::regex header_regex(
				"^(?:D:)?" // date header
				"(\\d{4})" // year
				"(\\d$|\\d{2}|$)" // month
				"(\\d$|\\d{2}|$)" // day
				"(\\d$|\\d{2}|$)" // hour
				"(\\d$|\\d{2}|$)" // minute
				"(\\d$|\\d{2}|$)" // second

				"(?:"
					"([Z+-]|$)"		// offset type
					"(\\d{2}|$)"	// offset hour
					"'?"			// offset separator
					"(\\d{2}|$)"	// offset minutes
					"'?"			// this apostrophe is not mentioned, but is included in all examples
				")$"
				);

			std::smatch sm;
			if (!std::regex_match(str, sm, header_regex))
				throw GeneralException("Could not parse datetime: " + str);

			auto length = sm.size();

			if (length >= 2 && sm[1].matched && sm[1].str().size() > 0) m_year = std::stoi(sm[1]);
			if (length >= 3 && sm[2].matched && sm[2].str().size() > 0) m_month = std::stoi(sm[2]);
			if (length >= 4 && sm[3].matched && sm[3].str().size() > 0) m_day = std::stoi(sm[3]);
			if (length >= 5 && sm[4].matched && sm[4].str().size() > 0) m_hour = std::stoi(sm[4]);
			if (length >= 6 && sm[5].matched && sm[5].str().size() > 0) m_minute = std::stoi(sm[5]);
			if (length >= 7 && sm[6].matched && sm[6].str().size() > 0) m_second = std::stoi(sm[6]);
			if (length >= 9 && sm[8].matched && sm[8].str().size() > 0) m_hour_offset = std::stoi(sm[8]);
			if (length >= 10 && sm[9].matched && sm[9].str().size() > 0) m_minute_offset = std::stoi(sm[9]);

			if (length >= 8 && sm[7].matched) {
				if (sm[7].str() == "Z" || sm[7].str() == "")
					m_timezone = TimezoneType::UTC;
				else if (sm[7].str() == "+") {
					m_timezone = TimezoneType::Later;
				}
				else if (sm[7].str() == "-") {
					m_timezone = TimezoneType::Earlier;
				}
				else {
					assert(false && "Error in regular expression above");
					throw GeneralException("Could not parse datetime: " + str);
				}
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
