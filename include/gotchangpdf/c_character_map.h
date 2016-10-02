#ifndef _C_CHARACTER_MAP_H
#define _C_CHARACTER_MAP_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum {
		CharacterMapType_Embedded = 0,
		CharacterMapType_Unicode
	} CharacterMapType, *PCharacterMapType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_Type(CharacterMapHandle handle, PCharacterMapType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_Release(CharacterMapHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_ToUnicode(CharacterMapHandle handle, PUnicodeCharacterMapHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_ToEmbedded(CharacterMapHandle handle, PEmbeddedCharacterMapHandle result);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_GetMappedValue(
		UnicodeCharacterMapHandle handle,
		BufferHandle key_handle,
		PBufferHandle result);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_Release(UnicodeCharacterMapHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION EmbeddedCharacterMap_Release(EmbeddedCharacterMapHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_CHARACTER_MAP_H */
