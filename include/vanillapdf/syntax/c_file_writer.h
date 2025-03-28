#ifndef _C_FILE_WRITER_H
#define _C_FILE_WRITER_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_file_writer.h
    * \brief This file contains class definitions for \ref FileWriterHandle
    */

    /**
    * \class FileWriterHandle
    * \extends IUnknownHandle
    * \ingroup group_files
    * \brief Implements serialization of Files to destination stream
    */

    /**
    * \memberof FileWriterHandle
    * @{
    */

    /**
    * \brief Create new FileWriter instance
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_Create(FileWriterHandle** result);

    /**
    * \brief Save file state into new destination
    *
    * Destination file will be overwritten if it exists.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_Write(FileWriterHandle* handle, FileHandle* source, FileHandle* destination);

    /**
    * \brief Save file state into new destination and create new section for all modifications done by user
    *
    * This method creates a new section with all modified objects.
    * Whole file content preceeding new section will be preserved.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_WriteIncremental(FileWriterHandle* handle, FileHandle* source, FileHandle* destination);

    /**
    * \brief Subscribe a new observer to the file writer events
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_Subscribe(FileWriterHandle* handle, FileWriterObserverHandle* observer);

    /**
    * \brief Unsubscribe existing observer from the file writer events
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_Unsubscribe(FileWriterHandle* handle, FileWriterObserverHandle* observer);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_ToUnknown(FileWriterHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref FileWriterHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_FromUnknown(IUnknownHandle* handle, FileWriterHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_Release(FileWriterHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FILE_WRITER_H */
