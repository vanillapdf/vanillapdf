#ifndef _C_CATALOG_H
#define _C_CATALOG_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#include "c_pdf_version.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_catalog.h
	* This file contains class definitions for CatalogHandle
	*/

	/**
	* \brief Page layout which shall be used when the document is opened.
	*/
	typedef enum
	{
		/**
		* \brief Display one page at a time
		*/
		PageLayout_SinglePage = 0,

		/**
		* \brief Display the pages in one column
		*/
		PageLayout_OneColumn,

		/**
		* \brief Display the pages in two columns, with odd-numbered pages on the left
		*/
		PageLayout_TwoColumnLeft,

		/**
		* \brief Display the pages in two columns, with odd-numbered pages on the right
		*/
		PageLayout_TwoColumnRight,

		/**
		* \brief (PDF 1.5) Display the pages two at a time, with odd-numbered pages on the left
		*/
		PageLayout_TwoPageLeft,

		/**
		* \brief (PDF 1.5) Display the pages two at a time, with odd-numbered pages on the right
		*/
		PageLayout_TwoPageRight
	} PageLayout;

	/**
	* \brief Pointer to PageLayout
	*/
	typedef PageLayout *PPageLayout;

	/**
	* \memberof CatalogHandle
	* @{
	*/

	/**
	* \brief The root of the document's page tree (see 7.7.3, "Page Tree").
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPages(CatalogHandle handle, PPageTreeHandle result);

	/**
	* \brief A number tree defining the page labelling for the document (see 12.4.2, "Page Labels").
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPageLabels(CatalogHandle handle, PPageLabelsHandle result);

	/**
	* \copydoc PageLayout
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPageLayout(CatalogHandle handle, PPageLayout result);

	/**
	* \brief
	* The version of the PDF specification to which the document conforms
	* if later than the version specified in the file's header.
	*
	* If the header specifies a later version, or if this entry is absent,
	* the document shall conform to the version specified in the header.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetVersion(CatalogHandle handle, PPDFVersion result);

	/**
	* \brief
	* An extensions dictionary containing developer prefix identification and
	* version numbers for developer extensions that occur in this document.
	*
	* 7.12, "Extensions Dictionary", describes this dictionary and how it shall be used.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetExtensions(CatalogHandle handle, PDeveloperExtensionsHandle result);

	/**
	* \brief The way the document shall be displayed on the screen.
	*
	* If this entry is absent, conforming readers shall use their own current
	* user preference settings.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetViewerPreferences(CatalogHandle handle, PViewerPreferencesHandle result);

	/**
	* \brief Root of the document's outline hierarchy (see 12.3.3, "Document Outline").
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetOutlines(CatalogHandle handle, POutlineHandle result);

	/**
	* \brief
	* A dictionary of names and corresponding destinations
	* (see 12.3.2.3, "Named Destinations").
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetDestinations(CatalogHandle handle, PNamedDestinationsHandle result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_Release(CatalogHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CATALOG_H */
