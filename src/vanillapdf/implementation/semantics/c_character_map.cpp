#include "precompiled.h"
#include "semantics/objects/character_map.h"

#include "vanillapdf/semantics/c_character_map.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION CharacterMap_GetCharacterMapType(CharacterMapHandle* handle, CharacterMapType* result)
{
	CharacterMapBase* obj = reinterpret_cast<CharacterMapBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetCharacterMapType()) {
	case CharacterMapBase::Type::Embedded:
		*result = CharacterMapType_Embedded; break;
	case CharacterMapBase::Type::Unicode:
		*result = CharacterMapType_Unicode; break;
	default:
		return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION CharacterMap_ToUnknown(CharacterMapHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<CharacterMapBase, IUnknown, CharacterMapHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION CharacterMap_FromUnknown(IUnknownHandle* handle, CharacterMapHandle** result) {
	return SafeObjectConvert<IUnknown, CharacterMapBase, IUnknownHandle, CharacterMapHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION CharacterMap_Release(CharacterMapHandle* handle) {
	return ObjectRelease<CharacterMapBase, CharacterMapHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_GetMappedValue(
	UnicodeCharacterMapHandle* handle,
	BufferHandle* key_handle,
	BufferHandle** result)
{
	UnicodeCharacterMap* obj = reinterpret_cast<UnicodeCharacterMap*>(handle);
	Buffer* key = reinterpret_cast<Buffer*>(key_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(key);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->GetMappedValue(key);
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_ToCharacterMap(UnicodeCharacterMapHandle* handle, CharacterMapHandle** result) {
	return SafeObjectConvert<UnicodeCharacterMap, CharacterMapBase, UnicodeCharacterMapHandle, CharacterMapHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_FromCharacterMap(CharacterMapHandle* handle, UnicodeCharacterMapHandle** result) {
	return SafeObjectConvert<CharacterMapBase, UnicodeCharacterMap, CharacterMapHandle, UnicodeCharacterMapHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_Release(UnicodeCharacterMapHandle* handle) {
	return ObjectRelease<UnicodeCharacterMap, UnicodeCharacterMapHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION EmbeddedCharacterMap_ToCharacterMap(EmbeddedCharacterMapHandle* handle, CharacterMapHandle** result) {
	return SafeObjectConvert<EmbeddedCharacterMap, CharacterMapBase, EmbeddedCharacterMapHandle, CharacterMapHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION EmbeddedCharacterMap_FromCharacterMap(CharacterMapHandle* handle, EmbeddedCharacterMapHandle** result) {
	return SafeObjectConvert<CharacterMapBase, EmbeddedCharacterMap, CharacterMapHandle, EmbeddedCharacterMapHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION EmbeddedCharacterMap_Release(EmbeddedCharacterMapHandle* handle) {
	return ObjectRelease<EmbeddedCharacterMap, EmbeddedCharacterMapHandle>(handle);
}
