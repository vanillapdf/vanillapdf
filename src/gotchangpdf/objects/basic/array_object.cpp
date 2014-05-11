#include "array_object.h"
#include "Lexical/Parser.h"
#include "dictionary_object.h"
#include "string_object.h"
#include "real_object.h"
#include "integer_object.h"
#include "indirect_object_reference.h"
#include "exception.h"
#include "export.h"

namespace gotchangpdf
{
	using namespace lexical;

	ArrayObject::ArrayObject() : Object(Object::Type::ArrayObject) {}
	int ArrayObject::Size(void) const { return _list.size(); }

	ObjectReferenceWrapper<Object> ArrayObject::operator[](unsigned int i) const { return _list[i]; }

	ObjectReferenceWrapper<Object> ArrayObject::At(unsigned int at) const { return _list.at(at); }

	ArrayObject::~ArrayObject() {}

	Parser& operator>>(Parser& s, ArrayObject& o)
	{
		s.LexicalSettingsPush();
		auto settings = s.LexicalSettingsGet();
		settings->skip.push_back(Token::Type::EOL);

		if (s.PeekTokenType() == Token::Type::ARRAY_BEGIN)
			s.ReadToken();

		while (s.PeekTokenType() != Token::Type::ARRAY_END)
		{
			auto val = s.readObject();
			o._list.push_back(val);
		}

		s.ReadTokenWithType(Token::Type::ARRAY_END);

		s.LexicalSettingsPop();
		return s;
	}

	#pragma region DllInterface

	int IArrayObject::Size(void) const
	{
		auto removed = const_cast<IArrayObject*>(this);
		auto obj = reinterpret_cast<ArrayObject*>(removed);

		return obj->Size();
	}

	IObject* IArrayObject::At(unsigned int at) const
	{
		auto removed = const_cast<IArrayObject*>(this);
		auto obj = reinterpret_cast<ArrayObject*>(removed);

		return reinterpret_cast<IObject*>(obj->At(at).AddRefGet());
	}

	IArrayObject::~IArrayObject() {};

	#pragma endregion
}
