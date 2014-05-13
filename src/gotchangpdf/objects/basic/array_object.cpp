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

	ObjectReferenceWrapper<Object> ArrayObject::At(unsigned int at) const { return _list.at(at); }
	ObjectReferenceWrapper<Object> ArrayObject::operator[](unsigned int i) const { return _list[i]; }

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
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION ArrayObject_At(ArrayObjectHandle handle, int at)
{
	gotchangpdf::ArrayObject* arr = reinterpret_cast<gotchangpdf::ArrayObject*>(handle);
	return reinterpret_cast<ObjectHandle>(arr->At(at).AddRefGet());
}

GOTCHANG_PDF_API int CALLING_CONVENTION ArrayObject_Size(ArrayObjectHandle handle)
{
	gotchangpdf::ArrayObject* arr = reinterpret_cast<gotchangpdf::ArrayObject*>(handle);
	return arr->Size();
}

GOTCHANG_PDF_API void CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle handle)
{
	gotchangpdf::ArrayObject* obj = reinterpret_cast<gotchangpdf::ArrayObject*>(handle);
	obj->Release();
}
