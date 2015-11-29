#ifndef _VERSION_H
#define _VERSION_H

#include "exception.h"

namespace gotchangpdf
{
	enum class Version : unsigned char
	{
		PDF10,
		PDF11,
		PDF12,
		PDF13,
		PDF14,
		PDF15,
		PDF16,
		PDF17
	};

	template <Version ver>
	class RequireVersion
	{
	public:
		RequireVersion() = default;

		void OnWriteCheck(Version file_version)
		{
			if (static_cast<unsigned char>(ver) > static_cast<unsigned char>(current))
				throw exceptions::Exception("TODO");
		}
	};
}

#endif /* _VERSION_H */
