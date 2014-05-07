#ifndef _DATE_H
#define _DATE_H

#include "string_object.h"

// format (D:YYYYMMDDHHmmSSOHH'mm)

namespace gotchangpdf
{
	struct Date
	{
	public:
		static void FromString(const LiteralString&);
		static LiteralString& ToLiteralString();

		enum class UTCRelation : unsigned char
		{
			equal = 'Z',
			earlier = '-',
			later = '+'
		};

		int year,
			month,
			day,
			hour,
			minute,
			second,
			offset_hours,
			offset_minutes;

		UTCRelation relation;
	};

	void Date::FromString(const LiteralString& str)
	{
		// TODO
		static const char header_fmt[] = "D:%4d%2d%2d%2d%2d%2d%c%2d'%2d";
		//int result = sscanf_s(resultStr.c_str(), header_fmt, &o._majorVersion, &o._minorVersion);
	}

	LiteralString& Date::ToLiteralString()
	{
		// TODO
		LiteralString result = LiteralString("D:YYYYMMDDHHmmSSOHH'mm");
		return result;
	}
}

#endif /* _DATE_H */
