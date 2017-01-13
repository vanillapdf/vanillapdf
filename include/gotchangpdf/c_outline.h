#ifndef _C_OUTLINE_H
#define _C_OUTLINE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_outline.h
	* This file contains contains class definitions for document outlines
	*/

	/**
	* \class OutlineBaseHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* Base class for document outlines
	* \see CatalogHandle
	*/

	/**
	* \class OutlineHandle
	* \extends OutlineBaseHandle
	* \ingroup Documents
	* \brief
	* The root of a document's outline hierarchy
	* \see CatalogHandle
	*/

	/**
	* \class OutlineItemHandle
	* \extends OutlineBaseHandle
	* \ingroup Documents
	* \brief
	* Child element within tree-structured hierarchy of outline items
	* \see CatalogHandle
	*/

	/**
	* \class OutlineItemColorHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* Representing the components in the DeviceRGB colour
	* space of the colour that shall be used for the outline entry's text.
	* \see OutlineItemHandle
	*/

	/**
	* \class OutlineItemFlagsHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* A set of flags specifying style characteristics
	* for displaying the outline item's text
	* \see OutlineItemHandle
	*/

	/**
	* \brief Available types of outlines.
	*/
	typedef enum
	{
		/**
		* \brief Object is type OutlineHandle
		*/
		OutlineType_Outline,

		/**
		* \brief Object is type OutlineItemHandle
		*/
		OutlineType_Item
	} OutlineType;

	/**
	* \brief Pointer to OutlineType
	*/
	typedef OutlineType *POutlineType;

	/**
	* \memberof OutlineBaseHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_GetType(OutlineBaseHandle handle, POutlineType result);

	/**
	* \brief Reinterpret current object as OutlineHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_ToOutline(OutlineBaseHandle handle, POutlineHandle result);

	/**
	* \brief Reinterpret current object as OutlineItemHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_ToOutlineItem(OutlineBaseHandle handle, POutlineItemHandle result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_Release(OutlineBaseHandle handle);

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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetFirst(OutlineHandle handle, POutlineItemHandle result);

	/**
	* \brief
	* An outline item dictionary representing
	* the last top-level item in the outline.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetLast(OutlineHandle handle, POutlineItemHandle result);

	/**
	* \brief
	* Total number of visible outline items at all levels of the outline.
	*
	* The value cannot be negative.
	*
	* This entry shall be omitted if there are no open outline items.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetCount(OutlineHandle handle, PIntegerObjectHandle result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_Release(OutlineHandle handle);

	/** @} */

	/**
	* \memberof OutlineItemHandle
	* @{
	*/

	/**
	* \brief
	* The text that shall be displayed on the screen for this item.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetTitle(OutlineItemHandle handle, PStringObjectHandle result);

	/**
	* \brief
	* The parent of this item in the outline hierarchy.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetParent(OutlineItemHandle handle, POutlineBaseHandle result);

	/**
	* \brief
	* The previous item at this outline level.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetPrev(OutlineItemHandle handle, POutlineItemHandle result);

	/**
	* \brief
	* The next item at this outline level.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetNext(OutlineItemHandle handle, POutlineItemHandle result);

	/**
	* \brief
	* The first of this item's immediate children in the outline hierarchy.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetFirst(OutlineItemHandle handle, POutlineItemHandle result);

	/**
	* \brief
	* The last of this item's immediate children in the outline hierarchy.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetLast(OutlineItemHandle handle, POutlineItemHandle result);

	/**
	* \brief
	* Sum of the number of visible descendent outline items at all levels.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetCount(OutlineItemHandle handle, PIntegerObjectHandle result);

	//GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetDestinations(OutlineItemHandle handle, PIntegerObjectHandle result);
	//GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetActions(OutlineItemHandle handle, PIntegerObjectHandle result);
	//GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetStructureElement(OutlineItemHandle handle, PIntegerObjectHandle result);

	/**
	* \brief \copybrief OutlineItemColorHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetColor(OutlineItemHandle handle, POutlineItemColorHandle result);

	/**
	* \brief \copybrief OutlineItemFlagsHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetFlags(OutlineItemHandle handle, POutlineItemFlagsHandle result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_Release(OutlineItemHandle handle);

	/** @} */

	/**
	* \memberof OutlineItemColorHandle
	* @{
	*/

	/**
	* \brief Get \b red component of defined RGB color
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetRed(OutlineItemColorHandle handle, PIntegerObjectHandle result);

	/**
	* \brief Get \b green component of defined RGB color
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetGreen(OutlineItemColorHandle handle, PIntegerObjectHandle result);

	/**
	* \brief Get \b blue component of defined RGB color
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetBlue(OutlineItemColorHandle handle, PIntegerObjectHandle result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_Release(OutlineItemColorHandle handle);

	/** @} */

	/**
	* \memberof OutlineItemFlagsHandle
	* @{
	*/

	/**
	* \brief Determine if the outline item shall be displayed in \a italic
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_IsItalic(OutlineItemFlagsHandle handle, out_boolean_type result);

	/**
	* \brief Determine if the outline item shall be displayed in \b bold
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_IsBold(OutlineItemFlagsHandle handle, out_boolean_type result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_Release(OutlineItemFlagsHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OUTLINE_H */
