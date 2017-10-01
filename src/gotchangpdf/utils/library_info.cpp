#include "precompiled.h"

#include "utils/library_info.h"

#ifndef GOTCHANG_PDF_VERSION_MAJOR
	#define GOTCHANG_PDF_VERSION_MAJOR 0
#endif /* GOTCHANG_PDF_VERSION_MAJOR */

#ifndef GOTCHANG_PDF_VERSION_MINOR
	#define GOTCHANG_PDF_VERSION_MINOR 0
#endif /* GOTCHANG_PDF_VERSION_MINOR */

#ifndef GOTCHANG_PDF_VERSION_PATCH
	#define GOTCHANG_PDF_VERSION_PATCH 0
#endif /* GOTCHANG_PDF_VERSION_PATCH */

#ifndef GOTCHANG_PDF_VERSION_BUILD
	#define GOTCHANG_PDF_VERSION_BUILD 0
#endif /* GOTCHANG_PDF_VERSION_BUILD */

#ifndef GOTCHANG_PDF_BUILD_DAY
	// MSVC cannot parse date time 1.1.1970 00:00:00,
	// because it takes timezone into account.
	// Lets shift the default build day!
	#define GOTCHANG_PDF_BUILD_DAY 2
#endif /* GOTCHANG_PDF_BUILD_DAY */

#ifndef GOTCHANG_PDF_BUILD_MONTH
	#define GOTCHANG_PDF_BUILD_MONTH 1
#endif /* GOTCHANG_PDF_BUILD_MONTH */

#ifndef GOTCHANG_PDF_BUILD_YEAR
	#define GOTCHANG_PDF_BUILD_YEAR 1970
#endif /* GOTCHANG_PDF_BUILD_YEAR */

namespace gotchangpdf {

int LibraryInfo::MajorVersion() noexcept {
	const int MAJOR_VERSION = GOTCHANG_PDF_VERSION_MAJOR;
	return MAJOR_VERSION;
}

int LibraryInfo::MinorVersion() noexcept {
	const int MINOR_VERSION = GOTCHANG_PDF_VERSION_MINOR;
	return MINOR_VERSION;
}

int LibraryInfo::PatchVersion() noexcept {
	const int PATCH_VERSION = GOTCHANG_PDF_VERSION_PATCH;
	return PATCH_VERSION;
}

int LibraryInfo::BuildVersion() noexcept {
	const int BUILD_VERSION = GOTCHANG_PDF_VERSION_BUILD;
	return BUILD_VERSION;
}

const char * LibraryInfo::Author() noexcept {
	return "Juraj Zikmund";
}

int LibraryInfo::BuildDay() noexcept {
	const int BUILD_DAY = GOTCHANG_PDF_BUILD_DAY;
	return BUILD_DAY;
}

int LibraryInfo::BuildMonth() noexcept {
	const int BUILD_MONTH = GOTCHANG_PDF_BUILD_MONTH;
	return BUILD_MONTH;
}

int LibraryInfo::BuildYear() noexcept {
	const int BUILD_YEAR = GOTCHANG_PDF_BUILD_YEAR;
	return BUILD_YEAR;
}

} // gotchangpdf
