#include "precompiled.h"

#include "semantics/utils/document_encryption_settings.h"

#include "vanillapdf/semantics/c_document_encryption_settings.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

// Verify the enum values to match at compile time
static_assert(sizeof(UserAccessPermissionFlags) == sizeof(DocumentEncryptionSettings::UserAccessPermissionFlags));

static_assert(static_cast<int32_t>(UserAccessPermissionFlag_None) == static_cast<int32_t>(DocumentEncryptionSettings::UserAccessPermissionFlags::None));
static_assert(static_cast<int32_t>(UserAccessPermissionFlag_PrintDegraded) == static_cast<int32_t>(DocumentEncryptionSettings::UserAccessPermissionFlags::PrintDegraded));
static_assert(static_cast<int32_t>(UserAccessPermissionFlag_ModifyContents) == static_cast<int32_t>(DocumentEncryptionSettings::UserAccessPermissionFlags::ModifyContents));
static_assert(static_cast<int32_t>(UserAccessPermissionFlag_CopyAndExtract) == static_cast<int32_t>(DocumentEncryptionSettings::UserAccessPermissionFlags::CopyAndExtract));
static_assert(static_cast<int32_t>(UserAccessPermissionFlag_AddAnnotations) == static_cast<int32_t>(DocumentEncryptionSettings::UserAccessPermissionFlags::AddAnnotations));
static_assert(static_cast<int32_t>(UserAccessPermissionFlag_FillForms) == static_cast<int32_t>(DocumentEncryptionSettings::UserAccessPermissionFlags::FillForms));
static_assert(static_cast<int32_t>(UserAccessPermissionFlag_ExtractText) == static_cast<int32_t>(DocumentEncryptionSettings::UserAccessPermissionFlags::ExtractText));
static_assert(static_cast<int32_t>(UserAccessPermissionFlag_AssembleDocument) == static_cast<int32_t>(DocumentEncryptionSettings::UserAccessPermissionFlags::AssembleDocument));
static_assert(static_cast<int32_t>(UserAccessPermissionFlag_PrintFaithful) == static_cast<int32_t>(DocumentEncryptionSettings::UserAccessPermissionFlags::PrintFaithful));

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_Create(DocumentEncryptionSettingsHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		DocumentEncryptionSettingsPtr encryption_settings;
		auto ptr = encryption_settings.AddRefGet();
		*result = reinterpret_cast<DocumentEncryptionSettingsHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_GetAlgorithm(DocumentEncryptionSettingsHandle* handle, EncryptionAlgorithmType* result) {
	DocumentEncryptionSettings* obj = reinterpret_cast<DocumentEncryptionSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetEncryptionAlgorithm()) {
	case EncryptionAlgorithm::Undefined:
		*result = EncryptionAlgorithmType_Undefined; break;
	case EncryptionAlgorithm::None:
		*result = EncryptionAlgorithmType_None; break;
	case EncryptionAlgorithm::RC4:
		*result = EncryptionAlgorithmType_RC4; break;
	case EncryptionAlgorithm::AES:
		*result = EncryptionAlgorithmType_AES; break;
	default:
		return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_SetAlgorithm(DocumentEncryptionSettingsHandle* handle, EncryptionAlgorithmType encryption_algorithm) {
	DocumentEncryptionSettings* obj = reinterpret_cast<DocumentEncryptionSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	switch (encryption_algorithm) {
	case EncryptionAlgorithmType_Undefined:
		obj->SetEncryptionAlgorithm(EncryptionAlgorithm::Undefined); break;
	case EncryptionAlgorithmType_None:
		obj->SetEncryptionAlgorithm(EncryptionAlgorithm::None); break;
	case EncryptionAlgorithmType_RC4:
		obj->SetEncryptionAlgorithm(EncryptionAlgorithm::RC4); break;
	case EncryptionAlgorithmType_AES:
		obj->SetEncryptionAlgorithm(EncryptionAlgorithm::AES); break;
	default:
		return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_GetKeyLength(DocumentEncryptionSettingsHandle* handle, integer_type* result) {
	DocumentEncryptionSettings* obj = reinterpret_cast<DocumentEncryptionSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetKeyLength();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_SetKeyLength(DocumentEncryptionSettingsHandle* handle, integer_type key_length) {
	DocumentEncryptionSettings* obj = reinterpret_cast<DocumentEncryptionSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	obj->SetKeyLength(key_length);
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_GetUserAccessPermissions(DocumentEncryptionSettingsHandle* handle, UserAccessPermissionFlags* result) {
	DocumentEncryptionSettings* obj = reinterpret_cast<DocumentEncryptionSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	// The values between internal and public enum match exactly allowing to make this conversion
	*result = static_cast<UserAccessPermissionFlags>(obj->GetUserPermissions());
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_SetUserAccessPermissions(DocumentEncryptionSettingsHandle* handle, UserAccessPermissionFlags user_permissions) {
	DocumentEncryptionSettings* obj = reinterpret_cast<DocumentEncryptionSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	// The values between internal and public enum match exactly allowing to make this conversion
	auto user_permissions_converted = static_cast<DocumentEncryptionSettings::UserAccessPermissionFlags>(user_permissions);
	obj->SetUserPermissions(user_permissions_converted);

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_GetUserPassword(DocumentEncryptionSettingsHandle* handle, BufferHandle** result) {
	DocumentEncryptionSettings* obj = reinterpret_cast<DocumentEncryptionSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->GetUserPassword();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_SetUserPassword(DocumentEncryptionSettingsHandle* handle, BufferHandle* user_password) {
	DocumentEncryptionSettings* obj = reinterpret_cast<DocumentEncryptionSettings*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(user_password);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetUserPassword(buffer);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_GetOwnerPassword(DocumentEncryptionSettingsHandle* handle, BufferHandle** result) {
	DocumentEncryptionSettings* obj = reinterpret_cast<DocumentEncryptionSettings*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->GetOwnerPassword();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_SetOwnerPassword(DocumentEncryptionSettingsHandle* handle, BufferHandle* owner_password) {
	DocumentEncryptionSettings* obj = reinterpret_cast<DocumentEncryptionSettings*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(owner_password);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetOwnerPassword(buffer);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_ToUnknown(DocumentEncryptionSettingsHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<DocumentEncryptionSettings, IUnknown, DocumentEncryptionSettingsHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_FromUnknown(IUnknownHandle* handle, DocumentEncryptionSettingsHandle** result) {
	return SafeObjectConvert<IUnknown, DocumentEncryptionSettings, IUnknownHandle, DocumentEncryptionSettingsHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_Release(DocumentEncryptionSettingsHandle* handle) {
	return ObjectRelease<DocumentEncryptionSettings, DocumentEncryptionSettingsHandle>(handle);
}
