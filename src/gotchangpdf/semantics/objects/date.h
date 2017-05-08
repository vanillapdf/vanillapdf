#ifndef _DATE_H
#define _DATE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "utils/time_utils.h"

namespace gotchangpdf {
namespace semantics {

class Date : public HighLevelObject<syntax::StringObjectPtr> {
public:
	explicit Date(syntax::StringObjectPtr root);

	int GetYear(void) const noexcept { return m_year; }
	int GetMonth(void) const noexcept { return m_month; }
	int GetDay(void) const noexcept { return m_day; }
	int GetHour(void) const noexcept { return m_hour; }
	int GetMinute(void) const noexcept { return m_minute; }
	int GetSecond(void) const noexcept { return m_second; }
	Timezone GetTimezone(void) const noexcept { return m_timezone; }
	int GetHourOffset(void) const noexcept { return m_hour_offset; }
	int GetMinuteOffset(void) const noexcept { return m_minute_offset; }

	static DatePtr GetCurrentDate();

private:
	int m_year = 1900;
	int m_month = 1;
	int m_day = 1;
	int m_hour = 0;
	int m_minute = 0;
	int m_second = 0;
	Timezone m_timezone = Timezone::UTC;
	int m_hour_offset = 0;
	int m_minute_offset = 0;
};

} // semantics
} // gotchangpdf

#endif /* _DATE_H */
