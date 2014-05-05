#ifndef _CROSS_REFERENCE_TABLE_H
#define _CROSS_REFERENCE_TABLE_H

#include "Fwd.h"
#include "BaseObjects/Object.h"
#include "Character.h"
#include "BaseObjects/IntegerObject.h"
#include "IndirectObject.h"
#include "Constants.h"

#include "boost/intrusive_ptr.hpp"

#include <vector>

namespace Pdf
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

		friend Lexical::Parser& operator>> (Lexical::Parser& s, CrossReferenceInfo& o);

	private:
		Type _type;
		std::vector<Entry> _entries;

		mutable long _intrusive_ref_count;

		friend void ::boost::intrusive_ptr_add_ref(CrossReferenceInfo*);
		friend void ::boost::intrusive_ptr_release(CrossReferenceInfo*);

		CrossReferenceInfo::Entry ReadEntry(Lexical::Parser& s, int objNumber);
	};
}

#endif /* _CROSS_REFERENCE_TABLE_H */
