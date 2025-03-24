#ifndef _C_CATALOG_H
#define _C_CATALOG_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#include "vanillapdf/utils/c_pdf_version.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_catalog.h
    * \brief This file contains class definitions for \ref CatalogHandle
    */

    /**
    * \class CatalogHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief The root of a document's object hierarchy
    * \see \ref DocumentHandle
    */

    /**
    * \brief Page layout which shall be used when the document is opened.
    */
    typedef enum {

        /**
        * \brief Undefined unitialized default value, triggers error when used
        */
        PageLayout_Undefined = 0,

        /**
        * \brief Display one page at a time
        */
        PageLayout_SinglePage,

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
    * \memberof CatalogHandle
    * @{
    */

    /**
    * \brief The root of the document's page tree (see 7.7.3, "Page Tree").
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetPages(CatalogHandle* handle, PageTreeHandle** result);

    /**
    * \brief A number tree defining the page labelling for the document (see 12.4.2, "Page Labels").
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetPageLabels(CatalogHandle* handle, PageLabelsHandle** result);

    /**
    * \copydoc PageLayout
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetPageLayout(CatalogHandle* handle, PageLayout* result);

    /**
    * \brief
    * The version of the PDF specification to which the document conforms
    * if later than the version specified in the file's header.
    *
    * If the header specifies a later version, or if this entry is absent,
    * the document shall conform to the version specified in the header.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetVersion(CatalogHandle* handle, PDFVersion* result);

    /**
    * \brief
    * An extensions dictionary containing developer prefix identification and
    * version numbers for developer extensions that occur in this document.
    *
    * 7.12, "Extensions Dictionary", describes this dictionary and how it shall be used.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetExtensions(CatalogHandle* handle, DeveloperExtensionsHandle** result);

    /**
    * \brief The way the document shall be displayed on the screen.
    *
    * If this entry is absent, conforming readers shall use their own current
    * user preference settings.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetViewerPreferences(CatalogHandle* handle, ViewerPreferencesHandle** result);

    /**
    * \brief Root of the document's outline hierarchy (see 12.3.3, "Document Outline").
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetOutlines(CatalogHandle* handle, OutlineHandle** result);

    /**
    * \brief
    * A dictionary of names and corresponding destinations
    * (see 12.3.2.3, "Named Destinations").
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetDestinations(CatalogHandle* handle, NamedDestinationsHandle** result);

    /**
    * \brief
    * The document's interactive form
    * (see 12.7, "Interactive Forms").
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetAcroForm(CatalogHandle* handle, InteractiveFormHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_ToUnknown(CatalogHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref CatalogHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_FromUnknown(IUnknownHandle* handle, CatalogHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Catalog_Release(CatalogHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CATALOG_H */
