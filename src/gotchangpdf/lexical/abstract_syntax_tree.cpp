#include "abstract_syntax_tree.h"

#include "name_object.h"
#include "array_object.h"
#include "boolean.h"
#include "dictionary_object.h"
#include "integer_object.h"
#include "indirect_object.h"
#include "indirect_object_reference.h"
#include "name_object.h"
#include "null_object.h"
#include "real_object.h"
#include "string_object.h"
#include "stream_object.h"
#include "function.h"

#include "exception.h"

#include <algorithm>
/*
#define NIGGA_MORE_VARIABLE(name, type)			vector<ObjectReferenceWrapper<type>> name
#define NIGGA_MORE_LAMBDA(name, type)			auto to_##name = [](ObjectAST* obj)->ObjectReferenceWrapper<type>{ return dynamic_wrapper_cast<type>(obj->Value()); }
#define NIGGA_MORE_TRANSFORM(name, func)		name.resize(_list.size()); \
	transform(_list.begin(), _list.end(), name.begin(), to_##func)

namespace gotchangpdf
{
	namespace lexical
	{
		using namespace exceptions;
		using namespace std;

		void SemanticAnalyzer::Analyze(ParseTree& tree)
		{
			DictionaryObjectAST *dict = dynamic_cast<DictionaryObjectAST*>(&tree);
			auto type_found = dict->_list.find(R"(/Type)");
			if (type_found == dict->_list.end())
				return;

			ObjectAST *type = type_found->second;
			NameObjectAST *type_parsed = dynamic_cast<NameObjectAST*>(type);
			if (nullptr == type_parsed)
				throw new Exception();

			if (type_parsed->_value.value().ToString() != R"(/Pages)")
				return;

			auto kids_found = dict->_list.find(R"(/Kids)");
			if (kids_found == dict->_list.end())
				throw new Exception();

			ObjectAST *kids = kids_found->second;
			ArrayObjectAST *kids_parsed = dynamic_cast<ArrayObjectAST*>(kids);
			if (nullptr == kids_parsed)
				throw new Exception();

			kids_parsed->_single_typed = true;
			kids_parsed->_single_type = Object::Type::IndirectObjectReference;
		}

		ObjectReferenceWrapper<Object> ArrayObjectAST::Value() const
		{
			if (_single_typed)
			{
				NIGGA_MORE_VARIABLE(arrays, ArrayObject);
				NIGGA_MORE_LAMBDA(array, ArrayObject);

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

				switch (_single_type)
				{
				case Object::Type::Unknown:
					throw Exception("FIXME: Are your really trying to return unknown type??");
				case Object::Type::ArrayObject:
					NIGGA_MORE_TRANSFORM(arrays, array);
					return ObjectReferenceWrapper<ArrayObject<ArrayObject>>(new ArrayObject<ArrayObject>(arrays));
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
			{
				NIGGA_MORE_VARIABLE(objs, Object);
				NIGGA_MORE_LAMBDA(obj, Object);
				NIGGA_MORE_TRANSFORM(objs, obj);

				return ObjectReferenceWrapper<ArrayObject>(new ArrayObject(objs));
			}
		}
	}
}
*/
