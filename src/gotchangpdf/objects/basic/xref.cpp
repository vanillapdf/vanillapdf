#include "xref.h"
#include "integer_object.h"
#include "Parser.h"
#include "exception.h"
#include "export.h"

#include <cassert>
#include <sstream>

namespace gotchangpdf
{
	using namespace std;
	using namespace lexical;
	using namespace exceptions;

	void Xref::Add(const Entry& e) { _entries.push_back(e); }
	int Xref::Size(void) const { return _entries.size(); }
	Xref::Entry Xref::At(int at) const { return _entries.at(at); }

	Xref::Xref() : _entries(), _type(Xref::Type::TABLE) {}

	Xref::Entry Xref::ReadEntry(lexical::Parser& s, int objNumber)
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

		Xref::Entry result;

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

		result._reference = ObjectReferenceWrapper<IndirectObject>(new IndirectObject(s.file(), objNumber, IntegerObject(number), IntegerObject(offset)));

		return result;
	}

	void Xref::Release() { boost::intrusive_ptr_release(this); }

	Xref::~Xref() {}

	Parser& operator>>(Parser& s, Xref& o)
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

	Xref::Entry::Entry() : _in_use(false) {}

	Xref::Entry::Entry(IXref::IEntry entry) : _reference(entry._reference), _in_use(entry._in_use) {}

	#pragma region DllInterface

	IXref::IEntry::IEntry() : _in_use(false), _reference(nullptr) {}
	IXref::IEntry::IEntry(IIndirectObject *obj, bool in_use) : _in_use(in_use), _reference(obj) {}

	void IXref::Add(const IEntry& e)
	{
		auto removed = const_cast<IXref*>(this);
		auto obj = reinterpret_cast<Xref*>(removed);

		obj->Add(Xref::Entry(e));
	}

	int IXref::Size(void) const
	{
		auto removed = const_cast<IXref*>(this);
		auto obj = reinterpret_cast<Xref*>(removed);

		return obj->Size();
	}

	IXref::IEntry IXref::At(int at) const
	{
		auto removed = const_cast<IXref*>(this);
		auto obj = reinterpret_cast<Xref*>(removed);

		Xref::Entry entry = obj->At(at);
		IXref::IEntry result(entry._reference.AddRefGet(), entry._in_use);

		return result;
	}

	void IXref::Release()
	{
		auto removed = const_cast<IXref*>(this);
		auto obj = reinterpret_cast<Xref*>(removed);

		obj->Release();
	}

	IXref::~IXref() {}

	#pragma endregion
}
