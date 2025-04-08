#ifndef _C_DOCUMENT_H
#define _C_DOCUMENT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_document.h
    * \brief This file contains class definitions for \ref DocumentHandle
    */

    /**
    * \class DocumentHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief Represents high-level file access handle
    */

    /**
    * \memberof DocumentHandle
    * @{
    */

    /**
    * \brief Opens a new document
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_Open(string_type filename, DocumentHandle** result);

    /**
    * \brief Opens a document using already existing file handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_OpenFile(FileHandle* holder, DocumentHandle** result);

    /**
    * \brief Creates a new document
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_Create(string_type filename, DocumentHandle** result);

    /**
    * \brief Creates a new document
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_CreateFile(FileHandle* holder, DocumentHandle** result);

    /**
    * \copydoc FileWriterHandle::FileWriter_Write
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_Save(DocumentHandle* handle, string_type filename);

    /**
    * \copydoc FileWriterHandle::FileWriter_Write
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_SaveFile(DocumentHandle* handle, FileHandle* file_handle);

    /**
    * \copydoc FileWriterHandle::FileWriter_WriteIncremental
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_SaveIncremental(DocumentHandle* handle, string_type filename);

    /**
    * \copydoc FileWriterHandle::FileWriter_WriteIncremental
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_SaveIncrementalFile(DocumentHandle* handle, FileHandle* file_handle);

    /**
    * \brief Get document's catalog property
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_GetCatalog(DocumentHandle* handle, CatalogHandle** result);

    /**
    * \brief Get detailed document metadata
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_GetDocumentInfo(DocumentHandle* handle, DocumentInfoHandle** result);

    /**
    * \brief Append another document's contents at the end of the file.
    *
    * This appends all \p source document's pages beyond \p handle document's pages.
    *
    * After pages are merged, all indirect referenced are resolved so they point to correct objects.
    *
    * The last step fixes link annotations, which can be referenced by page number.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_AppendDocument(DocumentHandle* handle, DocumentHandle* source);

    /**
    * \brief Digitally signs the document using specified settings
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_Sign(DocumentHandle* handle, FileHandle* destination, DocumentSignatureSettingsHandle* settings);

    /**
    * \brief Encrypt the document using the specified settings
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_AddEncryption(DocumentHandle* handle, DocumentEncryptionSettingsHandle* settings);

    /**
    * \brief Remove any existing encryption from the document.
    * 
    * The document MUST be unlocked either with password or encryption key.
    * \see \ref FileHandle::File_SetEncryptionPassword
    * \see \ref FileHandle::File_SetEncryptionKey
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_RemoveEncryption(DocumentHandle* handle);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_ToUnknown(DocumentHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref DocumentHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_FromUnknown(IUnknownHandle* handle, DocumentHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_Release(DocumentHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DOCUMENT_H */
