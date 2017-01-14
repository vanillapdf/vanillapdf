#include "precompiled.h"

#include "utils/library_info.h"

#ifndef GOTCHANG_PDF_VERSION_MAJOR
	#define GOTCHANG_PDF_VERSION_MAJOR 0
#endif

#ifndef GOTCHANG_PDF_VERSION_MINOR
	#define GOTCHANG_PDF_VERSION_MINOR 0
#endif

#ifndef GOTCHANG_PDF_VERSION_PATCH
	#define GOTCHANG_PDF_VERSION_PATCH 0
#endif 

namespace gotchangpdf
{
	int LibraryInfo::MajorVersion() noexcept
	{
		const int MAJOR_VERSION = GOTCHANG_PDF_VERSION_MAJOR;
		return MAJOR_VERSION;
	}

	int LibraryInfo::MinorVersion() noexcept
	{
		const int MINOR_VERSION = GOTCHANG_PDF_VERSION_MINOR;
		return MINOR_VERSION;
	}

	int LibraryInfo::PatchVersion() noexcept
	{
		const int PATCH_VERSION = GOTCHANG_PDF_VERSION_PATCH;
		return PATCH_VERSION;
	}

	const char * LibraryInfo::Author() noexcept
	{
		return "Juraj Zikmund";
	}
}
