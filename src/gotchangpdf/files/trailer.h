#ifndef _TRAILER_H
#define _TRAILER_H

#include "fwd.h"
#include "deferred.h"
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
			DictionaryObjectPtr dictionary() const;

		private:
			types::stream_offset _xref_offset = std::_BADOFF;
			DictionaryObjectPtr _dictionary;
		};
	}
}

#endif /* _TRAILER_H */
