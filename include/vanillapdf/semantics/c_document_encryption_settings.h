#ifndef _C_DOCUMENT_ENCRYPTION_SETTINGS_H
#define _C_DOCUMENT_ENCRYPTION_SETTINGS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_document_encryption_settings.h
	* \brief This file contains class definitions for \ref DocumentEncryptionSettingsHandle
	*/

	/**
	* \class DocumentEncryptionSettingsHandle
	* \extends IUnknownHandle
	* \ingroup group_documents
	* \brief Group of settings controlling document's encryption and decryption
	*/

	/**
	* \brief Available user access permission flags for the encrypted document
	*/
	typedef enum {

		/**
		* \brief No actions are allowed for the user.
		*/
		UserAccessPermissionFlag_None = 0,

		/**
		* \brief
		* (Security handlers of revision 2) Print the document.
		* 
		* (Security handlers of revision 3 or greater) Print the document (possibly not at the highest quality level,
		* depending on whether bit 12 is also set).
		*/
		UserAccessPermissionFlag_PrintDegraded = 4,

		/**
		* \brief Modify the contents of the document by operations other than those controlled by bits 6, 9, and 11.
		*/
		UserAccessPermissionFlag_ModifyContents = 8,

		/**
		* \brief
		* (Security handlers of revision 2)
		* Copy or otherwise extract text and graphics from the document,
		* including extracting text and graphics (in support of accessibility to users with disabilities or for other purposes).
		*
		* (Security handlers of revision 3 or greater)
		* Copy or otherwise extract text and graphics from the document by operations other than that controlled by bit 10.
		*/
		UserAccessPermissionFlag_CopyAndExtract = 16,

		/**
		* \brief Add or modify text annotations, fill in interactive form fields, and, if bit 4 is also set,
		* create or modify interactive form fields (including signature fields).
		*/
		UserAccessPermissionFlag_AddAnnotations = 32,

		/**
		* \brief
		* (Security handlers of revision 3 or greater)
		* Fill in existing interactive form fields (including signature fields), even if bit 6 is clear.
		*/
		UserAccessPermissionFlag_FillForms = 256,

		/**
		* \brief
		* (Security handlers of revision 3 or greater)
		* Extract text and graphics (in support of accessibility to users with disabilities or for other purposes).
		*/
		UserAccessPermissionFlag_ExtractText = 512,

		/**
		* \brief
		* (Security handlers of revision 3 or greater)
		* Assemble the document (insert, rotate, or delete pages and create bookmarks or thumbnail images), even if bit 4 is clear.
		*/
		UserAccessPermissionFlag_AssembleDocument = 1024,

		/**
		* \brief
		* (Security handlers of revision 3 or greater)
		* Print the document to a representation from which a faithful digital copy of the PDF content could be generated.
		* When this bit is clear (and bit 3 is set), printing is limited to a low-level representation of the appearance,
		* possibly of degraded quality.
		*/
		UserAccessPermissionFlag_PrintFaithful = 2048
	} UserAccessPermissionFlags;

	/**
	* \brief Algorithm to be used for document encryption
	*/
	typedef enum {

		/**
		* \brief Undefined unitialized default value, triggers error when used
		*/
		EncryptionAlgorithmType_Undefined = 0,

		/**
		* \brief The application shall not decrypt data but shall direct the input stream to the security handler for decryption.
		*/
		EncryptionAlgorithmType_None,

		/**
		* \brief
		* A proprietary encryption algorithm known as RC4. RC4 is a symmetric stream cipher:
		* the same algorithm shall be used for both encryption and decryption,
		* and the algorithm does not change the length of the data. RC4 is a copyrighted,
		* proprietary algorithm of RSA Security, Inc. Independent software vendors may be required to license RC4
		* to develop software that encrypts or decrypts PDF documents.
		* For further information, visit the RSA Web site at <http://www.rsasecurity.com> or send e-mail to <products@rsasecurity.com>.
		*/
		EncryptionAlgorithmType_RC4,

		/**
		* \brief
		* The AES (Advanced Encryption Standard) algorithm (beginning with PDF 1.6).
		* AES is a symmetric block cipher: the same algorithm shall be used for both encryption and decryption,
		* and the length of the data when encrypted is rounded up to a multiple of the block size, which is fixed to always be 16 bytes,
		* as specified in FIPS 197, Advanced Encryption Standard (AES); see the Bibliography).
		*/
		EncryptionAlgorithmType_AES
	} EncryptionAlgorithmType;

	/**
	* \memberof DocumentEncryptionSettingsHandle
	* @{
	*/

	/**
	* \brief Creates a new DocumentEncryptionSettings instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_Create(DocumentEncryptionSettingsHandle** result);

	/**
	* \brief Algorithm to be used for document encryption
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_GetAlgorithm(DocumentEncryptionSettingsHandle* handle, EncryptionAlgorithmType* result);

	/**
	* \copydoc DocumentEncryptionSettings_GetAlgorithm
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_SetAlgorithm(DocumentEncryptionSettingsHandle* handle, EncryptionAlgorithmType encryption_algorithm);

	/**
	* \brief Length of the key for document encryption
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_GetKeyLength(DocumentEncryptionSettingsHandle* handle, integer_type* result);

	/**
	* \copydoc DocumentEncryptionSettings_GetKeyLength
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_SetKeyLength(DocumentEncryptionSettingsHandle* handle, integer_type key_length);

	/**
	* \brief Permissions for user, when opening the document with user password
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_GetUserAccessPermissions(DocumentEncryptionSettingsHandle* handle, UserAccessPermissionFlags* result);

	/**
	* \copydoc DocumentEncryptionSettings_GetUserAccessPermissions
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_SetUserAccessPermissions(DocumentEncryptionSettingsHandle* handle, UserAccessPermissionFlags user_permissions);

	/**
	* \brief User password to be used for document encryption
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_GetUserPassword(DocumentEncryptionSettingsHandle* handle, BufferHandle** result);

	/**
	* \copydoc DocumentEncryptionSettings_GetUserPassword
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_SetUserPassword(DocumentEncryptionSettingsHandle* handle, BufferHandle* user_password);

	/**
	* \brief Owner password to be used for document encryption
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_GetOwnerPassword(DocumentEncryptionSettingsHandle* handle, BufferHandle** result);

	/**
	* \copydoc DocumentEncryptionSettings_GetOwnerPassword
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_SetOwnerPassword(DocumentEncryptionSettingsHandle* handle, BufferHandle* owner_password);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_ToUnknown(DocumentEncryptionSettingsHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref DocumentEncryptionSettingsHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_FromUnknown(IUnknownHandle* handle, DocumentEncryptionSettingsHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentEncryptionSettings_Release(DocumentEncryptionSettingsHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DOCUMENT_ENCRYPTION_SETTINGS_H */
