#ifndef _XREF_H
#define _XREF_H

#include "fwd.h"
#include "indirect_object.h"

#include <vector>

namespace gotchangpdf
{
	namespace files
	{
		struct XrefEntry
		{
			Deferred<IndirectObject> reference;
			bool in_use;
		};

		class Xref : public std::vector<XrefEntry>, public boost::intrusive_ref_counter<Xref>
		{
		public:
			enum class Type : unsigned char
			{
				TABLE = 0,
				STREAM
			};

			void Release();

			friend lexical::Parser& operator>> (lexical::Parser& s, Xref& o);

		private:
			Type _type = Type::TABLE;
		};
	}
}

#endif /* _XREF_H */
