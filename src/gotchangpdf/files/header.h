#ifndef _HEADER_H
#define _HEADER_H

#include "fwd.h"
#include "version.h"

#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace gotchangpdf
{
	namespace files
	{
		class Header : public boost::intrusive_ref_counter<Header>
		{
		public:
			friend raw::Stream& operator>> (raw::Stream& s, Header& o);

		public:
			inline void Release() const { boost::sp_adl_block::intrusive_ptr_release(this); }
			Version GetVersion(void) const;

		private:
			Version _version;
		};
	}
}

#endif /* _HEADER_H */
