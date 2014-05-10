#ifndef _HEADER_H
#define _HEADER_H

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

			Version GetVersion(void) const;

		private:
			Version _version;

			mutable long _intrusive_ref_count;

			template <typename T>
			friend void ::boost::intrusive_ptr_add_ref(T*);

			template <typename T>
			friend void ::boost::intrusive_ptr_release(T*);
		};
	}
}

#endif /* _HEADER_H */
