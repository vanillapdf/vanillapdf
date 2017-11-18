#ifndef _C_DEVELOPER_EXTENSIONS_H
#define _C_DEVELOPER_EXTENSIONS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#include "vanillapdf/utils/c_pdf_version.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_developer_extensions.h
	* This file contains class definitions for developer extensions.
	*/

	/**
	* \class DeveloperExtensionsHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* The extensions dictionary, an entry in the document's catalog dictionary,
	* if present, shall contain one or more entries identifying
	* developer-defined extensions to the ISO 32000-1 Standard.
	* \see CatalogHandle
	*/

	/**
	* \class DeveloperExtensionsIteratorHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* Iterator concept allowing to traverse over
	* DeveloperExtensionsHandle collection.
	* \see DeveloperExtensionsHandle
	*/

	/**
	* \class DeveloperExtensionHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* Single item within DeveloperExtensionsHandle collection.
	* \see DeveloperExtensionsHandle
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
	VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_GetKey(DeveloperExtensionsIteratorHandle handle, NameObjectHandle* result);

	/**
	* \brief Get value at iterator position
	*
	* Ensure the iterator is valid.
	* \see ::DeveloperExtensionsIterator_IsValid
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_GetValue(DeveloperExtensionsIteratorHandle handle, DeveloperExtensionHandle* result);

	/**
	* \brief Determine if current position is valid
	*
	* Invalid position may mean that iterator moved past the end of the collection, as well as the collection was modified.
	*
	* Any other operation except \b THIS will fail on invalid iterator.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_IsValid(DeveloperExtensionsIteratorHandle handle, DeveloperExtensionsHandle parent, boolean_type* result);

	/**
	* \brief Advances iterator to the next position
	*
	* Ensure the iterator is valid.
	* \see ::DeveloperExtensionsIterator_IsValid
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_Next(DeveloperExtensionsIteratorHandle handle);

	/**
	* \copydoc IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_Release(DeveloperExtensionsIteratorHandle handle);

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
	VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensions_Iterator(DeveloperExtensionsHandle handle, DeveloperExtensionsIteratorHandle* result);

	/**
	* \copydoc IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtensions_Release(DeveloperExtensionsHandle handle);

	/** @} */

	/**
	* \memberof DeveloperExtensionHandle
	* @{
	*/

	/**
	* \brief The name of the PDF version to which this extension applies.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtension_GetBaseVersion(DeveloperExtensionHandle handle, PDFVersion* result);

	/**
	* \brief
	* An integer defined by the developer
	* to denote the extension being used.
	*
	* If the developer introduces more than one extension to a given
	* BaseVersion the extension level numbers assigned by that
	* developer shall increase over time.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtension_GetExtensionLevel(DeveloperExtensionHandle handle, IntegerObjectHandle* result);

	/**
	* \copydoc IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DeveloperExtension_Release(DeveloperExtensionHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DEVELOPER_EXTENSIONS_H */
