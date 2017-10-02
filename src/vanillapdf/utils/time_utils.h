#ifndef _TIME_UTILS_H
#define _TIME_UTILS_H

#include "utils/buffer.h"

#include <string>

namespace vanillapdf {

enum class Timezone : char {
	UTC = 'Z',
	Later = '+',
	Earlier = '-'
};

class TimeInfo {
public:
	int32_t GetYear(void) const noexcept { return m_year; }
	int32_t GetMonth(void) const noexcept { return m_month; }
	int32_t GetDay(void) const noexcept { return m_day; }
	int32_t GetHour(void) const noexcept { return m_hour; }
	int32_t GetMinute(void) const noexcept { return m_minute; }
	int32_t GetSecond(void) const noexcept { return m_second; }
	Timezone GetTimezone(void) const noexcept { return m_timezone; }
	int32_t GetHourOffset(void) const noexcept { return m_hour_offset; }
	int32_t GetMinuteOffset(void) const noexcept { return m_minute_offset; }

	void SetYear(int32_t value) noexcept { m_year = value; }
	void SetMonth(int32_t value) noexcept { m_month = value; }
	void SetDay(int32_t value) noexcept { m_day = value; }
	void SetHour(int32_t value) noexcept { m_hour = value; }
	void SetMinute(int32_t value) noexcept { m_minute = value; }
	void SetSecond(int32_t value) noexcept { m_second = value; }
	void SetTimezone(Timezone value) noexcept { m_timezone = value; }
	void SetHourOffset(int32_t value) noexcept { m_hour_offset = value; }
	void SetMinuteOffset(int32_t value) noexcept { m_minute_offset = value; }

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

class TimeUtils {
public:
	static TimeInfo GetCurrentTime();
	static BufferPtr GetCurrentTimeString(const char * format);

private:
	TimeUtils() = default;
};

} // vanillapdf

#endif /* _TIME_UTILS_H */
