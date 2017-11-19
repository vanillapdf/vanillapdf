#include "precompiled.h"

#include "semantics/objects/digital_signature.h"

#include "vanillapdf/semantics/c_digital_signature.h"
#include "implementation/c_helper.h"

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

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetDate(DigitalSignatureHandle* handle, DateHandle** result) {
	DigitalSignature* signature = reinterpret_cast<DigitalSignature*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(signature);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		OutputDatePtr direct;
		bool contains = signature->Date(direct);
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

VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_GetOffset(ByteRangeHandle* handle, IntegerObjectHandle** result) {
	ByteRange* range = reinterpret_cast<ByteRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(range);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto direct = range->ByteOffset();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_GetLength(ByteRangeHandle* handle, IntegerObjectHandle** result) {
	ByteRange* range = reinterpret_cast<ByteRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(range);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto direct = range->Length();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Size(ByteRangeCollectionHandle* handle, size_type* result) {
	ByteRangeCollection* collection = reinterpret_cast<ByteRangeCollection*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = collection->Size();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_At(ByteRangeCollectionHandle* handle, size_type at, ByteRangeHandle** result) {
	ByteRangeCollection* collection = reinterpret_cast<ByteRangeCollection*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto direct = collection->At(at);
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<ByteRangeHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_Release(ByteRangeHandle* handle) {
	return ObjectRelease<ByteRange, ByteRangeHandle*>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Release(ByteRangeCollectionHandle* handle) {
	return ObjectRelease<ByteRangeCollection, ByteRangeCollectionHandle*>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_Release(DigitalSignatureHandle* handle) {
	return ObjectRelease<DigitalSignature, DigitalSignatureHandle*>(handle);
}
