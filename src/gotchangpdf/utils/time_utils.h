#ifndef _TIME_UTILS_H
#define _TIME_UTILS_H

#include "utils/buffer.h"

#include <string>

namespace gotchangpdf {

enum class Timezone : char {
	UTC = 'Z',
	Later = '+',
	Earlier = '-'
};

class TimeInfo {
public:
	int GetYear(void) const noexcept { return m_year; }
	int GetMonth(void) const noexcept { return m_month; }
	int GetDay(void) const noexcept { return m_day; }
	int GetHour(void) const noexcept { return m_hour; }
	int GetMinute(void) const noexcept { return m_minute; }
	int GetSecond(void) const noexcept { return m_second; }
	Timezone GetTimezone(void) const noexcept { return m_timezone; }
	int GetHourOffset(void) const noexcept { return m_hour_offset; }
	int GetMinuteOffset(void) const noexcept { return m_minute_offset; }

	void SetYear(int value) noexcept { m_year = value; }
	void SetMonth(int value) noexcept { m_month = value; }
	void SetDay(int value) noexcept { m_day = value; }
	void SetHour(int value) noexcept { m_hour = value; }
	void SetMinute(int value) noexcept { m_minute = value; }
	void SetSecond(int value) noexcept { m_second = value; }
	void SetTimezone(Timezone value) noexcept { m_timezone = value; }
	void SetHourOffset(int value) noexcept { m_hour_offset = value; }
	void SetMinuteOffset(int value) noexcept { m_minute_offset = value; }

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

class TimeUtils {
public:
	static TimeInfo GetCurrentTime();
	static BufferPtr GetCurrentTimeString(const char * format);

private:
	TimeUtils() = default;
};

} // gotchangpdf

#endif /* _TIME_UTILS_H */
