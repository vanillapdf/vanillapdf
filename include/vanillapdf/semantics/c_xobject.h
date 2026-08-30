#ifndef _C_XOBJECT_H
#define _C_XOBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_xobject.h
    * \brief This file contains class definitions for \ref XObjectHandle and \ref FormXObjectHandle.
    */

    /**
    * \class XObjectHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * Base class for external objects - self-contained content streams
    * that can be painted from other content streams, such as form
    * XObjects and image XObjects.
    *
    * For more details please visit [section 8.8 - External Objects](PDF32000_2008.pdf#G7.3849285).
    */

    /**
    * \class FormXObjectHandle
    * \extends XObjectHandle
    * \ingroup group_documents
    * \brief
    * A form XObject is a PDF content stream that is a self-contained
    * description of any sequence of graphics objects. It may be painted
    * multiple times - either on several pages or at several locations
    * on the same page, and is also the carrier of annotation
    * appearance streams.
    *
    * For more details please visit [section 8.10 - Form XObjects](PDF32000_2008.pdf#G7.3849395).
    */

    /**
    * \brief Types of XObjects
    * \ingroup group_documents
    */
    typedef enum {
        XObjectType_Undefined = 0,

        /**
        * \copybrief FormXObjectHandle
        * \see \ref FormXObjectHandle
        */
        XObjectType_Form,

        /**
        * \brief
        * An image XObject represents a sampled image - a rectangular
        * array of sample values, each representing a colour.
        */
        XObjectType_Image,

        /**
        * \brief
        * A PostScript XObject contains PostScript language commands
        * (deprecated in PDF 2.0).
        */
        XObjectType_PostScript
    } XObjectType;

    /**
    * \memberof XObjectHandle
    * @{
    */

    /**
    * \brief Get derived type of current object.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION XObject_GetXObjectType(XObjectHandle* handle, XObjectType* result);

    /**
    * \brief
    * Switch object to low-level syntax API
    *
    * Useful for cases, where semantic API is not sufficient.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION XObject_GetBaseObject(XObjectHandle* handle, ObjectHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION XObject_ToUnknown(XObjectHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref XObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION XObject_FromUnknown(IUnknownHandle* handle, XObjectHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION XObject_Release(XObjectHandle* handle);

    /** @} */

    /**
    * \memberof FormXObjectHandle
    * @{
    */

    /**
    * \brief
    * Create an empty form XObject backed by a new content stream
    * registered as an indirect object within the document.
    *
    * Form XObjects are referenced indirectly (e.g. from an annotation's
    * appearance dictionary), so the stream has to own a cross-reference
    * entry. There is deliberately no parameterless create, as an
    * unregistered stream would produce a dangling reference that only
    * fails when the document is written.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_CreateFromDocument(DocumentHandle* handle, FormXObjectHandle** result);

    /**
    * \brief Get the bounding box (BBox entry) in form coordinate space
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_GetBoundingBox(FormXObjectHandle* handle, RectangleHandle** result);

    /**
    * \brief Set the bounding box (BBox entry) in form coordinate space
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_SetBoundingBox(FormXObjectHandle* handle, RectangleHandle* value);

    /**
    * \brief Get the transformation matrix (Matrix entry) mapping form space into user space
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_GetMatrix(FormXObjectHandle* handle, MatrixHandle** result);

    /**
    * \brief Set the transformation matrix (Matrix entry) mapping form space into user space
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_SetMatrix(FormXObjectHandle* handle, MatrixHandle* value);

    /**
    * \brief Get the resource dictionary (Resources entry) of the form
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_GetResources(FormXObjectHandle* handle, ResourceDictionaryHandle** result);

    /**
    * \brief Set the resource dictionary (Resources entry) of the form
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_SetResources(FormXObjectHandle* handle, ResourceDictionaryHandle* value);

    /**
    * \brief Reinterpret current object as \ref XObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_ToBaseXObject(FormXObjectHandle* handle, XObjectHandle** result);

    /**
    * \brief Convert \ref XObjectHandle to \ref FormXObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_FromBaseXObject(XObjectHandle* handle, FormXObjectHandle** result);

    /**
    * \copydoc XObject_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_Release(FormXObjectHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_XOBJECT_H */
