#ifndef _HEADER_H
#define _HEADER_H

#include "Lexical/Token.h"
#include "Intrusive.h"
#include "streams/basic/BasicStream.h"

namespace Pdf
{
	namespace Files
	{
		class Header
		{
		public:
			friend Streams::Basic::Stream& operator>> (Streams::Basic::Stream& s, Header& o);

		private:
			int _majorVersion;
			int _minorVersion;

			mutable long _intrusive_ref_count;
			friend void ::boost::intrusive_ptr_add_ref(Header*);
			friend void ::boost::intrusive_ptr_release(Header*);
		};
	}
}

#endif /* _HEADER_H */
