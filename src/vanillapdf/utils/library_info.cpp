#include "precompiled.h"

#include "utils/library_info.h"

#ifndef VANILLAPDF_VERSION_MAJOR
	#define VANILLAPDF_VERSION_MAJOR 0
#endif /* VANILLAPDF_VERSION_MAJOR */

#ifndef VANILLAPDF_VERSION_MINOR
	#define VANILLAPDF_VERSION_MINOR 0
#endif /* VANILLAPDF_VERSION_MINOR */

#ifndef VANILLAPDF_VERSION_PATCH
	#define VANILLAPDF_VERSION_PATCH 0
#endif /* VANILLAPDF_VERSION_PATCH */

#ifndef VANILLAPDF_VERSION_BUILD
	#define VANILLAPDF_VERSION_BUILD 0
#endif /* VANILLAPDF_VERSION_BUILD */

#ifndef VANILLAPDF_BUILD_DAY
	// MSVC cannot parse date time 1.1.1970 00:00:00,
	// because it takes timezone into account.
	// Lets shift the default build day!
	#define VANILLAPDF_BUILD_DAY 2
#endif /* VANILLAPDF_BUILD_DAY */

#ifndef VANILLAPDF_BUILD_MONTH
	#define VANILLAPDF_BUILD_MONTH 1
#endif /* VANILLAPDF_BUILD_MONTH */

#ifndef VANILLAPDF_BUILD_YEAR
	#define VANILLAPDF_BUILD_YEAR 1970
#endif /* VANILLAPDF_BUILD_YEAR */

namespace vanillapdf {

int LibraryInfo::MajorVersion() noexcept {
	const int MAJOR_VERSION = VANILLAPDF_VERSION_MAJOR;
	return MAJOR_VERSION;
}

int LibraryInfo::MinorVersion() noexcept {
	const int MINOR_VERSION = VANILLAPDF_VERSION_MINOR;
	return MINOR_VERSION;
}

int LibraryInfo::PatchVersion() noexcept {
	const int PATCH_VERSION = VANILLAPDF_VERSION_PATCH;
	return PATCH_VERSION;
}

int LibraryInfo::BuildVersion() noexcept {
	const int BUILD_VERSION = VANILLAPDF_VERSION_BUILD;
	return BUILD_VERSION;
}

const char * LibraryInfo::Author() noexcept {
	return "Vanilla.PDF";
}

int LibraryInfo::BuildDay() noexcept {
	const int BUILD_DAY = VANILLAPDF_BUILD_DAY;
	return BUILD_DAY;
}

int LibraryInfo::BuildMonth() noexcept {
	const int BUILD_MONTH = VANILLAPDF_BUILD_MONTH;
	return BUILD_MONTH;
}

int LibraryInfo::BuildYear() noexcept {
	const int BUILD_YEAR = VANILLAPDF_BUILD_YEAR;
	return BUILD_YEAR;
}

} // vanillapdf
