#ifndef _VERSION_H
#define _VERSION_H

#include "exception.h"

namespace gotchangpdf
{
	enum class Version : int
	{
		PDF10 = 0,
		PDF11 = 1,
		PDF12 = 2,
		PDF13 = 3,
		PDF14 = 4,
		PDF15 = 5,
		PDF16 = 6,
		PDF17 = 7
	};

	template <Version ver>
	class RequireVersion
	{
	public:
		RequireVersion() = default;

		void OnWriteCheck(Version file_version)
		{
			if (static_cast<unsigned char>(ver) > static_cast<unsigned char>(file_version))
				throw GeneralException("Expected file version does not match");
		}
	};
}

#endif /* _VERSION_H */
