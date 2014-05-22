#include "array_object.h"
#include "parser.h"
#include "token.h"

#include "c_array_object.h"

namespace gotchangpdf
{
	using namespace lexical;
	using namespace exceptions;
	using namespace std;

	lexical::Parser& operator>>(lexical::Parser& s, ArrayObject& o)
	{
		s.LexicalSettingsPush();
		auto settings = s.LexicalSettingsGet();
		settings->skip.push_back(lexical::Token::Type::EOL);

		if (s.PeekTokenType() == lexical::Token::Type::ARRAY_BEGIN)
			s.ReadToken();

		while (s.PeekTokenType() != lexical::Token::Type::ARRAY_END)
		{
			auto val = s.readObject();
			o._list.push_back(val);
			val->SetParent(&o);
		}

		s.ReadTokenWithType(lexical::Token::Type::ARRAY_END);

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
