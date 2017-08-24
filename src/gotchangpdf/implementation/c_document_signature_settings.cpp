#include "precompiled.h"

#include "semantics/utils/document_signature_settings.h"

#include "gotchangpdf/c_document_signature_settings.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetDigest(DocumentSignatureSettingsHandle handle, MessageDigestAlgorithmType* result) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		switch (obj->GetDigest()) {
			case MessageDigestAlgorithm::None:
				*result = MessageDigestAlgorithmType_None; break;
			case MessageDigestAlgorithm::MDNULL:
				*result = MessageDigestAlgorithmType_MDNULL; break;
			case MessageDigestAlgorithm::MD2:
				*result = MessageDigestAlgorithmType_MD2; break;
			case MessageDigestAlgorithm::MD4:
				*result = MessageDigestAlgorithmType_MD4; break;
			case MessageDigestAlgorithm::MD5:
				*result = MessageDigestAlgorithmType_MD5; break;
			case MessageDigestAlgorithm::SHA1:
				*result = MessageDigestAlgorithmType_SHA1; break;
			case MessageDigestAlgorithm::SHA224:
				*result = MessageDigestAlgorithmType_SHA224; break;
			case MessageDigestAlgorithm::SHA256:
				*result = MessageDigestAlgorithmType_SHA256; break;
			case MessageDigestAlgorithm::SHA384:
				*result = MessageDigestAlgorithmType_SHA384; break;
			case MessageDigestAlgorithm::SHA512:
				*result = MessageDigestAlgorithmType_SHA512; break;
			case MessageDigestAlgorithm::MDC2:
				*result = MessageDigestAlgorithmType_MDC2; break;
			case MessageDigestAlgorithm::RIPEMD160:
				*result = MessageDigestAlgorithmType_RIPEMD160; break;
			case MessageDigestAlgorithm::WHIRLPOOL:
				*result = MessageDigestAlgorithmType_WHIRLPOOL; break;
			default:
				return GOTCHANG_PDF_ERROR_GENERAL;
		}

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetDigest(DocumentSignatureSettingsHandle handle, MessageDigestAlgorithmType value) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try {
		MessageDigestAlgorithm algorithm = MessageDigestAlgorithm::None;
		switch (value) {
			case MessageDigestAlgorithmType_None:
				algorithm = MessageDigestAlgorithm::None; break;
			case MessageDigestAlgorithmType_MDNULL:
				algorithm = MessageDigestAlgorithm::MDNULL; break;
			case MessageDigestAlgorithmType_MD2:
				algorithm = MessageDigestAlgorithm::MD2; break;
			case MessageDigestAlgorithmType_MD4:
				algorithm = MessageDigestAlgorithm::MD4; break;
			case MessageDigestAlgorithmType_MD5:
				algorithm = MessageDigestAlgorithm::MD5; break;
			case MessageDigestAlgorithmType_SHA1:
				algorithm = MessageDigestAlgorithm::SHA1; break;
			case MessageDigestAlgorithmType_SHA224:
				algorithm = MessageDigestAlgorithm::SHA224; break;
			case MessageDigestAlgorithmType_SHA256:
				algorithm = MessageDigestAlgorithm::SHA256; break;
			case MessageDigestAlgorithmType_SHA384:
				algorithm = MessageDigestAlgorithm::SHA384; break;
			case MessageDigestAlgorithmType_SHA512:
				algorithm = MessageDigestAlgorithm::SHA512; break;
			case MessageDigestAlgorithmType_MDC2:
				algorithm = MessageDigestAlgorithm::MDC2; break;
			case MessageDigestAlgorithmType_RIPEMD160:
				algorithm = MessageDigestAlgorithm::RIPEMD160; break;
			case MessageDigestAlgorithmType_WHIRLPOOL:
				algorithm = MessageDigestAlgorithm::WHIRLPOOL; break;
			default:
				return GOTCHANG_PDF_ERROR_GENERAL;
		}

		obj->SetDigest(algorithm);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetSigningKey(DocumentSignatureSettingsHandle handle, SigningKeyHandle* result) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		OutputPointer<ISigningKeyPtr> key;
		bool has_key = obj->GetSigningKey(key);
		if (!has_key) {
			return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		}

		auto ptr = key.AddRefGet();
		*result = reinterpret_cast<SigningKeyHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetSigningKey(DocumentSignatureSettingsHandle handle, SigningKeyHandle value) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	ISigningKey* key = reinterpret_cast<ISigningKey*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(key);

	try {
		obj->SetSigningKey(key);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetName(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle* result) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		OutputPointer<LiteralStringObjectPtr> name;
		bool has_name = obj->GetName(name);
		if (!has_name) {
			return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		}

		auto ptr = name.AddRefGet();
		*result = reinterpret_cast<LiteralStringObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetName(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle value) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	LiteralStringObject* name = reinterpret_cast<LiteralStringObject*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);

	try {
		obj->SetName(name);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetLocation(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle* result) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		OutputPointer<LiteralStringObjectPtr> location;
		bool has_name = obj->GetLocation(location);
		if (!has_name) {
			return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		}

		auto ptr = location.AddRefGet();
		*result = reinterpret_cast<LiteralStringObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetLocation(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle value) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	LiteralStringObject* location = reinterpret_cast<LiteralStringObject*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(location);

	try {
		obj->SetLocation(location);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetReason(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle* result) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		OutputPointer<LiteralStringObjectPtr> reason;
		bool has_name = obj->GetReason(reason);
		if (!has_name) {
			return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		}

		auto ptr = reason.AddRefGet();
		*result = reinterpret_cast<LiteralStringObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetReason(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle value) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	LiteralStringObject* reason = reinterpret_cast<LiteralStringObject*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(reason);

	try {
		obj->SetReason(reason);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetCertificate(DocumentSignatureSettingsHandle handle, HexadecimalStringObjectHandle* result) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		OutputPointer<HexadecimalStringObjectPtr> certificate;
		bool has_name = obj->GetCertificate(certificate);
		if (!has_name) {
			return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		}

		auto ptr = certificate.AddRefGet();
		*result = reinterpret_cast<HexadecimalStringObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetCertificate(DocumentSignatureSettingsHandle handle, HexadecimalStringObjectHandle value) {
	DocumentSignatureSettings* obj = reinterpret_cast<DocumentSignatureSettings*>(handle);
	HexadecimalStringObject* certificate = reinterpret_cast<HexadecimalStringObject*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(certificate);

	try {
		obj->SetCertificate(certificate);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_Release(DocumentSignatureSettingsHandle handle) {
	return ObjectRelease<DocumentSignatureSettings, DocumentSignatureSettingsHandle>(handle);
}