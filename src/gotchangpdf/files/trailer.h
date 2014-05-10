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
		class Trailer
		{
		public:
			Trailer();

			friend lexical::ReverseStream& operator>> (lexical::ReverseStream& s, Trailer& o);
			streamOffsetValueType xref_offset() const;
			ObjectReferenceWrapper<DictionaryObject> dictionary() const;

		private:
			streamOffsetValueType _xref_offset;
			ObjectReferenceWrapper<DictionaryObject> _dictionary;

			mutable long _intrusive_ref_count;

			template <typename T>
			friend void ::boost::intrusive_ptr_add_ref(T*);

			template <typename T>
			friend void ::boost::intrusive_ptr_release(T*);
		};
	}
}

#endif /* _TRAILER_H */
