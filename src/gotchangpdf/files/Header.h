#ifndef _HEADER_H
#define _HEADER_H

#include "token.h"
#include "intrusive.h"
#include "basic_stream.h"
#include "version.h"

namespace gotchangpdf
{
	namespace files
	{
		class Header
		{
		public:
			friend basic::Stream& operator>> (basic::Stream& s, Header& o);

		private:
			Version _version;

			mutable long _intrusive_ref_count;
			friend void ::boost::intrusive_ptr_add_ref(Header*);
			friend void ::boost::intrusive_ptr_release(Header*);
		};
	}
}

#endif /* _HEADER_H */
