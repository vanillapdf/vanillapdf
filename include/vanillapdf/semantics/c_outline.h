#ifndef _C_OUTLINE_H
#define _C_OUTLINE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_outline.h
    * \brief This file contains contains class definitions for document outlines
    */

    /**
    * \class OutlineBaseHandle
    * \extends IUnknownHandle
    * \ingroup group_outline
    * \brief
    * Base class for document outlines
    * \see \ref CatalogHandle
    */

    /**
    * \class OutlineHandle
    * \extends OutlineBaseHandle
    * \ingroup group_outline
    * \brief
    * The root of a document's outline hierarchy
    */

    /**
    * \class OutlineItemHandle
    * \extends OutlineBaseHandle
    * \ingroup group_outline
    * \brief
    * Child element within tree-structured hierarchy of outline items
    */

    /**
    * \class OutlineItemColorHandle
    * \extends IUnknownHandle
    * \ingroup group_outline
    * \brief
    * Representing the components in the DeviceRGB colour
    * space of the colour that shall be used for the outline entry's text.
    */

    /**
    * \class OutlineItemFlagsHandle
    * \extends IUnknownHandle
    * \ingroup group_outline
    * \brief
    * A set of flags specifying style characteristics
    * for displaying the outline item's text
    */

    /**
    * \brief Available types of outlines.
    * \ingroup group_outline
    */
    typedef enum {
        OutlineType_Undefined = 0,

        /**
        * \copybrief OutlineHandle
        * \see \ref OutlineHandle
        */
        OutlineType_Outline,

        /**
        * \copybrief OutlineItemHandle
        * \see \ref OutlineItemHandle
        */
        OutlineType_Item
    } OutlineType;

    /**
    * \memberof OutlineBaseHandle
    * @{
    */

    /**
    * \brief Get derived type of current object
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineBase_GetOutlineType(OutlineBaseHandle* handle, OutlineType* result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineBase_ToUnknown(OutlineBaseHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref CatalogHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineBase_FromUnknown(IUnknownHandle* handle, OutlineBaseHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineBase_Release(OutlineBaseHandle* handle);

    /** @} */

    /**
    * \memberof OutlineHandle
    * @{
    */

    /**
    * \brief
    * An outline item dictionary representing
    * the first top-level item in the outline.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Outline_GetFirst(OutlineHandle* handle, OutlineItemHandle** result);

    /**
    * \brief
    * An outline item dictionary representing
    * the last top-level item in the outline.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Outline_GetLast(OutlineHandle* handle, OutlineItemHandle** result);

    /**
    * \brief
    * Total number of visible outline items at all levels of the outline.
    *
    * The value cannot be negative.
    *
    * This entry shall be omitted if there are no open outline items.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Outline_GetCount(OutlineHandle* handle, IntegerObjectHandle** result);

    /**
    * \brief Reinterpret current object as \ref OutlineBaseHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Outline_ToOutlineBase(OutlineHandle* handle, OutlineBaseHandle** result);

    /**
    * \brief Convert \ref OutlineBaseHandle to \ref OutlineItemHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Outline_FromOutlineBase(OutlineBaseHandle* handle, OutlineHandle** result);

    /**
    * \copydoc OutlineBase_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Outline_Release(OutlineHandle* handle);

    /** @} */

    /**
    * \memberof OutlineItemHandle
    * @{
    */

    /**
    * \brief
    * The text that shall be displayed on the screen for this item.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetTitle(OutlineItemHandle* handle, StringObjectHandle** result);

    /**
    * \brief
    * The parent of this item in the outline hierarchy.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetParent(OutlineItemHandle* handle, OutlineBaseHandle** result);

    /**
    * \brief
    * The previous item at this outline level.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetPrev(OutlineItemHandle* handle, OutlineItemHandle** result);

    /**
    * \brief
    * The next item at this outline level.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetNext(OutlineItemHandle* handle, OutlineItemHandle** result);

    /**
    * \brief
    * The first of this item's immediate children in the outline hierarchy.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetFirst(OutlineItemHandle* handle, OutlineItemHandle** result);

    /**
    * \brief
    * The last of this item's immediate children in the outline hierarchy.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetLast(OutlineItemHandle* handle, OutlineItemHandle** result);

    /**
    * \brief
    * Sum of the number of visible descendent outline items at all levels.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetCount(OutlineItemHandle* handle, IntegerObjectHandle** result);

    //VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetDestinations(OutlineItemHandle* handle, IntegerObjectHandle** result);
    //VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetActions(OutlineItemHandle* handle, IntegerObjectHandle** result);
    //VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetStructureElement(OutlineItemHandle* handle, IntegerObjectHandle** result);

    /**
    * \brief \copybrief OutlineItemColorHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetColor(OutlineItemHandle* handle, OutlineItemColorHandle** result);

    /**
    * \brief \copybrief OutlineItemFlagsHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_GetFlags(OutlineItemHandle* handle, OutlineItemFlagsHandle** result);

    /**
    * \brief Reinterpret current object as \ref OutlineBaseHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_ToOutlineBase(OutlineItemHandle* handle, OutlineBaseHandle** result);

    /**
    * \brief Convert \ref OutlineBaseHandle to \ref OutlineItemHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_FromOutlineBase(OutlineBaseHandle* handle, OutlineItemHandle** result);

    /**
    * \copydoc OutlineBase_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItem_Release(OutlineItemHandle* handle);

    /** @} */

    /**
    * \memberof OutlineItemColorHandle
    * @{
    */

    /**
    * \brief Get \b red component of defined RGB color
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItemColor_GetRed(OutlineItemColorHandle* handle, IntegerObjectHandle** result);

    /**
    * \brief Get \b green component of defined RGB color
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItemColor_GetGreen(OutlineItemColorHandle* handle, IntegerObjectHandle** result);

    /**
    * \brief Get \b blue component of defined RGB color
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItemColor_GetBlue(OutlineItemColorHandle* handle, IntegerObjectHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItemColor_Release(OutlineItemColorHandle* handle);

    /** @} */

    /**
    * \memberof OutlineItemFlagsHandle
    * @{
    */

    /**
    * \brief Determine if the outline item shall be displayed in \a italic
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItemFlags_IsItalic(OutlineItemFlagsHandle* handle, boolean_type* result);

    /**
    * \brief Determine if the outline item shall be displayed in \b bold
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItemFlags_IsBold(OutlineItemFlagsHandle* handle, boolean_type* result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION OutlineItemFlags_Release(OutlineItemFlagsHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OUTLINE_H */
