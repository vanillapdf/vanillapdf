#ifndef _DATE_H
#define _DATE_H

#include "semantics_fwd.h"
#include "high_level_object.h"

#include <ctime>

namespace gotchangpdf
{
	namespace semantics
	{
		class Date : public HighLevelObject<syntax::StringObjectPtr>
		{
		public:
			enum class TimezoneType
			{
				UTC = 'Z',
				Later = '+',
				Earlier = '-'
			};

		public:
			explicit Date(syntax::StringObjectPtr root);
			int Year(void) const noexcept { return m_year; }
			int Month(void) const noexcept { return m_month; }
			int Day(void) const noexcept { return m_day; }
			int Hour(void) const noexcept { return m_hour; }
			int Minute(void) const noexcept { return m_minute; }
			int Second(void) const noexcept { return m_second; }
			TimezoneType Timezone(void) const noexcept { return m_timezone; }
			int HourOffset(void) const noexcept { return m_hour_offset; }
			int MinuteOffset(void) const noexcept { return m_minute_offset; }

		private:
			int m_year = 0;
			int m_month = 0;
			int m_day = 0;
			int m_hour = 0;
			int m_minute = 0;
			int m_second = 0;
			TimezoneType m_timezone = TimezoneType::UTC;
			int m_hour_offset = 0;
			int m_minute_offset = 0;
		};
	}
}

#endif /* _DATE_H */
