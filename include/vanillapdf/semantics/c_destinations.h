#ifndef _C_DESTINATIONS_H
#define _C_DESTINATIONS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_destinations.h
	* \brief This file contains class definitions for \ref DestinationHandle and \ref NamedDestinationsHandle
	*/

	/**
	* \class DestinationHandle
	* \extends IUnknownHandle
	* \ingroup group_destinations
	* \brief Base class for all destinations
	*/

	/**
	* \class NamedDestinationsHandle
	* \extends IUnknownHandle
	* \ingroup group_destinations
	* \brief A map of unique names mapped to their associated a \ref DestinationHandle.
	*
	* For more details please visit [section 12.3.2.3 - Named Destinations](PDF32000_2008.pdf#G11.1947713).
	*/

	/**
	* \brief Available destination types.
	* \ingroup group_destinations
	*/
	typedef enum {
		DestinationType_Undefined = 0,

		/**
		* \brief
		* Display the page designated by page, with the coordinates
		* (left, top) positioned at the upper-left corner of the window
		* and the contents of the page magnified by the factor zoom.
		*/
		DestinationType_XYZ,

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
	* IntegerObjectHandle or IndirectReferenceObjectHandle.
	*
	* IntegerObjectHandle means index in document's page tree.
	*
	* IndirectReferenceObjectHandle means it is indirect reference to
	* \ref PageObjectHandle representing the destination page.
	* \see \ref PageTreeHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Destination_GetPageNumber(DestinationHandle* handle, ObjectHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Destination_Release(DestinationHandle* handle);

	/** @} */

	/**
	* \memberof NamedDestinationsHandle
	* @{
	*/

	/**
	* \brief Determine if current map contains \p name.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NamedDestinations_Contains(NamedDestinationsHandle* handle, const NameObjectHandle* name, boolean_type* result);

	/**
	* \brief Find mapped value for key \p name.
	*
	* Prefer using \ref NamedDestinations_Contains for validations.
	*
	* This function throws internal exception on failure,
	* which may render it slower.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NamedDestinations_Find(NamedDestinationsHandle* handle, const NameObjectHandle* name, DestinationHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NamedDestinations_Release(NamedDestinationsHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DESTINATIONS_H */
