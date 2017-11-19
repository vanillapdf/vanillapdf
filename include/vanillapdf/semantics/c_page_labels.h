#ifndef _C_PAGE_LABELS_H
#define _C_PAGE_LABELS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_page_labels.h
	* This file contains class definitions for
	* \ref PageLabelsHandle and \ref PageLabelHandle.
	*/

	/**
	* \class PageLabelsHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* Collection of \ref PageLabelHandle
	* \see \ref CatalogHandle
	*/

	/**
	* \class PageLabelHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* Page labels are used for numbering or otherwise identifying
	* individual pages (see 12.4.2, "Page Labels").
	* \see \ref PageLabelsHandle
	*/

	/**
	* \brief Numbering style types.
	*/
	typedef enum {
		/**
		* \brief Decimal arabic numerals.
		*/
		NumberingStyle_Decimal = 0,

		/**
		* \brief Uppercase roman numerals.
		*/
		NumberingStyle_UpperRoman,

		/**
		* \brief Lowercase roman numerals.
		*/
		NumberingStyle_LowerRoman,

		/**
		* \brief
		* Uppercase letters
		* (A to Z for the first 26 pages,
		* AA to ZZ for the next 26, and so on).
		*/
		NumberingStyle_UpperLetters,

		/**
		* \brief
		* Lowercase letters
		* (a to z for the first 26 pages,
		* aa to zz for the next 26, and so on).
		*/
		NumberingStyle_LowerLetters
	} NumberingStyle;

	/**
	* \memberof PageLabelsHandle
	* @{
	*/

	/**
	* \brief
	* Detemines if collection contains
	* label for page number \p page_number.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageLabels_Contains(PageLabelsHandle handle, size_type page_number, boolean_type* result);

	/**
	* \brief
	* Get page label for page number \p page_number.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageLabels_At(PageLabelsHandle handle, size_type page_number, PageLabelHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageLabels_Release(PageLabelsHandle handle);

	/** @} */

	/**
	* \memberof PageLabelHandle
	* @{
	*/

	/**
	* \brief The label prefix for page labels in this range.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageLabel_Prefix(PageLabelHandle handle, StringObjectHandle* result);

	/**
	* \brief
	* The value of the numeric portion for the first page label in the range.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageLabel_Start(PageLabelHandle handle, IntegerObjectHandle* result);

	/**
	* \brief
	* The numbering style that shall be used for
	* the numeric portion of each page label.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageLabel_Style(PageLabelHandle handle, NumberingStyle* result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageLabel_Release(PageLabelHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_PAGE_LABELS_H */
