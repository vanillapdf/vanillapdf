#ifndef _C_FILE_H
#define _C_FILE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

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
	*
	* The file is basically composed of:
	* - __Header__ is basically just a statement about
	*   the PDF version this file is referring to
	* - __Body__ is just a sequence of \ref Objects described in ::XrefHandle
	* - The __trailer__ contains the ::XrefHandle itself,
	*   with the bytes offset to start of the last cross-reference section.
	*
	* When the file has been incrementally updated,
	* there may be multiple cross-reference sections.
	* For details about this topic please visit section
	* 7.5.6 "Incremental Updates" of the PDF specification.
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
	VANILLAPDF_API error_type CALLING_CONVENTION File_Open(string_type filename, FileHandle* result);

	/**
	* \brief Creates a file for writing.
	*
	* Truncates the contents if it already exists.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION File_Create(string_type filename, FileHandle* result);

	/**
	* \brief Perform basic intialization.
	*
	* Read xref tables, determine if file is encrypted.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION File_Initialize(FileHandle handle);

	/**
	* \brief Determine if file is encrypted.
	*
	* If the file was not initialized returns false.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION File_IsEncrypted(FileHandle handle, boolean_type* result);

	/**
	* \brief Set encryption password.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION File_SetEncryptionPassword(FileHandle handle, string_type password);

	/**
	* \brief Set files encryption key
	*
	* This is most often certificate.
	* \see EncryptionKeyHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION File_SetEncryptionKey(FileHandle handle, EncryptionKeyHandle key);

	/**
	* \brief Get chain of xref tables for iteration
	* \see XrefChainHandle
	* \see XrefChainIterator
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION File_XrefChain(FileHandle handle, XrefChainHandle* result);

	/**
	* \brief Find exact object within all xref tables
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION File_GetIndirectObject(FileHandle handle, biguint_type objNumber, ushort_type genNumber, ObjectHandle* result);

	/**
	* \copydoc IUnknown_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION File_Release(FileHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FILE_H */
