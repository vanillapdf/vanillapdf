#ifndef _C_DESTINATIONS_H
#define _C_DESTINATIONS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_destinations.h
	* This file contains class definitions for DestinationHandle and NamedDestinationsHandle
	*/

	/**
	* \class DestinationHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* A destination defines a particular view of a document.
	*
	* Consisting of the following items:
	*
	* - The page of the document that shall be displayed.
	*
	* - The location of the document window on that page.
	*
	* - The magnification (zoom) factor.
	*/

	/**
	* \class NamedDestinationsHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* A name tree mapping name strings
	* to destinations (see 12.3.2.3, "Named Destinations").
	*/

	/**
	* \brief Available destination types.
	*/
	typedef enum {
		/**
		* \brief
		* Display the page designated by page, with the coordinates
		* (left, top) positioned at the upper-left corner of the window
		* and the contents of the page magnified by the factor zoom.
		*/
		DestinationType_XYZ = 0,

		/**
		* \brief
		* Display the page designated by page, with its contents magnified
		* just enough to fit the entire page within the window both
		* horizontally and vertically.
		*
		* If the required horizontal and vertical magnification
		* factors are different, use the smaller of the two,
		* centering the page within the window in the other dimension.
		*/
		DestinationType_Fit,

		/**
		* \brief
		* Display the page designated by page, with the vertical coordinate
		* top positioned at the top edge of the window and the contents of
		* the page magnified just enough to fit the entire width of the
		* page within the window.
		*/
		DestinationType_FitHorizontal,

		/**
		* \brief
		* Display the page designated by page, with the horizontal coordinate
		* left positioned at the left edge of the window and the contents of
		* the page magnified just enough to fit the entire height of the
		* page within the window.
		*/
		DestinationType_FitVertical,

		/**
		* \brief
		* Display the page designated by page, with its contents magnified
		* just enough to fit the rectangle specified by the coordinates
		* left, bottom, right, and top entirely within the window both
		* horizontally and vertically.
		*
		* If the required horizontal and vertical magnification
		* factors are different, use the smaller of the two,
		* centering the rectangle within the window in the other dimension.
		*/
		DestinationType_FitRectangle,

		/**
		* \brief
		* Display the page designated by page, with its contents magnified
		* just enough to fit its bounding box entirely within the window both
		* horizontally and vertically.
		*
		* If the required horizontal and vertical magnification
		* factors are different, use the smaller of the two,
		* centering the bounding box within the window in the other dimension.
		*/
		DestinationType_FitBoundingBox,

		/**
		* \brief
		* Display the page designated by page, with the vertical coordinate
		* top positioned at the top edge of the window and the contents
		* of the page magnified just enough to fit the entire width of
		* its bounding box within the window.
		*/
		DestinationType_FitBoundingBoxHorizontal,

		/**
		* \brief
		* Display the page designated by page, with the horizontal coordinate
		* left positioned at the left edge of the window and the contents
		* of the page magnified just enough to fit the entire height of
		* its bounding box within the window.
		*/
		DestinationType_FitBoundingBoxVertical,
	} DestinationType;

	/**
	* \memberof DestinationHandle
	* @{
	*/

	/**
	* \brief The page of the document that shall be displayed.
	*
	* The object type shall be either
	* IntegerObjectHandle or IndirectObjectReferenceHandle.
	*
	* IntegerObjectHandle means index in document's page tree.
	*
	* IndirectObjectReferenceHandle means it is indirect reference to
	* PageObjectHandle representing the destination page.
	* \see PageTreeHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Destination_GetPageNumber(DestinationHandle handle, ObjectHandle* result);

	/**
	* \copydoc IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Destination_Release(DestinationHandle handle);

	/** @} */

	/**
	* \memberof NamedDestinationsHandle
	* @{
	*/

	/**
	* \brief Determine if current map contains \p name.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NamedDestinations_Contains(NamedDestinationsHandle handle, NameObjectHandle name, boolean_type* result);

	/**
	* \brief Find mapped value for key \p name.
	*
	* Prefer using ::NamedDestinations_Contains for validations.
	*
	* This function throws internal exception on failure,
	* which may render it slower.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NamedDestinations_Find(NamedDestinationsHandle handle, NameObjectHandle name, DestinationHandle* result);

	/**
	* \copydoc IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NamedDestinations_Release(NamedDestinationsHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DESTINATIONS_H */
