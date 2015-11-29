#ifndef _ABSTRACT_SYNTAX_TREE_H
#define _ABSTRACT_SYNTAX_TREE_H

#include "constants.h"
#include "direct_object.h"
#include "objects.h"
#include "object_stream_header.h"

#include <boost/fusion/include/adapt_struct.hpp>
//#include <boost/fusion/adapted/adt/adapt_adt.hpp>
//#include <boost/spirit/home/support/adapt_adt_attributes.hpp>
#include <boost/fusion/include/std_pair.hpp>

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::ObjectStreamHeader,
(gotchangpdf::types::integer, object_number)
(gotchangpdf::types::stream_offset, offset)
)


//BOOST_FUSION_ADAPT_STRUCT(
//gotchangpdf::BooleanObjectPtr,
//(bool, Content->_value)
//)

//BOOST_FUSION_ADAPT_STRUCT(
//gotchangpdf::IntegerObjectPtr,
//(gotchangpdf::types::integer, Content->_value)
//)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::NameObjectPtr,
(gotchangpdf::BufferPtr, Content->_value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::MixedArrayObjectPtr,
(gotchangpdf::syntax::MixedArrayObject::list_type, Content->_list)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::DictionaryObjectPtr,
(gotchangpdf::syntax::DictionaryObject::list_type, Content->_list)
)
//
//BOOST_FUSION_ADAPT_STRUCT(
//gotchangpdf::DictionaryObject,
//(gotchangpdf::DictionaryObject::list_type, _list)
//)

//BOOST_FUSION_ADAPT_STRUCT(
//gotchangpdf::RealObjectPtr,
//(gotchangpdf::types::real, Content->_value)
//)

//BOOST_FUSION_ADAPT_STRUCT(
//gotchangpdf::StreamObjectPtr,
//(gotchangpdf::DictionaryObjectPtr, Content->_header)
//(gotchangpdf::types::stream_offset, Content->_raw_data_offset)
//)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::LiteralStringObjectPtr,
(gotchangpdf::BufferPtr, Content->_value)
)

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::HexadecimalStringObjectPtr,
(gotchangpdf::BufferPtr, Content->_value)
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
(gotchangpdf::IntegerObject::value_type, const gotchangpdf::IntegerObject::value_type, obj->Value(), obj->operator=(val))
)
*/
/*
BOOST_FUSION_ADAPT_ADT(
gotchangpdf::NameObject,
(gotchangpdf::NameObject::value_type, const gotchangpdf::NameObject::value_type, obj.Value(), obj.SetName(val))
)
*/
/*
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
gotchangpdf::syntax::ast::NameObject,
(gotchangpdf::Buffer, Content->value))

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::ast::DictionaryObject,
(gotchangpdf::syntax::ast::map_type, Content->map))

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::ast::IndirectReferenceObject,
(gotchangpdf::syntax::ast::IntegerObject, Content->objNumber)
(gotchangpdf::syntax::ast::IntegerObject, Content->genNumber))

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::ast::IntegerObject,
(int, Content->value))

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::ast::RealObject,
(double, Content->value))

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::ast::StreamObject,
(gotchangpdf::syntax::ast::DictionaryObject, Content->dictionary)
(int, Content->data_offset))

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::ast::LiteralStringObject,
(gotchangpdf::Buffer, Content->value))

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::ast::HexadecimalStringObject,
(gotchangpdf::Buffer, Content->value))

BOOST_FUSION_ADAPT_STRUCT(
gotchangpdf::syntax::ast::IndirectObject,
(gotchangpdf::syntax::ast::IntegerObject, Content->objNumber)
(gotchangpdf::syntax::ast::IntegerObject, Content->genNumber)
(gotchangpdf::syntax::ast::DirectObject, Content->obj))
*/
#endif /* _ABSTRACT_SYNTAX_TREE_H */
