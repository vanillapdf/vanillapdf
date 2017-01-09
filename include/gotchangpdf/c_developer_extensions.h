#ifndef _C_DEVELOPER_EXTENSIONS_H
#define _C_DEVELOPER_EXTENSIONS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#include "c_pdf_version.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_developer_extensions.h
	* This file contains class definitions for
	* DeveloperExtensionsHandle
	* DeveloperExtensionHandle
	* DeveloperExtensionsIteratorHandle.
	*/

	/**
	* \memberof DeveloperExtensionsIteratorHandle
	* @{
	*/

	/**
	* \brief Get key at iterator position
	*
	* Ensure the iterator is valid.
	* \see ::DeveloperExtensionsIterator_IsValid
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_GetKey(DeveloperExtensionsIteratorHandle handle, PNameObjectHandle result);

	/**
	* \brief Get value at iterator position
	*
	* Ensure the iterator is valid.
	* \see ::DeveloperExtensionsIterator_IsValid
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_GetValue(DeveloperExtensionsIteratorHandle handle, PDeveloperExtensionHandle result);

	/**
	* \brief Determine if current position is valid
	*
	* Invalid position may mean that iterator moved past the end of the collection, as well as the collection was modified.
	*
	* Any other operation except \b THIS will fail on invalid iterator.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_IsValid(DeveloperExtensionsIteratorHandle handle, DeveloperExtensionsHandle parent, out_boolean_type result);

	/**
	* \brief Advances iterator to the next position
	*
	* Ensure the iterator is valid.
	* \see ::DeveloperExtensionsIterator_IsValid
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_Next(DeveloperExtensionsIteratorHandle handle);

	/**
	* \copydoc IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_Release(DeveloperExtensionsIteratorHandle handle);

	/** @} */

	/**
	* \memberof DeveloperExtensionsHandle
	* @{
	*/

	/**
	* \brief Get collection iterator for enumerating all entries
	*
	* Modifying collection may invalidate this handle.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_Iterator(DeveloperExtensionsHandle handle, PDeveloperExtensionsIteratorHandle result);

	/**
	* \copydoc IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_Release(DeveloperExtensionsHandle handle);

	/** @} */

	/**
	* \memberof DeveloperExtensionHandle
	* @{
	*/

	/**
	* \brief The name of the PDF version to which this extension applies.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtension_GetBaseVersion(DeveloperExtensionHandle handle, PPDFVersion result);

	/**
	* \brief
	* An integer defined by the developer
	* to denote the extension being used.
	*
	* If the developer introduces more than one extension to a given
	* BaseVersion the extension level numbers assigned by that
	* developer shall increase over time.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtension_GetExtensionLevel(DeveloperExtensionHandle handle, PIntegerObjectHandle result);

	/**
	* \copydoc IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtension_Release(DeveloperExtensionHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DEVELOPER_EXTENSIONS_H */
