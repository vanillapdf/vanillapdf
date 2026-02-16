#include "precompiled.h"

#include "semantics/objects/date.h"
#include "utils/text_string_encoding.h"

#include "utils/time_utils.h"

#include <regex>

namespace vanillapdf {
namespace semantics {

Date::Date(syntax::StringObjectPtr root) : HighLevelObject(root) {
    auto utf8 = TextStringToUtf8(*root->GetValue());
    auto str = utf8->ToString();

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
    if (!std::regex_match(str, sm, header_regex)) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "Could not parse datetime: {}", str);
    }

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
        if (sm[7].str() == "Z" || sm[7].str() == "") {
            m_timezone = Timezone::UTC;
        } else if (sm[7].str() == "+") {
            m_timezone = Timezone::Later;
        } else if (sm[7].str() == "-") {
            m_timezone = Timezone::Earlier;
        } else {
            LOG_ERROR_AND_THROW(InvalidParameterException, "Could not parse datetime: {}",str);
        }
    }

    if (m_month < 1 || m_month > 12) throw InvalidParameterException("Month is out of range: " + std::to_string(m_month));
    if (m_day < 1 || m_day > 31) throw InvalidParameterException("Day is out of range: " + std::to_string(m_day));
    if (m_hour < 0 || m_hour > 23) throw InvalidParameterException("Hour is out of range: " + std::to_string(m_hour));
    if (m_minute < 0 || m_minute > 59) throw InvalidParameterException("Minute is out of range: " + std::to_string(m_minute));
    if (m_second < 0 || m_second > 59) throw InvalidParameterException("Second is out of range: " + std::to_string(m_second));
    if (m_hour_offset < 0 || m_hour_offset > 23) throw InvalidParameterException("Hour offset is out of range: " + std::to_string(m_hour_offset));
    if (m_minute_offset < 0 || m_minute_offset > 59) throw InvalidParameterException("Minute offset is out of range: " + std::to_string(m_minute_offset));
}

DatePtr Date::GetCurrentDate() {
    TimeInfo local_time = TimeUtils::GetCurrentTime();

    std::string formatted_time;
    Timezone timezone = local_time.GetTimezone();

    if (timezone == Timezone::UTC) {
        formatted_time = fmt::format("D:{:04d}{:02d}{:02d}{:02d}{:02d}{:02d}Z",
            local_time.GetYear(), local_time.GetMonth(), local_time.GetDay(),
            local_time.GetHour(), local_time.GetMinute(), local_time.GetSecond());
    } else if (timezone == Timezone::Earlier) {
        formatted_time = fmt::format("D:{:04d}{:02d}{:02d}{:02d}{:02d}{:02d}-{:02d}'{:02d}'",
            local_time.GetYear(), local_time.GetMonth(), local_time.GetDay(),
            local_time.GetHour(), local_time.GetMinute(), local_time.GetSecond(),
            local_time.GetHourOffset(), local_time.GetMinuteOffset());
    } else if (timezone == Timezone::Later) {
        formatted_time = fmt::format("D:{:04d}{:02d}{:02d}{:02d}{:02d}{:02d}+{:02d}'{:02d}'",
            local_time.GetYear(), local_time.GetMonth(), local_time.GetDay(),
            local_time.GetHour(), local_time.GetMinute(), local_time.GetSecond(),
            local_time.GetHourOffset(), local_time.GetMinuteOffset());
    } else {
        throw InvalidParameterException("Unknown timezone type");
    }

    auto string_object = syntax::LiteralStringObject::CreateFromDecoded(formatted_time);
    return make_deferred<Date>(string_object);
}

void Date::UpdateObject() {
    std::string formatted_time;

    if (m_timezone == Timezone::UTC) {
        formatted_time = fmt::format("D:{:04d}{:02d}{:02d}{:02d}{:02d}{:02d}Z",
            m_year, m_month, m_day, m_hour, m_minute, m_second);
    } else if (m_timezone == Timezone::Earlier) {
        formatted_time = fmt::format("D:{:04d}{:02d}{:02d}{:02d}{:02d}{:02d}-{:02d}'{:02d}'",
            m_year, m_month, m_day, m_hour, m_minute, m_second,
            m_hour_offset, m_minute_offset);
    } else if (m_timezone == Timezone::Later) {
        formatted_time = fmt::format("D:{:04d}{:02d}{:02d}{:02d}{:02d}{:02d}+{:02d}'{:02d}'",
            m_year, m_month, m_day, m_hour, m_minute, m_second,
            m_hour_offset, m_minute_offset);
    } else {
        throw InvalidParameterException("Unknown timezone type");
    }

    SetObject(syntax::LiteralStringObject::CreateFromDecoded(formatted_time));
}

void Date::SetYear(int32_t value) {
    m_year = value;
    UpdateObject();
}

void Date::SetMonth(int32_t value) {
    m_month = value;
    UpdateObject();
}

void Date::SetDay(int32_t value) {
    m_day = value;
    UpdateObject();
}

void Date::SetHour(int32_t value) {
    m_hour = value;
    UpdateObject();
}

void Date::SetMinute(int32_t value) {
    m_minute = value;
    UpdateObject();
}

void Date::SetSecond(int32_t value) {
    m_second = value;
    UpdateObject();
}

void Date::SetTimezone(Timezone value) {
    m_timezone = value;
    UpdateObject();
}

void Date::SetHourOffset(int32_t value) {
    m_hour_offset = value;
    UpdateObject();
}

void Date::SetMinuteOffset(int32_t value) {
    m_minute_offset = value;
    UpdateObject();
}

} // semantics
} // vanillapdf
