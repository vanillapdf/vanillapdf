#ifndef _XREF_H
#define _XREF_H

#include "fwd.h"
#include "constants.h"

#include <vector>

#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace gotchangpdf
{
	namespace files
	{
		struct XrefEntry
		{
			IndirectObjectPtr reference;
			types::integer obj_number;
			types::ushort gen_number;
			types::stream_offset offset;
			bool in_use;
			bool initialized;
		};

		class Xref : public std::vector<XrefEntry>, public boost::intrusive_ref_counter<Xref>
		{
		public:
			enum class Type : unsigned char
			{
				TABLE = 0,
				STREAM
			};

			inline void Release() { boost::sp_adl_block::intrusive_ptr_release(this); }

			friend lexical::Parser& operator>> (lexical::Parser& s, Xref& o);

		private:
			Type _type = Type::TABLE;
		};
	}
}

#endif /* _XREF_H */
