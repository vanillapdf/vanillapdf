#include "precompiled.h"

#include "utils/time_utils.h"

#include <chrono>
#include <ctime>
#include <mutex>

namespace vanillapdf {

TimeInfo TimeUtils::GetCurrentTime() {
	// localtime function may not be thread-safe
	static std::mutex m_chrono;

	auto today = std::chrono::system_clock::now();
	auto rawtime = std::chrono::system_clock::to_time_t(today);

	std::lock_guard<std::mutex> locker(m_chrono);
	auto gm_time = std::gmtime(&rawtime);
	if (gm_time == nullptr) {
		throw GeneralException("Could not get local time");
	}

	TimeInfo result;
	result.SetYear(gm_time->tm_year + 1900);
	result.SetMonth(gm_time->tm_mon + 1);
	result.SetDay(gm_time->tm_mday);
	result.SetHour(gm_time->tm_hour);
	result.SetMinute(gm_time->tm_min);
	result.SetSecond(gm_time->tm_sec);
	result.SetTimezone(Timezone::UTC);

	//auto gm_time = std::gmtime(&rawtime);
	//if (gm_time == nullptr) {
	//	throw GeneralException("Could not get gm time");
	//}

	//int hour_offset = result.GetHour() - gm_time->tm_hour;
	//int minute_offset = result.GetMinute() - gm_time->tm_min;
	//if (hour_offset != 0 || minute_offset != 0) {
	//	Timezone timezone = Timezone::Later;
	//	if (hour_offset < 0) {
	//		timezone = Timezone::Earlier;
	//	}

	//	int absolute_hour_offset = std::abs(hour_offset);
	//	int absolute_minute_offset = std::abs(minute_offset);

	//	result.SetTimezone(timezone);
	//	result.SetHourOffset(absolute_hour_offset);
	//	result.SetMinuteOffset(absolute_minute_offset);
	//}

	return result;
}

BufferPtr TimeUtils::GetCurrentTimeString(const char * format) {
	TimeInfo time = GetCurrentTime();

	tm time_info;
	time_info.tm_year = time.GetYear() - 1900;
	time_info.tm_mon = time.GetMonth() - 1;
	time_info.tm_mday = time.GetDay();
	time_info.tm_hour = time.GetHour();
	time_info.tm_min = time.GetMinute();
	time_info.tm_sec = time.GetSecond();
	time_info.tm_hour = time.GetHour();

	char buffer[128] = { 0 };
	size_t result = strftime(buffer, sizeof(buffer), format, &time_info);
	if (result == 0) {
		throw GeneralException("Could not convert localtime to string");
	}

	return make_deferred_container<Buffer>(buffer);
}

} // vanillapdf
