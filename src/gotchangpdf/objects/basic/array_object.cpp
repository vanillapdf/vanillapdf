#include "array_object.h"
#include "parser.h"
#include "exception.h"

#include "boolean.h"
#include "dictionary_object.h"
#include "function.h"
#include "integer_object.h"
#include "name_object.h"
#include "null_object.h"
#include "real_object.h"
#include "stream_object.h"
#include "string_object.h"
#include "indirect_object.h"
#include "indirect_object_reference.h"

#include "c_array_object.h"

#include <algorithm>

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
		}

		s.ReadTokenWithType(lexical::Token::Type::ARRAY_END);

		s.LexicalSettingsPop();
		return s;
	}
	/*
#define NIGGA_MORE_VARIABLE(name, type)			vector<ObjectReferenceWrapper<type>> name
#define NIGGA_MORE_LAMBDA(name, type)			auto to_##name = [](ObjectReferenceWrapper<Object> obj)->ObjectReferenceWrapper<type>{ return dynamic_wrapper_cast<type>(obj); }
#define NIGGA_MORE_TRANSFORM(name, func)		name.resize(list.size()); \
												transform(list.begin(), list.end(), name.begin(), to_##func)

	ObjectReferenceWrapper<ArrayObjectBase> ArrayObjectBase::ReadArray(Parser& s)
	{
		std::vector<ObjectReferenceWrapper<Object>> list;

		s.LexicalSettingsPush();
		auto settings = s.LexicalSettingsGet();
		settings->skip.push_back(lexical::Token::Type::EOL);

		if (s.PeekTokenType() == lexical::Token::Type::ARRAY_BEGIN)
			s.ReadToken();

		auto type = s.peekObject()->GetType();
		bool is_single = true;

		while (s.PeekTokenType() != lexical::Token::Type::ARRAY_END)
		{
			auto val = s.readObject();
			list.push_back(val);

			is_single &= val->GetType() == type;
		}

		s.ReadTokenWithType(lexical::Token::Type::ARRAY_END);

		s.LexicalSettingsPop();

		if (is_single)
		{
			NIGGA_MORE_VARIABLE(arrays, ArrayObjectBase);
			NIGGA_MORE_LAMBDA(array, ArrayObjectBase);

			NIGGA_MORE_VARIABLE(bools, Boolean);
			NIGGA_MORE_LAMBDA(bool, Boolean);

			NIGGA_MORE_VARIABLE(dicts, DictionaryObject);
			NIGGA_MORE_LAMBDA(dict, DictionaryObject);

			NIGGA_MORE_VARIABLE(funcs, Function);
			NIGGA_MORE_LAMBDA(func, Function);

			NIGGA_MORE_VARIABLE(integers, IntegerObject);
			NIGGA_MORE_LAMBDA(integer, IntegerObject);

			NIGGA_MORE_VARIABLE(names, NameObject);
			NIGGA_MORE_LAMBDA(name, NameObject);

			NIGGA_MORE_VARIABLE(nulls, NullObject);
			NIGGA_MORE_LAMBDA(null, NullObject);

			NIGGA_MORE_VARIABLE(reals, RealObject);
			NIGGA_MORE_LAMBDA(real, RealObject);

			NIGGA_MORE_VARIABLE(streams, StreamObject);
			NIGGA_MORE_LAMBDA(stream, StreamObject);

			NIGGA_MORE_VARIABLE(hexs, HexadecimalString);
			NIGGA_MORE_LAMBDA(hex, HexadecimalString);

			NIGGA_MORE_VARIABLE(literals, LiteralString);
			NIGGA_MORE_LAMBDA(literal, LiteralString);

			NIGGA_MORE_VARIABLE(refs, IndirectObjectReference);
			NIGGA_MORE_LAMBDA(ref, IndirectObjectReference);

			NIGGA_MORE_VARIABLE(indirects, IndirectObject);
			NIGGA_MORE_LAMBDA(indirect, IndirectObject);

			switch (type)
			{
				case Object::Type::Unknown:
					throw Exception("FIXME: Are your really trying to return unknown type??");
			case Object::Type::ArrayObject:
				NIGGA_MORE_TRANSFORM(arrays, array);
				return ObjectReferenceWrapper<ArrayObject<ArrayObjectBase>>(new ArrayObject<ArrayObjectBase>(arrays));
			case Object::Type::Boolean:
				NIGGA_MORE_TRANSFORM(bools, bool);
				return ObjectReferenceWrapper<ArrayObject<Boolean>>(new ArrayObject<Boolean>(bools));
			case Object::Type::DictionaryObject:
				NIGGA_MORE_TRANSFORM(dicts, dict);
				return ObjectReferenceWrapper<ArrayObject<DictionaryObject>>(new ArrayObject<DictionaryObject>(dicts));
			case Object::Type::Function:
				NIGGA_MORE_TRANSFORM(funcs, func);
				return ObjectReferenceWrapper<ArrayObject<Function>>(new ArrayObject<Function>(funcs));
			case Object::Type::IntegerObject:
				NIGGA_MORE_TRANSFORM(integers, integer);
				return ObjectReferenceWrapper<ArrayObject<IntegerObject>>(new ArrayObject<IntegerObject>(integers));
			case Object::Type::NameObject:
				NIGGA_MORE_TRANSFORM(names, name);
				return ObjectReferenceWrapper<ArrayObject<NameObject>>(new ArrayObject<NameObject>(names));
			case Object::Type::NullObject:
				NIGGA_MORE_TRANSFORM(nulls, null);
				return ObjectReferenceWrapper<ArrayObject<NullObject>>(new ArrayObject<NullObject>(nulls));
			case Object::Type::RealObject:
				NIGGA_MORE_TRANSFORM(reals, real);
				return ObjectReferenceWrapper<ArrayObject<RealObject>>(new ArrayObject<RealObject>(reals));
			case Object::Type::StreamObject:
				NIGGA_MORE_TRANSFORM(streams, stream);
				return ObjectReferenceWrapper<ArrayObject<StreamObject>>(new ArrayObject<StreamObject>(streams));
			case Object::Type::HexadecimalString:
				NIGGA_MORE_TRANSFORM(hexs, hex);
				return ObjectReferenceWrapper<ArrayObject<HexadecimalString>>(new ArrayObject<HexadecimalString>(hexs));
			case Object::Type::LiteralString:
				NIGGA_MORE_TRANSFORM(literals, literal);
				return ObjectReferenceWrapper<ArrayObject<LiteralString>>(new ArrayObject<LiteralString>(literals));
			case Object::Type::IndirectObjectReference:
				NIGGA_MORE_TRANSFORM(refs, ref);
				return ObjectReferenceWrapper<ArrayObject<IndirectObjectReference>>(new ArrayObject<IndirectObjectReference>(refs));
			case Object::Type::IndirectObject:
				NIGGA_MORE_TRANSFORM(indirects, indirect);
				return ObjectReferenceWrapper<ArrayObject<IndirectObject>>(new ArrayObject<IndirectObject>(indirects));
			default:
				assert(false);
				throw Exception("FIXME: Unknown object type");
			}
		}
		else
			return ObjectReferenceWrapper<MixedArrayObject>(new MixedArrayObject(list));
	}

	ArrayObjectBase::~ArrayObjectBase() {}
	*/
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
