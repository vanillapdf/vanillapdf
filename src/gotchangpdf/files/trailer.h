#ifndef _TRAILER_H
#define _TRAILER_H

#include "fwd.h"
#include "smart_ptr.h"
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
			types::stream_offset xref_offset() const;
			SmartPtr<DictionaryObject> dictionary() const;

		private:
			types::stream_offset _xref_offset = std::_BADOFF;
			SmartPtr<DictionaryObject> _dictionary = SmartPtr<DictionaryObject>(new DictionaryObject());
		};
	}
}

#endif /* _TRAILER_H */
