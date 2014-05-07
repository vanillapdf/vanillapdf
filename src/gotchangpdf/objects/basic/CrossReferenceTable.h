#ifndef _CROSS_REFERENCE_TABLE_H
#define _CROSS_REFERENCE_TABLE_H

#include "Fwd.h"
#include "Object.h"
#include "Character.h"
#include "IntegerObject.h"
#include "IndirectObject.h"
#include "Constants.h"

#include "boost/intrusive_ptr.hpp"

#include <vector>

namespace gotchangpdf
{
	class CrossReferenceInfo
	{
	public:
		enum class Type : unsigned char
		{
			TABLE = 0,
			STREAM
		};

		struct Entry
		{
			boost::intrusive_ptr<IndirectObject> _reference;
			bool _in_use;
		};

		CrossReferenceInfo();

		void Add(const Entry& e);
		int Size(void) const;
		Entry At(int at) const;

		friend lexical::Parser& operator>> (lexical::Parser& s, CrossReferenceInfo& o);

	private:
		Type _type;
		std::vector<Entry> _entries;

		mutable long _intrusive_ref_count;

		friend void ::boost::intrusive_ptr_add_ref(CrossReferenceInfo*);
		friend void ::boost::intrusive_ptr_release(CrossReferenceInfo*);

		CrossReferenceInfo::Entry ReadEntry(lexical::Parser& s, int objNumber);
	};
}

#endif /* _CROSS_REFERENCE_TABLE_H */
