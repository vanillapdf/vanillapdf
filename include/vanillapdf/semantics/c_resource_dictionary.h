#ifndef _C_RESOURCE_DICTIONARY_H
#define _C_RESOURCE_DICTIONARY_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_resource_dictionary.h
    * \brief This file contains class definitions for \ref ResourceDictionaryHandle
    */

    /**
    * \class ResourceDictionaryHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * A content stream's named resources shall be defined
    * by a resource dictionary, which shall enumerate the
    * named resources needed by the operators in the
    * content stream and the names by which they can be referred to.
    * \see \ref PageObjectHandle
    */

    /**
    * \memberof ResourceDictionaryHandle
    * @{
    */

    /**
    * \brief
    * A dictionary that maps resource names to font dictionaries.
    */
    /**
    * \brief
    * Create an empty resource dictionary.
    *
    * The resulting dictionary is a direct object and becomes part of the
    * document once it is attached through \ref PageObject_SetResources.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_Create(ResourceDictionaryHandle** result);

    /**
    * \brief
    * Create a resource dictionary from an existing low-level dictionary object.
    *
    * Use this overload to wrap a dictionary that is already registered
    * in the document, for example an indirect object obtained
    * through \ref File_AllocateNewEntry.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_CreateFromDictionary(DictionaryObjectHandle* handle, ResourceDictionaryHandle** result);

    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_GetFontMap(ResourceDictionaryHandle* handle, FontMapHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_ToUnknown(ResourceDictionaryHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref ResourceDictionaryHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_FromUnknown(IUnknownHandle* handle, ResourceDictionaryHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_Release(ResourceDictionaryHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_RESOURCE_DICTIONARY_H */
