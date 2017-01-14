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
	* \file c_file.h
	* This file contains class definitions for FileHandle
	*/

	/**
	* \class FileHandle
	* \extends IUnknownHandle
	* \ingroup Files
	* \brief Represents low-level file access handle.
	*
	* This is a direct reference to a PDF specification,
	* section 7.5, "File Structure".
	*/

	/**
	* \class FileWriterHandle
	* \extends IUnknownHandle
	* \ingroup Files
	* \brief Implements serialization of Files to destination stream
	*/

	/**
	* \memberof FileHandle
	* @{
	*/

	/**
	* \brief Opens a file for reading.
	*
	* The file must exist.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Open(string_type filename, FileHandle* result);

	/**
	* \brief Creates a file for writing.
	*
	* Truncates the contents if it already exists.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Create(string_type filename, FileHandle* result);

	/**
	* \brief Perform basic intialization.
	*
	* Read xref tables, determine if file is encrypted.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Initialize(FileHandle handle);

	/**
	* \brief Determine if file is encrypted.
	*
	* If the file was not initialized returns false.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_IsEncrypted(FileHandle handle, boolean_type* result);

	/**
	* \brief Set encryption password.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_SetEncryptionPassword(FileHandle handle, string_type password);

	/**
	* \brief Set files encryption key
	*
	* This is most often certificate.
	* \see EncryptionKeyHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_SetEncryptionKey(FileHandle handle, EncryptionKeyHandle key);

	/**
	* \brief Get chain of xref tables for iteration
	* \see XrefChainHandle
	* \see XrefChainIterator
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_XrefChain(FileHandle handle, XrefChainHandle* result);

	/**
	* \brief Find exact object within all xref tables
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_GetIndirectObject(FileHandle handle, biguint_type objNumber, ushort_type genNumber, ObjectHandle* result);

	/**
	* \copydoc IUnknown_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Release(FileHandle handle);

	/** @} */

	/**
	* \memberof FileWriterHandle
	* @{
	*/

	/**
	* \brief Create new FileWriter instance
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_Create(FileWriterHandle* result);

	/**
	* \brief Save file state into new destination
	*
	* Destination file will be overwritten if it exists.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_Write(FileWriterHandle handle, FileHandle source, FileHandle destination);

	/**
	* \brief Save file state into new destination and create new section for all modifications done by user
	*
	* This method creates a new section with all modified objects.
	* Whole file content preceeding new section will be preserved.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_WriteIncremental(FileWriterHandle handle, FileHandle source, FileHandle destination);

	/**
	* \copydoc IUnknown_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_Release(FileWriterHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FILE_H */
