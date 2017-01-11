#ifndef _C_RESOURCE_DICTIONARY_H
#define _C_RESOURCE_DICTIONARY_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_resource_dictionary.h
	* This file contains class definitions for ResourceDictionaryHandle
	*/

	/**
	* \class ResourceDictionaryHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* A content stream's named resources shall be defined
	* by a resource dictionary, which shall enumerate the
	* named resources needed by the operators in the
	* content stream and the names by which they can be referred to.
	* \see PageObjectHandle
	*/

	/**
	* \memberof ResourceDictionaryHandle
	* @{
	*/

	/**
	* \brief
	* A dictionary that maps resource names to font dictionaries.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ResourceDictionary_GetFontMap(ResourceDictionaryHandle handle, PFontMapHandle result);

	/**
	* \copydoc IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ResourceDictionary_Release(ResourceDictionaryHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_RESOURCE_DICTIONARY_H */
