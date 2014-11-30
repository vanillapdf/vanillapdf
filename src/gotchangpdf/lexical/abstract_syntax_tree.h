#ifndef _ABSTRACT_SYNTAX_TREE_H
#define _ABSTRACT_SYNTAX_TREE_H

#include "deferred.h"
#include "constants.h"
#include "direct_object.h"
#include "objects.h"

//#include <boost/fusion/include/adapt_struct.hpp
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
/*
namespace gotchangpdf
{
	namespace lexical
	{
		namespace ast
		{
			class IntegerObjectTest
			{
			public:
				types::integer value = 0;

				mutable long references = 0;
			};

			typedef Deferred<IntegerObjectTest> IntegerObjectTestPtr;
		}
	}
}
*/
BOOST_FUSION_ADAPT_STRUCT(gotchangpdf::DirectObject, /**/)
BOOST_FUSION_ADAPT_STRUCT(gotchangpdf::StringObjectPtr, /**/)
BOOST_FUSION_ADAPT_STRUCT(gotchangpdf::FunctionObjectPtr, /**/)
BOOST_FUSION_ADAPT_STRUCT(gotchangpdf::NullObjectPtr, /**/)
/*
BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::lexical::ast::IntegerObjectTestPtr,
(gotchangpdf::types::integer, Content->value)
)
*/
BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::IntegerObjectPtr,
(gotchangpdf::types::integer, Content->_value)
)
/*
BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::NameObject,
(gotchangpdf::Buffer, _value)
)
*/
BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::NameObjectPtr,
(gotchangpdf::Buffer, Content->_value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::MixedArrayObjectPtr,
(gotchangpdf::MixedArrayObject::value_type, Content->_list)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::DictionaryObjectPtr,
(gotchangpdf::DictionaryObject::value_type, Content->_list)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::IndirectObjectReferencePtr,
(gotchangpdf::IntegerObjectPtr, Content->_obj_number)
(gotchangpdf::IntegerObjectPtr, Content->_gen_number)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::RealObjectPtr,
(gotchangpdf::types::real, Content->_value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::StreamObjectPtr,
(gotchangpdf::DictionaryObject::value_type, Content->_dictionary)
(gotchangpdf::types::stream_offset, Content->_raw_data_offset)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::LiteralStringPtr,
(gotchangpdf::Buffer, Content->_value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::HexadecimalStringPtr,
(gotchangpdf::Buffer, Content->_value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::IndirectObjectPtr,
(gotchangpdf::IntegerObjectPtr, Content->_obj_number)
(gotchangpdf::IntegerObjectPtr, Content->_gen_number)
(gotchangpdf::DirectObject, Content->_reference)
)


/*
BOOST_FUSION_ADAPT_ADT(
gotchangpdf::MixedArrayObjectPtr,
(gotchangpdf::MixedArrayObject::value_type, gotchangpdf::MixedArrayObject::value_type, obj->GetList(), obj->SetList(val))
)

BOOST_FUSION_ADAPT_ADT(
gotchangpdf::DictionaryObjectPtr,
(gotchangpdf::DictionaryObject::value_type, gotchangpdf::DictionaryObject::value_type, obj->GetMap(), obj->SetMap(val))
)
*/
/*
BOOST_FUSION_ADAPT_ADT(
gotchangpdf::IntegerObjectPtr,
(gotchangpdf::IntegerObject::value_type, gotchangpdf::IntegerObject::value_type, obj->Value(), obj->operator=(val))
)
*/
/*
BOOST_FUSION_ADAPT_ADT(
gotchangpdf::NameObjectPtr,
(gotchangpdf::NameObject::value_type, gotchangpdf::NameObject::value_type, obj->Value(), obj->SetName(val))
)

BOOST_FUSION_ADAPT_ADT(
gotchangpdf::IndirectObject,
(gotchangpdf::IntegerObject, gotchangpdf::IntegerObject, obj.GetObjectNumber(), obj.SetObjectNumber(val))
//(gotchangpdf::IntegerObject, gotchangpdf::IntegerObject, obj.GetGenerationNumber(), obj.SetGenerationNumber(val))
//(gotchangpdf::streamOffsetValueType, gotchangpdf::streamOffsetValueType, obj.GetOffset(), obj->SetOffset(val))
//(gotchangpdf::Deferred<gotchangpdf::Object>, gotchangpdf::Deferred<gotchangpdf::Object>, obj.GetObject(), obj.SetObject(val))
)
*/
/*
BOOST_FUSION_ADAPT_ADT(
gotchangpdf::IndirectObjectReferencePtr,
(gotchangpdf::types::integer, gotchangpdf::types::integer, obj->GetObjectNumber(), obj->SetObjectNumber(val))
(gotchangpdf::types::ushort, gotchangpdf::types::ushort, obj->GetGenerationNumber(), obj->SetGenerationNumber(val))
)
*/
/*
BOOST_FUSION_ADAPT_ADT(
gotchangpdf::IndirectObject,
(gotchangpdf::types::integer, gotchangpdf::types::integer, obj.GetObjectNumber(), obj.SetObjectNumber(val))
(gotchangpdf::types::ushort, gotchangpdf::types::ushort, obj.GetGenerationNumber(), obj.SetGenerationNumber(val))
//(gotchangpdf::streamOffsetValueType, gotchangpdf::streamOffsetValueType, obj.GetOffset(), obj->SetOffset(val))
(gotchangpdf::Deferred<gotchangpdf::Object>, gotchangpdf::Deferred<gotchangpdf::Object>, obj.GetObject(), obj.SetObject(val))
)

BOOST_FUSION_ADAPT_ADT(
gotchangpdf::IndirectObjectReferencePtr,
(gotchangpdf::types::integer, gotchangpdf::types::integer, obj->GetObjectNumber(), obj->SetObjectNumber(val))
(gotchangpdf::types::ushort, gotchangpdf::types::ushort, obj->GetGenerationNumber(), obj->SetGenerationNumber(val))
)
*/
/*
BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::MixedArrayObjectPtr,
(std::vector<gotchangpdf::DirectObject>, Content->_array))
*/
/*
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
(int, Content->value))

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::lexical::ast::RealObject,
(double, Content->value))

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::lexical::ast::StreamObject,
(gotchangpdf::lexical::ast::DictionaryObject, Content->dictionary)
(int, Content->data_offset))

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
*/
#endif /* _ABSTRACT_SYNTAX_TREE_H */
