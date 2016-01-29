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
			Version GetVersion(void) const { return _version; }
			void SetVersion(Version version) { _version = version; }

		private:
			Version _version;
		};
	}
}

#endif /* _HEADER_H */
