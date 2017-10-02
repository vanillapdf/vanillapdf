#ifndef _DATE_H
#define _DATE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "utils/time_utils.h"

namespace vanillapdf {
namespace semantics {

class Date : public HighLevelObject<syntax::StringObjectPtr> {
public:
	explicit Date(syntax::StringObjectPtr root);

	int32_t GetYear(void) const noexcept { return m_year; }
	int32_t GetMonth(void) const noexcept { return m_month; }
	int32_t GetDay(void) const noexcept { return m_day; }
	int32_t GetHour(void) const noexcept { return m_hour; }
	int32_t GetMinute(void) const noexcept { return m_minute; }
	int32_t GetSecond(void) const noexcept { return m_second; }
	Timezone GetTimezone(void) const noexcept { return m_timezone; }
	int32_t GetHourOffset(void) const noexcept { return m_hour_offset; }
	int32_t GetMinuteOffset(void) const noexcept { return m_minute_offset; }

	static DatePtr GetCurrentDate();

private:
	int32_t m_year = 1900;
	int32_t m_month = 1;
	int32_t m_day = 1;
	int32_t m_hour = 0;
	int32_t m_minute = 0;
	int32_t m_second = 0;
	Timezone m_timezone = Timezone::UTC;
	int32_t m_hour_offset = 0;
	int32_t m_minute_offset = 0;
};

} // semantics
} // vanillapdf

#endif /* _DATE_H */
