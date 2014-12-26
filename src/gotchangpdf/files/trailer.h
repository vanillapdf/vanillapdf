#ifndef _TRAILER_H
#define _TRAILER_H

#include "fwd.h"
#include "unknown_interface.h"
#include "constants.h"

namespace gotchangpdf
{
	namespace files
	{
		class Trailer : public IUnknown
		{
		public:
			friend lexical::ReverseStream& operator>> (lexical::ReverseStream& s, Trailer& o);

		public:
			types::stream_offset GetXrefOffset() const { return _xref_offset; }
			DictionaryObjectPtr GetDictionary() const { return _dictionary; }
			void SetDictionary(DictionaryObjectPtr dict) { _dictionary = dict; }

		private:
			types::stream_offset _xref_offset = std::_BADOFF;
			DictionaryObjectPtr _dictionary;
		};
	}
}

#endif /* _TRAILER_H */
