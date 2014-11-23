#ifndef _ABSTRACT_SYNTAX_TREE_H
#define _ABSTRACT_SYNTAX_TREE_H

#include "constants.h"
#include "direct_object.h"

//#include <boost/fusion/include/adapt_struct.hpp
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

BOOST_FUSION_ADAPT_STRUCT(gotchangpdf::DirectObject, /**/)
BOOST_FUSION_ADAPT_STRUCT(gotchangpdf::StringObjectPtr, /**/)
BOOST_FUSION_ADAPT_STRUCT(gotchangpdf::FunctionObjectPtr, /**/)
BOOST_FUSION_ADAPT_STRUCT(gotchangpdf::NullObjectPtr, /**/)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::IntegerObjectPtr,
(gotchangpdf::types::integer, get()->_value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::NameObject,
(gotchangpdf::Buffer, _value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::NameObjectPtr,
(gotchangpdf::Buffer, get()->_value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::MixedArrayObjectPtr,
(gotchangpdf::MixedArrayObject::value_type, get()->_list)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::DictionaryObjectPtr,
(gotchangpdf::DictionaryObject::value_type, get()->_list)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::IndirectObjectReferencePtr,
(gotchangpdf::types::integer, get()->_obj_number)
(gotchangpdf::types::ushort, get()->_gen_number)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::RealObjectPtr,
(gotchangpdf::types::real, get()->_value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::StreamObjectPtr,
(gotchangpdf::DictionaryObject::value_type, get()->_dictionary)
(gotchangpdf::types::stream_offset, get()->_raw_data_offset)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::LiteralStringPtr,
(gotchangpdf::Buffer, get()->_value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::HexadecimalStringPtr,
(gotchangpdf::Buffer, get()->_value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::IndirectObject,
(gotchangpdf::types::integer, get()->_obj_number)
(gotchangpdf::types::integer, get()->_gen_number)
(gotchangpdf::DirectObject, get()->_reference)
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
//(gotchangpdf::SmartPtr<gotchangpdf::Object>, gotchangpdf::SmartPtr<gotchangpdf::Object>, obj.GetObject(), obj.SetObject(val))
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
(gotchangpdf::SmartPtr<gotchangpdf::Object>, gotchangpdf::SmartPtr<gotchangpdf::Object>, obj.GetObject(), obj.SetObject(val))
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
