#ifndef _HEADER_H
#define _HEADER_H

#include "syntax_fwd.h"
#include "unknown_interface.h"
#include "version.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class Header : public IUnknown
		{
		public:
			friend Stream& operator>> (Stream& s, Header& o);

		public:
			Version GetVersion(void) const;

		private:
			Version _version;
		};
	}
}

#endif /* _HEADER_H */
