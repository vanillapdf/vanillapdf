#ifndef _TRAILER_H
#define _TRAILER_H

#include "fwd.h"
#include "constants.h"

#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace gotchangpdf
{
	namespace files
	{
		class Trailer : public boost::intrusive_ref_counter<Trailer>
		{
		public:
			friend lexical::ReverseStream& operator>> (lexical::ReverseStream& s, Trailer& o);

		public:
			types::stream_offset GetXrefOffset() const { return _xref_offset; }
			DictionaryObjectPtr GetDictionary() const { return _dictionary; }
			void SetDictionary(DictionaryObjectPtr dict) { _dictionary = dict; }

			inline void Release() const { boost::sp_adl_block::intrusive_ptr_release(this); }

		private:
			types::stream_offset _xref_offset = std::_BADOFF;
			DictionaryObjectPtr _dictionary;
		};
	}
}

#endif /* _TRAILER_H */
