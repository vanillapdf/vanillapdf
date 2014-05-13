#ifndef _XREF_H
#define _XREF_H

#include "fwd.h"
#include "indirect_object.h"

#include <vector>

namespace gotchangpdf
{
	namespace files
	{
		class Xref
		{
		public:
			enum class Type : unsigned char
			{
				TABLE = 0,
				STREAM
			};

			struct Entry
			{
				ObjectReferenceWrapper<IndirectObject> _reference;
				bool _in_use;
			};

			void Add(const Entry& e);
			int Size(void) const;
			Entry At(int at) const;
			void Release();

			friend lexical::Parser& operator>> (lexical::Parser& s, Xref& o);

		private:
			Type _type = Type::TABLE;
			std::vector<Entry> _entries;

			mutable long _intrusive_ref_count;

			template <typename T>
			friend void ::boost::intrusive_ptr_add_ref(T*);

			template <typename T>
			friend void ::boost::intrusive_ptr_release(T*);
		};
	}
}

#endif /* _XREF_H */
