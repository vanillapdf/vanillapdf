#ifndef _C_FONT_MAP_H
#define _C_FONT_MAP_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_font_map.h
    * \brief This file contains contains class definitions for \ref FontMapHandle
    */

    /**
    * \class FontMapHandle
    * \extends IUnknownHandle
    * \ingroup group_fonts
    * \brief
    * A dictionary that maps resource names to font dictionaries.
    * \see \ref ResourceDictionaryHandle
    */

    /**
    * \class FontMapIteratorHandle
    * \extends IUnknownHandle
    * \ingroup group_fonts
    * \brief
    * Iterator concept allowing to traverse over \ref FontMapHandle collection.
    */

    /**
    * \memberof FontMapIteratorHandle
    * @{
    */

    /**
    * \brief Get key at iterator position
    *
    * Ensure the iterator is valid.
    * \see \ref FontMapIterator_IsValid
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontMapIterator_GetKey(FontMapIteratorHandle* handle, NameObjectHandle** result);

    /**
    * \brief Get value at iterator position
    *
    * Ensure the iterator is valid.
    * \see \ref FontMapIterator_IsValid
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontMapIterator_GetValue(FontMapIteratorHandle* handle, FontHandle** result);

    /**
    * \brief Determine if current position is valid
    *
    * Invalid position may mean that iterator moved past the end of the collection, as well as the collection was modified.
    *
    * Any other operation except \b THIS will fail on invalid iterator.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontMapIterator_IsValid(FontMapIteratorHandle* handle, boolean_type* result);

    /**
    * \brief Advances iterator to the next position
    *
    * Ensure the iterator is valid.
    * \see \ref FontMapIterator_IsValid
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontMapIterator_Next(FontMapIteratorHandle* handle);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontMapIterator_Release(FontMapIteratorHandle* handle);

    /** @} */

    /**
    * \memberof FontMapHandle
    * @{
    */

    /**
    * \brief Determine if collection contains key \p key_handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Contains(FontMapHandle* handle, const NameObjectHandle* key_handle, boolean_type* result);

    /**
    * \brief Find mapped value for key \p key_handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Find(FontMapHandle* handle, const NameObjectHandle* key_handle, FontHandle** result);

    /**
    * \brief Get collection iterator for enumerating all entries
    *
    * Modifying collection may invalidate this handle.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontMap_GetIterator(FontMapHandle* handle, FontMapIteratorHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontMap_ToUnknown(FontMapHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref FontMapHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontMap_FromUnknown(IUnknownHandle* handle, FontMapHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Release(FontMapHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FONT_MAP_H */
