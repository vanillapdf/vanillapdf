#include "CrossReferenceTable.h"
#include "Lexical/Parser.h"
#include "Exception.h"
#include "Export.h"

#include <cassert>
#include <string>

namespace Pdf
{
	using namespace std;
	using namespace Pdf::Lexical;

	void CrossReferenceTable::Add(const Entry& e)
	{
		_table.push_back(e);
	}

	int CrossReferenceTable::Size(void) const { return _table.size(); }
	CrossReferenceTable::Entry CrossReferenceTable::At(int at) const { return _table.at(at); }

	CrossReferenceTable::CrossReferenceTable() : _table() {}

	CrossReferenceTable::Entry CrossReferenceTable::ReadEntry(Lexical::Parser& s, int objNumber)
	{
		// TODO space
		Character sp, key;
		Token offset, number, eol;
		s >> offset >> number >> sp >> key;
		s >> eol;

		if (eol.type() != Token::Type::EOL)
			throw Exception("End of line marker was not found in xref table entry");

		static const Character IN_USE = Character('n');
		static const Character NOT_IN_USE = Character('f');

		CrossReferenceTable::Entry result;

		if (key.value() == IN_USE)
			result._in_use = true;
		else if (key.value() == NOT_IN_USE)
			result._in_use = false;
		else
			throw Exception(string("Key in XRef table is either of ") + static_cast<char>(IN_USE.value()) + string(" or ") + static_cast<char>(NOT_IN_USE.value()));

		result._reference = boost::intrusive_ptr<IndirectObject>(new IndirectObject(s.file(), objNumber, IntegerObject(number), IntegerObject(offset)));

		return result;
	}

	Parser& operator>>(Parser& s, CrossReferenceTable& o)
	{
		s.ReadTokenWithType(Token::Type::XREF_MARKER);
		s.ReadTokenWithType(Token::Type::EOL);

		IntegerObject revision, numberOfObjects;
		s >> revision >> numberOfObjects;

		s.ReadTokenWithType(Token::Type::EOL);

		for (int i = 0; i < numberOfObjects; ++i)
		{
			auto entry = o.ReadEntry(s, static_cast<int>(revision + i));
			o.Add(entry);
		}

		return s;
	}
}

using namespace Pdf;

GOTCHANG_PDF_API int CALLING_CONVENTION Xref_Size(XrefHandle handle)
{
	CrossReferenceTable* table = reinterpret_cast<CrossReferenceTable*>(handle);
	return table->Size();
}

GOTCHANG_PDF_API XrefEntryHandle CALLING_CONVENTION Xref_At(XrefHandle handle, int at)
{
	CrossReferenceTable* table = reinterpret_cast<CrossReferenceTable*>(handle);
	CrossReferenceTable::Entry* entry = new CrossReferenceTable::Entry(table->At(at));
	return reinterpret_cast<XrefEntryHandle>(entry);
}

GOTCHANG_PDF_API void CALLING_CONVENTION Xref_Release(XrefHandle handle)
{
	CrossReferenceTable* table = reinterpret_cast<CrossReferenceTable*>(handle);
	boost::intrusive_ptr_release(table);
}

GOTCHANG_PDF_API void CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle)
{
	CrossReferenceTable::Entry* entry = reinterpret_cast<CrossReferenceTable::Entry*>(handle);
	delete entry;
}

GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION XrefEntry_Reference(XrefEntryHandle handle)
{
	CrossReferenceTable::Entry* entry = reinterpret_cast<CrossReferenceTable::Entry*>(handle);

	Pdf::IndirectObject *ptr = entry->_reference.get();
	boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<IndirectObjectHandle>(ptr);
}

GOTCHANG_PDF_API int CALLING_CONVENTION XrefEntry_In_Use(XrefEntryHandle handle)
{
	CrossReferenceTable::Entry* entry = reinterpret_cast<CrossReferenceTable::Entry*>(handle);

	if (entry->_in_use)
		return GOTCHANG_PDF_RV_TRUE;
	else
		return GOTCHANG_PDF_RV_FALSE;
}
