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
	long ArrayObject::Size(void) const { return _list.size(); }

	IObject* ArrayObject::At(long at) const
	{
		auto item = _list[at];

		gotchangpdf::Object *ptr = item.Get();
		boost::intrusive_ptr_add_ref(ptr);

		return reinterpret_cast<IObject*>(ptr);
	}

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
	return reinterpret_cast<ObjectHandle>(arr->At(at));
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
