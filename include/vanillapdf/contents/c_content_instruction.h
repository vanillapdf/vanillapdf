#ifndef _C_CONTENT_INSTRUCTION_H
#define _C_CONTENT_INSTRUCTION_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_content_instruction.h
    * \brief This file contains class definitions for \ref ContentInstructionHandle
    */

    /**
    * \class ContentInstructionHandle
    * \extends IUnknownHandle
    * \ingroup group_contents
    * \brief Base class for all content objects and operations.
    */

    /**
    * \class ContentInstructionCollectionHandle
    * \extends IUnknownHandle
    * \ingroup group_contents
    * \brief Collection of content instructions
    */

    /**
    * \class ContentInstructionCollectionIteratorHandle
    * \extends IUnknownHandle
    * \ingroup group_contents
    * \brief Enumerator for content instruction collection
    */

    /**
    * \brief Available content instruction types
    * \ingroup group_contents
    */
    typedef enum {
        ContentInstructionType_Undefined = 0,

        /**
        * \copydoc ContentOperationHandle
        * \see ContentOperationHandle
        */
        ContentInstructionType_Operation,

        /**
        * \copydoc ContentObjectHandle
        * \see \ref ContentObjectHandle
        */
        ContentInstructionType_Object
    } ContentInstructionType;

    /**
    * \memberof ContentInstructionHandle
    * @{
    */

    /**
    * \brief Get derived type of current object
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_GetInstructionType(ContentInstructionHandle* handle, ContentInstructionType* result);

    /**
    * \brief Reinterpret current object as \ref ContentOperationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_ToOperation(ContentInstructionHandle* handle, ContentOperationHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_ToUnknown(ContentInstructionHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref ContentInstructionHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_FromUnknown(IUnknownHandle* handle, ContentInstructionHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_Release(ContentInstructionHandle* handle);

    /** @} */

    /**
    * \memberof ContentInstructionCollectionHandle
    * @{
    */

    /**
    * \brief Return size of a collection
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_GetSize(ContentInstructionCollectionHandle* handle, size_type* result);

    /**
    * \brief Get instruction at location \p at
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_At(ContentInstructionCollectionHandle* handle, size_type at, ContentInstructionHandle** result);

    /**
    * \brief Insert new element at the end of the collection
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_Append(ContentInstructionCollectionHandle* handle, ContentInstructionHandle* value);

    /**
    * \brief Insert new element at location \p at
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_Insert(ContentInstructionCollectionHandle* handle, size_type at, ContentInstructionHandle* value);

    /**
    * \brief Remove element from location \p at
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_Remove(ContentInstructionCollectionHandle* handle, size_type at);

    /**
    * \brief Clear all items from the collection
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_Clear(ContentInstructionCollectionHandle* handle);

    /**
    * \brief Get collection iterator for enumerating all entries
    *
    * Modifying collection may invalidate this handle.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_GetIterator(ContentInstructionCollectionHandle* handle, ContentInstructionCollectionIteratorHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_ToUnknown(ContentInstructionCollectionHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref ContentInstructionCollectionHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_FromUnknown(IUnknownHandle* handle, ContentInstructionCollectionHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_Release(ContentInstructionCollectionHandle* handle);

    /** @} */

    /**
    * \memberof ContentInstructionCollectionIteratorHandle
    * @{
    */

    /**
    * \brief Get value at iterator position
    *
    * Ensure the iterator is valid.
    * \see \ref ContentInstructionCollectionIterator_IsValid
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_GetValue(ContentInstructionCollectionIteratorHandle* handle, ContentInstructionHandle** result);

    /**
    * \brief Determine if current position is valid
    *
    * Invalid position may mean that iterator moved past the end of the collection, as well as the collection was modified.
    *
    * Any other operation except \b THIS will fail on invalid iterator.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_IsValid(ContentInstructionCollectionIteratorHandle* handle, boolean_type* result);

    /**
    * \brief Advances iterator to the next position
    *
    * Ensure the iterator is valid.
    * \see \ref ContentInstructionCollectionIterator_IsValid
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_Next(ContentInstructionCollectionIteratorHandle* handle);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_ToUnknown(ContentInstructionCollectionIteratorHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref ContentInstructionCollectionIteratorHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_FromUnknown(IUnknownHandle* handle, ContentInstructionCollectionIteratorHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_Release(ContentInstructionCollectionIteratorHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENT_INSTRUCTION_H */
