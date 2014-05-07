#include "ArrayObject.h"
#include "Lexical/Parser.h"
#include "DictionaryObject.h"
#include "StringObject.h"
#include "RealObject.h"
#include "IntegerObject.h"
#include "IndirectObjectReference.h"
#include "Exception.h"
#include "Export.h"

namespace Pdf
{
	using namespace Lexical;

	ArrayObject::ArrayObject() : Object(Object::Type::ArrayObject) {}
	long ArrayObject::Size(void) const { return _list.size(); }

	IObject* ArrayObject::At(long at) const
	{
		auto item = _list[at];

		Pdf::Object *ptr = item.get();
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
	Pdf::ArrayObject* arr = reinterpret_cast<Pdf::ArrayObject*>(handle);
	return reinterpret_cast<ObjectHandle>(arr->At(at));
}

GOTCHANG_PDF_API int CALLING_CONVENTION ArrayObject_Size(ArrayObjectHandle handle)
{
	Pdf::ArrayObject* arr = reinterpret_cast<Pdf::ArrayObject*>(handle);
	return arr->Size();
}

GOTCHANG_PDF_API void CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle handle)
{
	Pdf::ArrayObject* obj = reinterpret_cast<Pdf::ArrayObject*>(handle);
	obj->Release();
}
