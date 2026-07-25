#include "precompiled.h"
#include "semantics/objects/document_info.h"
#include "semantics/objects/date.h"

#include "vanillapdf/semantics/c_document_info.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetTitle(DocumentInfoHandle* handle, StringObjectHandle** result)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputStringObjectPtr title;
        auto contains = obj->Title(title);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = title.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetAuthor(DocumentInfoHandle* handle, StringObjectHandle** result)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputStringObjectPtr author;
        auto contains = obj->Author(author);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = author.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetSubject(DocumentInfoHandle* handle, StringObjectHandle** result)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputStringObjectPtr subject;
        auto contains = obj->Subject(subject);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = subject.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetKeywords(DocumentInfoHandle* handle, StringObjectHandle** result)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputStringObjectPtr keywords;
        auto contains = obj->Keywords(keywords);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = keywords.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreator(DocumentInfoHandle* handle, StringObjectHandle** result)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputStringObjectPtr creator;
        auto contains = obj->Creator(creator);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = creator.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetProducer(DocumentInfoHandle* handle, StringObjectHandle** result)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputStringObjectPtr producer;
        auto contains = obj->Producer(producer);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = producer.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreationDate(DocumentInfoHandle* handle, DateHandle** result)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        auto contains = obj->CreationDate(date);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetModificationDate(DocumentInfoHandle* handle, DateHandle** result)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        auto contains = obj->ModificationDate(date);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetTrapped(DocumentInfoHandle* handle, DocumentTrappedType* result)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        vanillapdf::semantics::DocumentTrapped trapped;
        auto contains = obj->Trapped(trapped);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;

        switch (trapped)
        {
        case vanillapdf::semantics::DocumentTrapped::Unknown:
            *result = DocumentTrappedType_Unknown; break;
        case vanillapdf::semantics::DocumentTrapped::True:
            *result = DocumentTrappedType_True; break;
        case vanillapdf::semantics::DocumentTrapped::False:
            *result = DocumentTrappedType_False; break;
        default:
            return VANILLAPDF_ERROR_GENERAL;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_SetTitle(DocumentInfoHandle* handle, StringObjectHandle* value)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    StringObjectBase* title = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(title);

    try
    {
        obj->SetTitle(title);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_SetAuthor(DocumentInfoHandle* handle, StringObjectHandle* value)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    StringObjectBase* author = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(author);

    try
    {
        obj->SetAuthor(author);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_SetSubject(DocumentInfoHandle* handle, StringObjectHandle* value)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    StringObjectBase* subject = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(subject);

    try
    {
        obj->SetSubject(subject);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_SetKeywords(DocumentInfoHandle* handle, StringObjectHandle* value)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    StringObjectBase* keywords = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(keywords);

    try
    {
        obj->SetKeywords(keywords);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_SetCreator(DocumentInfoHandle* handle, StringObjectHandle* value)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    StringObjectBase* creator = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(creator);

    try
    {
        obj->SetCreator(creator);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_SetProducer(DocumentInfoHandle* handle, StringObjectHandle* value)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    StringObjectBase* producer = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(producer);

    try
    {
        obj->SetProducer(producer);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_SetCreationDate(DocumentInfoHandle* handle, DateHandle* value)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    Date* creation_date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(creation_date);

    try
    {
        obj->SetCreationDate(creation_date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_SetModificationDate(DocumentInfoHandle* handle, DateHandle* value)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    Date* modification_date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(modification_date);

    try
    {
        obj->SetModificationDate(modification_date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_SetTrapped(DocumentInfoHandle* handle, DocumentTrappedType value)
{
    DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        vanillapdf::semantics::DocumentTrapped trapped = vanillapdf::semantics::DocumentTrapped::Undefined;

        switch (value)
        {
        case DocumentTrappedType_Unknown:
            trapped = vanillapdf::semantics::DocumentTrapped::Unknown; break;
        case DocumentTrappedType_True:
            trapped = vanillapdf::semantics::DocumentTrapped::True; break;
        case DocumentTrappedType_False:
            trapped = vanillapdf::semantics::DocumentTrapped::False; break;
        default:
            return VANILLAPDF_ERROR_PARAMETER_VALUE;
        }

        obj->SetTrapped(trapped);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_ToUnknown(DocumentInfoHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<DocumentInfo, IUnknown, DocumentInfoHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_FromUnknown(IUnknownHandle* handle, DocumentInfoHandle** result) {
    return SafeObjectConvert<IUnknown, DocumentInfo, IUnknownHandle, DocumentInfoHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_Release(DocumentInfoHandle* handle) {
    return ObjectRelease<DocumentInfo, DocumentInfoHandle>(handle);
}
