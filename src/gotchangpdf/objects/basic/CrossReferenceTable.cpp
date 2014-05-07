#include "CrossReferenceTable.h"
#include "Lexical/Parser.h"
#include "Exception.h"
#include "Export.h"

#include <cassert>
#include <sstream>

namespace gotchangpdf
{
	using namespace std;
	using namespace lexical;
	using namespace exceptions;

	void CrossReferenceInfo::Add(const Entry& e) { _entries.push_back(e); }
	int CrossReferenceInfo::Size(void) const { return _entries.size(); }
	CrossReferenceInfo::Entry CrossReferenceInfo::At(int at) const { return _entries.at(at); }

	CrossReferenceInfo::CrossReferenceInfo() : _entries(), _type(CrossReferenceInfo::Type::TABLE) {}

	CrossReferenceInfo::Entry CrossReferenceInfo::ReadEntry(lexical::Parser& s, int objNumber)
	{
		// TODO space
		Character sp1, sp2, key, eol1, eol2;
		Token offset, number;
		s >> offset >> sp1 >> number >> sp2 >> key >> eol1 >> eol2;

		if (!(eol1 == Character::WhiteSpace::SPACE && eol2 == Character::WhiteSpace::CARRIAGE_RETURN) &&
			!(eol1 == Character::WhiteSpace::SPACE && eol2 == Character::WhiteSpace::LINE_FEED) &&
			!(eol1 == Character::WhiteSpace::CARRIAGE_RETURN && eol2 == Character::WhiteSpace::LINE_FEED))
		{
			throw Exception("End of line marker was not found in xref table entry");
		}

		static const Character IN_USE = Character('n');
		static const Character NOT_IN_USE = Character('f');

		CrossReferenceInfo::Entry result;

		if (key == IN_USE)
			result._in_use = true;
		else if (key == NOT_IN_USE)
			result._in_use = false;
		else
		{
			stringstream buffer;
			buffer << "Key in XRef table is either of " << IN_USE.Value() << " or " << NOT_IN_USE.Value();

			throw Exception(buffer.str());
		}

		result._reference = boost::intrusive_ptr<IndirectObject>(new IndirectObject(s.file(), objNumber, IntegerObject(number), IntegerObject(offset)));

		return result;
	}

	Parser& operator>>(Parser& s, CrossReferenceInfo& o)
	{
		// XRef stream
		if (s.PeekTokenType() == Token::Type::INTEGER_OBJECT)
		{
			auto obj = s.readObject();

			// TODO process

			return s;
		}

		s.ReadTokenWithType(Token::Type::XREF_MARKER);
		s.ReadTokenWithType(Token::Type::EOL);

		while (s.PeekTokenType() != Token::Type::TRAILER)
		{
			IntegerObject revision, numberOfObjects;
			s >> revision >> numberOfObjects;

			s.ReadTokenWithType(Token::Type::EOL);

			for (int i = 0; i < numberOfObjects; ++i)
			{
				auto entry = o.ReadEntry(s, static_cast<int>(revision + i));
				o.Add(entry);
			}
		}

		return s;
	}
}

using namespace gotchangpdf;

GOTCHANG_PDF_API int CALLING_CONVENTION Xref_Size(XrefHandle handle)
{
	CrossReferenceInfo* table = reinterpret_cast<CrossReferenceInfo*>(handle);
	return table->Size();
}

GOTCHANG_PDF_API XrefEntryHandle CALLING_CONVENTION Xref_At(XrefHandle handle, int at)
{
	CrossReferenceInfo* table = reinterpret_cast<CrossReferenceInfo*>(handle);
	CrossReferenceInfo::Entry* entry = new CrossReferenceInfo::Entry(table->At(at));
	return reinterpret_cast<XrefEntryHandle>(entry);
}

GOTCHANG_PDF_API void CALLING_CONVENTION Xref_Release(XrefHandle handle)
{
	CrossReferenceInfo* table = reinterpret_cast<CrossReferenceInfo*>(handle);
	boost::intrusive_ptr_release(table);
}

GOTCHANG_PDF_API void CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle)
{
	CrossReferenceInfo::Entry* entry = reinterpret_cast<CrossReferenceInfo::Entry*>(handle);
	delete entry;
}

GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION XrefEntry_Reference(XrefEntryHandle handle)
{
	CrossReferenceInfo::Entry* entry = reinterpret_cast<CrossReferenceInfo::Entry*>(handle);

	gotchangpdf::IndirectObject *ptr = entry->_reference.get();
	boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<IndirectObjectHandle>(ptr);
}

GOTCHANG_PDF_API int CALLING_CONVENTION XrefEntry_In_Use(XrefEntryHandle handle)
{
	CrossReferenceInfo::Entry* entry = reinterpret_cast<CrossReferenceInfo::Entry*>(handle);

	if (entry->_in_use)
		return GOTCHANG_PDF_RV_TRUE;
	else
		return GOTCHANG_PDF_RV_FALSE;
}
