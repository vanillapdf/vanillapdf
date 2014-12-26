#ifndef _HEADER_H
#define _HEADER_H

#include "fwd.h"
#include "unknown_interface.h"
#include "version.h"

namespace gotchangpdf
{
	namespace files
	{
		class Header : public IUnknown
		{
		public:
			friend raw::Stream& operator>> (raw::Stream& s, Header& o);

		public:
			Version GetVersion(void) const;

		private:
			Version _version;
		};
	}
}

#endif /* _HEADER_H */
