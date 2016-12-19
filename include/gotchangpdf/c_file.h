#ifndef _C_FILE_H
#define _C_FILE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \addtogroup Files
	* @{
	*/

	/**
	* \brief
	* Opens a file for reading.
	*
	* The file must exist.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Open(string_type filename, PFileHandle result);

	/**
	* \brief
	* Creates a file for writing.
	*
	* Truncates the contents if it already exists.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Create(string_type filename, PFileHandle result);

	/**
	* \brief
	* Perform basic intialization.
	*
	* Read xref tables, determine if file is encrypted.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Initialize(FileHandle handle);

	/**
	* \brief
	* Determine if file is encrypted.
	*
	* File must be initialized before trying to call this function,
	* otherwise GOTCHANG_PDF_ERROR_FILE_NOT_INITIALIZED will be returned.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_IsEncrypted(FileHandle handle, out_boolean_type result);

	/**
	* \brief
	* Set encryption password.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_SetEncryptionPassword(FileHandle handle, string_type password);

	/**
	* \brief
	* Set files encryption key
	*
	* This is most often certificate.
	* \see EncryptionKeyHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_SetEncryptionKey(FileHandle handle, EncryptionKeyHandle key);

	/**
	* \brief
	* Get chain of xref tables for iteration
	* \see XrefChainHandle
	* \see XrefChainIterator
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_XrefChain(FileHandle handle, PXrefChainHandle result);

	/**
	* \brief
	* Find exact object within all xref tables
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_GetIndirectObject(FileHandle handle, biguint_type objNumber, ushort_type genNumber, PObjectHandle result);

	/**
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Release(FileHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_Create(PFileWriterHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_Write(FileWriterHandle handle, FileHandle source, FileHandle destination);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_WriteIncremental(FileWriterHandle handle, FileHandle source, FileHandle destination);

	/**
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_Release(FileWriterHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FILE_H */
