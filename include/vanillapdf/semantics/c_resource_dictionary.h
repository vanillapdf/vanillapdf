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
    * \brief
    * Get the number of entries in the XObject sub-dictionary.
    *
    * The count is zero when the resource dictionary has no XObject entry.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_GetXObjectCount(ResourceDictionaryHandle* handle, size_type* result);

    /**
    * \brief
    * Get the resource name of the XObject entry at the specified position.
    * \param handle a handle to the resource dictionary
    * \param at position of the entry, valid within [0, \ref ResourceDictionary_GetXObjectCount)
    * \param result a pointer to variable that will contain the resource name upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_GetXObjectName(ResourceDictionaryHandle* handle, size_type at, NameObjectHandle** result);

    /**
    * \brief
    * Look up a named entry in the XObject sub-dictionary.
    *
    * Fails with \ref VANILLAPDF_ERROR_OBJECT_MISSING when the resource
    * dictionary has no XObject entry or the name is not present.
    * Check \ref XObject_GetXObjectType and convert the result to the
    * concrete type, e.g. via \ref FormXObject_FromBaseXObject.
    * \param handle a handle to the resource dictionary
    * \param name the resource name to look up
    * \param result a pointer to variable that will contain the XObject upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_FindXObject(ResourceDictionaryHandle* handle, const NameObjectHandle* name, XObjectHandle** result);

    /**
    * \brief
    * Register an XObject under the resource name, creating the XObject
    * sub-dictionary when missing and overwriting an existing entry.
    *
    * The entry stores an indirect reference, so the XObject's stream has
    * to be registered within the document - see \ref FormXObject_CreateFromDocument.
    * \param handle a handle to the resource dictionary
    * \param name the resource name to register the XObject under
    * \param value the XObject to register
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_AddXObject(ResourceDictionaryHandle* handle, const NameObjectHandle* name, XObjectHandle* value);

    /**
    * \brief
    * Remove a named entry from the XObject sub-dictionary.
    * \param handle a handle to the resource dictionary
    * \param name the resource name to remove
    * \param result a pointer to variable that will contain false when the
    * resource dictionary has no XObject entry or the name is not present
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_RemoveXObject(ResourceDictionaryHandle* handle, const NameObjectHandle* name, boolean_type* result);

    /**
    * \brief
    * Switch object to low-level syntax API
    *
    * Useful for cases, where semantic API is not sufficient.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_GetBaseObject(ResourceDictionaryHandle* handle, ObjectHandle** result);

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
