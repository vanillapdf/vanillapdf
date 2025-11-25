#include "precompiled.h"

#include "semantics/objects/digital_signature.h"
#include "semantics/utils/byte_range.h"

#include "vanillapdf/semantics/c_digital_signature.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetContactInfo(DigitalSignatureHandle* handle, StringObjectHandle** result) {
    DigitalSignature* signature = reinterpret_cast<DigitalSignature*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(signature);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputStringObjectPtr direct;
        bool contains = signature->ContactInfo(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetReason(DigitalSignatureHandle* handle, StringObjectHandle** result) {
    DigitalSignature* signature = reinterpret_cast<DigitalSignature*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(signature);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputStringObjectPtr direct;
        bool contains = signature->Reason(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetLocation(DigitalSignatureHandle* handle, StringObjectHandle** result) {
    DigitalSignature* signature = reinterpret_cast<DigitalSignature*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(signature);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputStringObjectPtr direct;
        bool contains = signature->Location(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetSigningTime(DigitalSignatureHandle* handle, DateHandle** result) {
    DigitalSignature* signature = reinterpret_cast<DigitalSignature*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(signature);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputDatePtr direct;
        bool contains = signature->SigningTime(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetName(DigitalSignatureHandle* handle, StringObjectHandle** result) {
    DigitalSignature* signature = reinterpret_cast<DigitalSignature*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(signature);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputStringObjectPtr direct;
        bool contains = signature->Name(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetRevision(DigitalSignatureHandle* handle, IntegerObjectHandle** result) {
    DigitalSignature* signature = reinterpret_cast<DigitalSignature*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(signature);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputIntegerObjectPtr direct;
        bool contains = signature->Revision(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<IntegerObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetCertificate(DigitalSignatureHandle* handle, StringObjectHandle** result) {
    DigitalSignature* signature = reinterpret_cast<DigitalSignature*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(signature);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputStringObjectPtr direct;
        bool contains = signature->Certificate(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetContents(DigitalSignatureHandle* handle, HexadecimalStringObjectHandle** result) {
    DigitalSignature* signature = reinterpret_cast<DigitalSignature*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(signature);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto direct = signature->Contents();
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<HexadecimalStringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetByteRange(DigitalSignatureHandle* handle, ByteRangeCollectionHandle** result) {
    DigitalSignature* signature = reinterpret_cast<DigitalSignature*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(signature);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OuputByteRangeCollectionPtr direct;
        bool contains = signature->ByteRange(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ByteRangeCollectionHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_Release(DigitalSignatureHandle* handle) {
    return ObjectRelease<DigitalSignature, DigitalSignatureHandle>(handle);
}
