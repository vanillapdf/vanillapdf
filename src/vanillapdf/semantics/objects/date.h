#ifndef _DATE_H
#define _DATE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "utils/time_utils.h"

namespace vanillapdf {
namespace semantics {

class Date : public HighLevelObject<syntax::StringObjectPtr> {
public:
	Date() = default;
	explicit Date(syntax::StringObjectPtr root);

	int32_t GetYear(void) const noexcept { return m_year; }
	void SetYear(int32_t value) noexcept { m_year = value; }

	int32_t GetMonth(void) const noexcept { return m_month; }
	void SetMonth(int32_t value) noexcept { m_month = value; }

	int32_t GetDay(void) const noexcept { return m_day; }
	void SetDay(int32_t value) noexcept { m_day = value; }

	int32_t GetHour(void) const noexcept { return m_hour; }
	void SetHour(int32_t value) noexcept { m_hour = value; }

	int32_t GetMinute(void) const noexcept { return m_minute; }
	void SetMinute(int32_t value) noexcept { m_minute = value; }

	int32_t GetSecond(void) const noexcept { return m_second; }
	void SetSecond(int32_t value) noexcept { m_second = value; }

	Timezone GetTimezone(void) const noexcept { return m_timezone; }
	void SetTimezone(Timezone value) noexcept { m_timezone = value; }

	int32_t GetHourOffset(void) const noexcept { return m_hour_offset; }
	void SetHourOffset(int32_t value) noexcept { m_hour_offset = value; }

	int32_t GetMinuteOffset(void) const noexcept { return m_minute_offset; }
	void SetMinuteOffset(int32_t value) noexcept { m_minute_offset = value; }

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
