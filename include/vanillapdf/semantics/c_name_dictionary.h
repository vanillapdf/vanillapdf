#ifndef _C_NAME_DICTIONARY_H
#define _C_NAME_DICTIONARY_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_name_dictionary.h
    * \brief This file contains class definitions for \ref NameDictionaryHandle
    */

    /**
    * \class NameDictionaryHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * The name dictionary (PDF 1.2+) associates names with various
    * document objects using name trees for efficient lookup.
    *
    * For more details please visit section 7.7.4 - Name Dictionary.
    * \see \ref CatalogHandle
    */

    /**
    * \memberof NameDictionaryHandle
    * @{
    */

    /**
    * \brief Create a new empty name dictionary.
    *
    * The created dictionary can be assigned to a catalog using Catalog_SetNames.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_Create(NameDictionaryHandle** result);

    /**
    * \brief Check if the name dictionary contains a Dests name tree.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_ContainsDestinations(NameDictionaryHandle* handle, boolean_type* result);

    /**
    * \brief Get the Dests name tree from the name dictionary.
    *
    * The Dests name tree maps destination names (strings) to destination objects.
    * Use NameDictionary_ContainsDestinations to check if the tree exists.
    *
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if the Dests tree does not exist
    */
    VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_GetDestinations(NameDictionaryHandle* handle, DestinationNameTreeHandle** result);

    /**
    * \brief Set the Dests name tree in the name dictionary.
    *
    * This replaces any existing Dests tree with the provided one.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_SetDestinations(NameDictionaryHandle* handle, DestinationNameTreeHandle* destinations);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_ToUnknown(NameDictionaryHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref NameDictionaryHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_FromUnknown(IUnknownHandle* handle, NameDictionaryHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_Release(NameDictionaryHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_DICTIONARY_H */
