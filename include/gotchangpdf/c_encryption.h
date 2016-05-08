#ifndef _C_ENCRYPTION_H
#define _C_ENCRYPTION_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef void (*InitializeFunction)(void);
	typedef void (*CleanupFunction)(void);
	typedef error_type (*DecryptFunction)(const BufferHandleTag* data, PBufferHandle result);
	typedef error_type (*EqualsFunction)(const BufferHandleTag* issuer, const BufferHandleTag* serial, out_boolean_type result);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Pkcs12EncryptionKey_CreateFromFile(string_type path, integer_type length, PEncryptionKeyHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Pkcs12EncryptionKey_CreateFromBuffer(BufferHandle data, PEncryptionKeyHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CustomEncryptionKey_Create(InitializeFunction initialize, CleanupFunction cleanup, DecryptFunction decrypt, EqualsFunction equals, PEncryptionKeyHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION EncryptionKey_Release(EncryptionKeyHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_ENCRYPTION_H */
