#ifndef _CROSS_REFERENCE_TABLE_H
#define _CROSS_REFERENCE_TABLE_H

#include "BaseObjects/Object.h"
#include "BaseObjects/Character.h"
#include "BaseObjects/IntegerObject.h"
#include "IndirectObject.h"
#include "Constants.h"

#include "boost/intrusive_ptr.hpp"

#include <vector>

namespace Pdf
{
	class CrossReferenceTable;

	namespace Lexical
	{
		class Parser;
	}
}

namespace boost
{
	void intrusive_ptr_add_ref(Pdf::CrossReferenceTable* obj);
	void intrusive_ptr_release(Pdf::CrossReferenceTable* obj);
}

namespace Pdf
{
	class CrossReferenceTable
	{
	public:
		struct Entry
		{
			boost::intrusive_ptr<IndirectObject> _reference;
			bool _in_use;
		};

		CrossReferenceTable();

		void Add(const Entry& e);
		int Size(void) const;
		Entry At(int at) const;

		friend Lexical::Parser& operator>> (Lexical::Parser& s, CrossReferenceTable& o);

	private:
		std::vector<Entry> _table;

		mutable long _intrusive_ref_count;

		friend void ::boost::intrusive_ptr_add_ref(CrossReferenceTable*);
		friend void ::boost::intrusive_ptr_release(CrossReferenceTable*);

		CrossReferenceTable::Entry ReadEntry(Lexical::Parser& s, int objNumber);
	};
}

#endif /* _CROSS_REFERENCE_TABLE_H */
