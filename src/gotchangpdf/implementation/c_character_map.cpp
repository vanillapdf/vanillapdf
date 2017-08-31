#include "precompiled.h"
#include "semantics/objects/character_map.h"

#include "gotchangpdf/c_character_map.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_GetType(CharacterMapHandle handle, PCharacterMapType result)
{
	CharacterMapBase* obj = reinterpret_cast<CharacterMapBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetType()) {
	case CharacterMapBase::Type::Embedded:
		*result = CharacterMapType_Embedded; break;
	case CharacterMapBase::Type::Unicode:
		*result = CharacterMapType_Unicode; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_Release(CharacterMapHandle handle)
{
	return ObjectRelease<CharacterMapBase, CharacterMapHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_ToUnicode(CharacterMapHandle handle, UnicodeCharacterMapHandle* result)
{
	return SafeObjectConvert<CharacterMapBase, UnicodeCharacterMap, CharacterMapHandle, UnicodeCharacterMapHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_ToEmbedded(CharacterMapHandle handle, EmbeddedCharacterMapHandle* result)
{
	return SafeObjectConvert<CharacterMapBase, EmbeddedCharacterMap, CharacterMapHandle, EmbeddedCharacterMapHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_GetMappedValue(
	UnicodeCharacterMapHandle handle,
	BufferHandle key_handle,
	BufferHandle* result)
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
		*result = reinterpret_cast<BufferHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_Release(UnicodeCharacterMapHandle handle)
{
	return ObjectRelease<UnicodeCharacterMap, UnicodeCharacterMapHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION EmbeddedCharacterMap_Release(EmbeddedCharacterMapHandle handle)
{
	return ObjectRelease<EmbeddedCharacterMap, EmbeddedCharacterMapHandle>(handle);
}