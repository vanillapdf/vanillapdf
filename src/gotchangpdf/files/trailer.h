#ifndef _TRAILER_H
#define _TRAILER_H

#include "fwd.h"
#include "object_reference_wrapper.h"
#include "dictionary_object.h"
#include "constants.h"

namespace gotchangpdf
{
	namespace files
	{
		class Trailer : public boost::intrusive_ref_counter<Trailer>
		{
		public:
			friend lexical::ReverseStream& operator>> (lexical::ReverseStream& s, Trailer& o);
			streamOffsetValueType xref_offset() const;
			ObjectReferenceWrapper<DictionaryObject> dictionary() const;

		private:
			streamOffsetValueType _xref_offset = 0;
			ObjectReferenceWrapper<DictionaryObject> _dictionary = ObjectReferenceWrapper<DictionaryObject>(new DictionaryObject());
		};
	}
}

#endif /* _TRAILER_H */
