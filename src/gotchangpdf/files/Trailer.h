#ifndef _TRAILER_H
#define _TRAILER_H

#include "Fwd.h"
#include "DictionaryObject.h"
#include "Constants.h"

namespace Pdf
{
	namespace Files
	{
		class Trailer
		{
		public:
			Trailer();

			friend Streams::Lexical::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, Trailer& o);
			streamOffsetValueType xref_offset() const;
			boost::intrusive_ptr<DictionaryObject> dictionary() const;

		private:
			streamOffsetValueType _xref_offset;
			boost::intrusive_ptr<DictionaryObject> _dictionary;

			mutable long _intrusive_ref_count;
			friend void ::boost::intrusive_ptr_add_ref(Trailer*);
			friend void ::boost::intrusive_ptr_release(Trailer*);
		};
	}
}

#endif /* _TRAILER_H */
