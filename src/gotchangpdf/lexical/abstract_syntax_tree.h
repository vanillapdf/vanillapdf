#ifndef _ABSTRACT_SYNTAX_TREE_H
#define _ABSTRACT_SYNTAX_TREE_H

#include "buffer.h"
#include "deferred.h"

#include <vector>
#include <map>
#include <stddef.h>

//#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/variant/variant.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace ast
		{
			struct True
			{
				mutable long references = 0;
			};

			struct False
			{
				mutable long references = 0;
			};

			struct ASTArrayObject;
			struct ASTDictionaryObject;
			struct ASTNameObject;
			struct ASTFunctionObject;
			struct ASTNullObject;
			struct ASTIndirectReferenceObject;
			struct ASTIntegerObject;
			struct ASTRealObject;
			struct ASTStreamObject;
			struct ASTLiteralStringObject;
			struct ASTHexadecimalStringObject;

			typedef Deferred<ASTArrayObject> ArrayObject;
			typedef Deferred<ASTNameObject> NameObject;
			typedef Deferred<ASTFunctionObject> FunctionObject;
			typedef Deferred<ASTDictionaryObject> DictionaryObject;
			typedef Deferred<ASTNullObject> NullObject;
			typedef Deferred<ASTIndirectReferenceObject> IndirectReferenceObject;
			typedef Deferred<ASTIntegerObject> IntegerObject;
			typedef Deferred<ASTRealObject> RealObject;
			typedef Deferred<ASTStreamObject> StreamObject;

			typedef Deferred<ASTHexadecimalStringObject> HexadecimalStringObject;
			typedef Deferred<ASTLiteralStringObject> LiteralStringObject;

			typedef boost::variant<LiteralStringObject, HexadecimalStringObject> StringObject;

			typedef boost::variant<True, False> BooleanObject;

			typedef boost::variant <
				ArrayObject,
				NameObject,
				DictionaryObject,
				FunctionObject,
				NullObject,
				BooleanObject,
				IndirectReferenceObject,
				IntegerObject,
				RealObject,
				StreamObject,
				StringObject
			> DirectObject;

			struct ASTArrayObject
			{
				std::vector<DirectObject> _array;

				mutable long references = 0;
			};

			struct ASTNameObject
			{
				struct Hasher
				{
					unsigned long operator()(const ASTNameObject& t) const
					{
						unsigned long result = 0;
						for (auto & val : t.name)
						{
							std::hash<char> hash_fn;
							result ^= hash_fn(val);
						}

						return result;
					}
				};

				Buffer name;

				mutable long references = 0;
			};

			struct ASTLiteralStringObject
			{
				Buffer value;
				mutable long references = 0;
			};

			struct ASTHexadecimalStringObject
			{
				Buffer value;
				mutable long references = 0;
			};

			struct ASTFunctionObject
			{
				mutable long references = 0;
			};

			struct ASTNullObject
			{
				mutable long references = 0;
			};

			typedef ::std::map<ast::ASTNameObject, ast::DirectObject, ast::ASTNameObject::Hasher> map_type;
			struct ASTDictionaryObject
			{
				map_type map;

				mutable long references = 0;
			};

			struct ASTIndirectReferenceObject
			{
				IntegerObject objNumber;
				IntegerObject genNumber;

				mutable long references = 0;
			};

			struct ASTIntegerObject
			{
				int value;
				mutable long references = 0;
			};

			struct ASTRealObject
			{
				double value;
				mutable long references = 0;
			};

			struct ASTStreamObject
			{
				DictionaryObject dictionary;
				Buffer data;
				mutable long references = 0;
			};

			struct ASTIndirectObject
			{
				IntegerObject objNumber;
				IntegerObject genNumber;

				DirectObject obj;

				mutable long references = 0;
			};

			typedef Deferred<ASTIndirectObject> IndirectObject;
		}
	}
}

namespace ast = gotchangpdf::lexical::ast;

BOOST_FUSION_ADAPT_STRUCT(gotchangpdf::lexical::ast::DirectObject, /**/)
BOOST_FUSION_ADAPT_STRUCT(gotchangpdf::lexical::ast::FunctionObject, /**/)
BOOST_FUSION_ADAPT_STRUCT(gotchangpdf::lexical::ast::NullObject, /**/)

	BOOST_FUSION_ADAPT_STRUCT(
	gotchangpdf::lexical::ast::ArrayObject,
	(std::vector<gotchangpdf::lexical::ast::DirectObject>, Content->_array))

	BOOST_FUSION_ADAPT_STRUCT(
	gotchangpdf::lexical::ast::NameObject,
	(gotchangpdf::Buffer, Content->value))

	BOOST_FUSION_ADAPT_STRUCT(
	gotchangpdf::lexical::ast::DictionaryObject,
	(gotchangpdf::lexical::ast::map_type, Content->map))

	BOOST_FUSION_ADAPT_STRUCT(
	gotchangpdf::lexical::ast::IndirectReferenceObject,
	(gotchangpdf::lexical::ast::IntegerObject, Content->objNumber)
	(gotchangpdf::lexical::ast::IntegerObject, Content->genNumber))

	BOOST_FUSION_ADAPT_STRUCT(
	gotchangpdf::lexical::ast::IntegerObject,
	(float, Content->value))

	BOOST_FUSION_ADAPT_STRUCT(
	gotchangpdf::lexical::ast::RealObject,
	(double, Content->value))

	BOOST_FUSION_ADAPT_STRUCT(
	gotchangpdf::lexical::ast::StreamObject,
	(gotchangpdf::lexical::ast::DictionaryObject, Content->dictionary)
	(gotchangpdf::Buffer, Content->data))

	BOOST_FUSION_ADAPT_STRUCT(
	gotchangpdf::lexical::ast::LiteralStringObject,
	(gotchangpdf::Buffer, Content->value))

	BOOST_FUSION_ADAPT_STRUCT(
	gotchangpdf::lexical::ast::HexadecimalStringObject,
	(gotchangpdf::Buffer, Content->value))

	BOOST_FUSION_ADAPT_STRUCT(
	gotchangpdf::lexical::ast::IndirectObject,
	(gotchangpdf::lexical::ast::IntegerObject, Content->objNumber)
	(gotchangpdf::lexical::ast::IntegerObject, Content->genNumber)
	(gotchangpdf::lexical::ast::DirectObject, Content->obj))

#endif /* _ABSTRACT_SYNTAX_TREE_H */
