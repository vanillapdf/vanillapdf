#ifndef _HEADER_H
#define _HEADER_H

#include "basic_stream.h"
#include "version.h"

#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace gotchangpdf
{
	namespace files
	{
		class Header : public boost::intrusive_ref_counter<Header>
		{
		public:
			friend basic::Stream& operator>> (basic::Stream& s, Header& o);

			Version GetVersion(void) const;

		private:
			Version _version;
		};
	}
}

#endif /* _HEADER_H */
