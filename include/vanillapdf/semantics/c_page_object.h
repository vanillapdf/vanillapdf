#ifndef _C_PAGE_OBJECT_H
#define _C_PAGE_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_page_object.h
    * \brief This file contains class definitions for \ref PageObjectHandle
    */

    /**
    * \class PageObjectHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * The leaves of the page tree are page objects,
    * each of which is a dictionary specifying the
    * attributes of a single page of the document.
    * \see \ref PageTreeHandle
    */

    /**
    * \memberof PageObjectHandle
    * @{
    */

    /**
    * \brief Creates a new instance.
    *
    * Function creates a new indirect dictionary with it's type set to "Page".
    *
    * This new entry has to be manually added to document catalog's page tree.
    * \see \ref PageTreeHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_CreateFromDocument(DocumentHandle* handle, PageObjectHandle** result);

    /**
    * \brief
    * Create a new PageObject from existing low-level syntax API object.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_CreateFromObject(DictionaryObjectHandle* handle, PageObjectHandle** result);

    /**
    * \brief
    * A content stream (see 7.8.2, "Content Streams") that shall describe
    * the contents of this page. If this entry is absent, the page shall be empty.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetContents(PageObjectHandle* handle, PageContentsHandle** result);

    /**
    * \copydoc PageObject_GetContents
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetContents(PageObjectHandle* handle, PageContentsHandle* value);

    /**
    * \brief
    * The page tree node that is the immediate parent of this page object.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetParent(PageObjectHandle* handle, PageTreeHandle** result);

    /**
    * \brief
    * A dictionary containing any resources required
    * by the page (see 7.8.3, "Resource Dictionaries").
    *
    * If the page requires no resources,
    * the value of this entry shall be an empty dictionary.
    * Omitting the entry entirely indicates that the resources
    * shall be inherited from an ancestor node in the page tree.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetResources(PageObjectHandle* handle, ResourceDictionaryHandle** result);

    /**
    * \copydoc PageObject_GetResources
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetResources(PageObjectHandle* handle, ResourceDictionaryHandle* value);

    /**
    * \brief
    * An array of annotation dictionaries that shall contain
    * indirect references to all annotations associated
    * with the page (see 12.5, "Annotations").
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetAnnotations(PageObjectHandle* handle, PageAnnotationsHandle** result);

    /**
    * \brief
    * Set the annotation array for this page.
    * \param handle a handle to the page object
    * \param value the annotation collection to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetAnnotations(PageObjectHandle* handle, PageAnnotationsHandle* value);

    /**
    * \brief
    * A rectangle expressed in default user space units,
    * that shall define the boundaries of the
    * physical medium on which the page shall
    * be displayed or printed (see 14.11.2, "Page Boundaries").
    *
    * Only the page's own entry is read. The entry is inheritable,
    * but a value inherited from the page tree is not resolved.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetMediaBox(PageObjectHandle* handle, RectangleHandle** result);

    /**
    * \copydoc PageObject_GetMediaBox
    *
    * The setter always writes the entry on the page itself.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetMediaBox(PageObjectHandle* handle, RectangleHandle* value);

    /**
    * \brief
    * A rectangle expressed in default user space units,
    * that shall define the visible region of default user space.
    * When the page is displayed or printed, its contents shall be clipped
    * to this rectangle (see 14.11.2, "Page Boundaries").
    *
    * Only the page's own entry is read. The entry is inheritable,
    * but a value inherited from the page tree is not resolved.
    * When absent from the page and its ancestors, the media box applies.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetCropBox(PageObjectHandle* handle, RectangleHandle** result);

    /**
    * \copydoc PageObject_GetCropBox
    *
    * The setter always writes the entry on the page itself.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetCropBox(PageObjectHandle* handle, RectangleHandle* value);

    /**
    * \brief
    * A rectangle expressed in default user space units,
    * that shall define the region to which the contents of the page
    * shall be clipped when output in a production environment
    * (see 14.11.2, "Page Boundaries").
    *
    * When absent, the crop box applies.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetBleedBox(PageObjectHandle* handle, RectangleHandle** result);

    /**
    * \copydoc PageObject_GetBleedBox
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetBleedBox(PageObjectHandle* handle, RectangleHandle* value);

    /**
    * \brief
    * A rectangle expressed in default user space units,
    * that shall define the intended dimensions of the finished page
    * after trimming (see 14.11.2, "Page Boundaries").
    *
    * When absent, the crop box applies.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetTrimBox(PageObjectHandle* handle, RectangleHandle** result);

    /**
    * \copydoc PageObject_GetTrimBox
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetTrimBox(PageObjectHandle* handle, RectangleHandle* value);

    /**
    * \brief
    * A rectangle expressed in default user space units,
    * that shall define the extent of the page's meaningful content
    * as intended by the page's creator (see 14.11.2, "Page Boundaries").
    *
    * When absent, the crop box applies.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetArtBox(PageObjectHandle* handle, RectangleHandle** result);

    /**
    * \copydoc PageObject_GetArtBox
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetArtBox(PageObjectHandle* handle, RectangleHandle* value);

    /**
    * \brief
    * The number of degrees by which the page shall be rotated
    * clockwise when displayed or printed.
    *
    * The stored value is returned unmodified. The specification requires
    * a multiple of 90, but a file may contain a negative value, a value
    * of 360 or more, or a malformed one - interpreting it is left to the caller.
    *
    * Only the page's own entry is read. The entry is inheritable,
    * but a value inherited from the page tree is not resolved.
    * When absent from the page and its ancestors, the page is not rotated.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetRotate(PageObjectHandle* handle, bigint_type* result);

    /**
    * \copydoc PageObject_GetRotate
    *
    * The setter always writes the entry on the page itself and stores
    * the value as given. A value that is not a multiple of 90 is rejected.
    * \param handle a handle to the page object
    * \param data the rotation, which shall be a multiple of 90
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetRotate(PageObjectHandle* handle, bigint_type data);

    /**
    * \brief
    * A positive number that shall give the size of default user space units,
    * in multiples of 1/72 inch (see 14.11.6, "User Space").
    *
    * When absent, one unit is 1/72 inch.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetUserUnit(PageObjectHandle* handle, real_type* result);

    /**
    * \copydoc PageObject_GetUserUnit
    * \param handle a handle to the page object
    * \param data the unit size, which shall be positive
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetUserUnit(PageObjectHandle* handle, real_type data);

    /**
    * \brief
    * A group attributes dictionary that shall specify the attributes
    * of the page's page group for use in the transparent imaging model
    * (see 11.6.6, "Transparency Group XObjects").
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetGroup(PageObjectHandle* handle, TransparencyGroupHandle** result);

    /**
    * \copydoc PageObject_GetGroup
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetGroup(PageObjectHandle* handle, TransparencyGroupHandle* value);

    /**
    * \brief
    * Switch object to low-level syntax API
    * 
    * Useful for cases, where semantic API is not sufficient.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetBaseObject(PageObjectHandle* handle, DictionaryObjectHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_ToUnknown(PageObjectHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref PageObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_FromUnknown(IUnknownHandle* handle, PageObjectHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageObject_Release(PageObjectHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_PAGE_OBJECT_H */
