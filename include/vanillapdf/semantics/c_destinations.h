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
    * \class XYZDestinationHandle
    * \extends DestinationHandle
    * \ingroup group_destinations
    * \brief XYZ destination with optional left, top, and zoom parameters
    */

    /**
    * \class FitDestinationHandle
    * \extends DestinationHandle
    * \ingroup group_destinations
    * \brief Fit destination that fits the entire page within the window
    */

    /**
    * \class FitHorizontalDestinationHandle
    * \extends DestinationHandle
    * \ingroup group_destinations
    * \brief FitH destination with vertical coordinate at top edge
    */

    /**
    * \class FitVerticalDestinationHandle
    * \extends DestinationHandle
    * \ingroup group_destinations
    * \brief FitV destination with horizontal coordinate at left edge
    */

    /**
    * \class FitRectangleDestinationHandle
    * \extends DestinationHandle
    * \ingroup group_destinations
    * \brief FitR destination that fits specified rectangle within window
    */

    /**
    * \class FitBoundingBoxDestinationHandle
    * \extends DestinationHandle
    * \ingroup group_destinations
    * \brief FitB destination that fits bounding box within window
    */

    /**
    * \class FitBoundingBoxHorizontalDestinationHandle
    * \extends DestinationHandle
    * \ingroup group_destinations
    * \brief FitBH destination with vertical coordinate at top of bounding box
    */

    /**
    * \class FitBoundingBoxVerticalDestinationHandle
    * \extends DestinationHandle
    * \ingroup group_destinations
    * \brief FitBV destination with horizontal coordinate at left of bounding box
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
    * \brief Get the type of destination to determine available parameters.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Destination_GetDestinationType(DestinationHandle* handle, DestinationType* result);

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

    /** @} */

    /**
    * \memberof XYZDestinationHandle
    * @{
    */

    /**
    * \brief Convert destination to XYZ destination if the type matches.
    * \return VANILLAPDF_ERROR_SUCCESS if destination is XYZ type, error otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_FromDestination(DestinationHandle* handle, XYZDestinationHandle** result);

    /**
    * \brief Convert XYZ destination back to base destination.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_ToDestination(XYZDestinationHandle* handle, DestinationHandle** result);

    /**
    * \brief Get the left coordinate for XYZ destination (null means no change).
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if left coordinate is null, VANILLAPDF_ERROR_SUCCESS otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_GetLeft(XYZDestinationHandle* handle, ObjectHandle** result);

    /**
    * \brief Get the top coordinate for XYZ destination (null means no change).
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if top coordinate is null, VANILLAPDF_ERROR_SUCCESS otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_GetTop(XYZDestinationHandle* handle, ObjectHandle** result);

    /**
    * \brief Get the zoom factor for XYZ destination (null means no change).
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if zoom factor is null, VANILLAPDF_ERROR_SUCCESS otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_GetZoom(XYZDestinationHandle* handle, ObjectHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_Release(XYZDestinationHandle* handle);

    /** @} */

    /**
    * \memberof FitDestinationHandle
    * @{
    */

    /**
    * \brief Convert destination to Fit destination if the type matches.
    * \return VANILLAPDF_ERROR_SUCCESS if destination is Fit type, error otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FitDestination_FromDestination(DestinationHandle* handle, FitDestinationHandle** result);

    /**
    * \brief Convert Fit destination back to base destination.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FitDestination_ToDestination(FitDestinationHandle* handle, DestinationHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FitDestination_Release(FitDestinationHandle* handle);

    /** @} */

    /**
    * \memberof FitHorizontalDestinationHandle
    * @{
    */

    VANILLAPDF_API error_type CALLING_CONVENTION FitHorizontalDestination_FromDestination(DestinationHandle* handle, FitHorizontalDestinationHandle** result);
    VANILLAPDF_API error_type CALLING_CONVENTION FitHorizontalDestination_ToDestination(FitHorizontalDestinationHandle* handle, DestinationHandle** result);

    /**
    * \brief Get the top coordinate for FitHorizontal destination (null means no change).
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if top coordinate is null, VANILLAPDF_ERROR_SUCCESS otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FitHorizontalDestination_GetTop(FitHorizontalDestinationHandle* handle, ObjectHandle** result);

    VANILLAPDF_API error_type CALLING_CONVENTION FitHorizontalDestination_Release(FitHorizontalDestinationHandle* handle);

    /** @} */

    /**
    * \memberof FitVerticalDestinationHandle
    * @{
    */

    VANILLAPDF_API error_type CALLING_CONVENTION FitVerticalDestination_FromDestination(DestinationHandle* handle, FitVerticalDestinationHandle** result);
    VANILLAPDF_API error_type CALLING_CONVENTION FitVerticalDestination_ToDestination(FitVerticalDestinationHandle* handle, DestinationHandle** result);

    /**
    * \brief Get the left coordinate for FitVertical destination (null means no change).
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if left coordinate is null, VANILLAPDF_ERROR_SUCCESS otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FitVerticalDestination_GetLeft(FitVerticalDestinationHandle* handle, ObjectHandle** result);

    VANILLAPDF_API error_type CALLING_CONVENTION FitVerticalDestination_Release(FitVerticalDestinationHandle* handle);

    /** @} */

    /**
    * \memberof FitRectangleDestinationHandle
    * @{
    */

    VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_FromDestination(DestinationHandle* handle, FitRectangleDestinationHandle** result);
    VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_ToDestination(FitRectangleDestinationHandle* handle, DestinationHandle** result);

    /**
    * \brief Get the left coordinate for FitRectangle destination.
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if left coordinate is null, VANILLAPDF_ERROR_SUCCESS otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_GetLeft(FitRectangleDestinationHandle* handle, ObjectHandle** result);

    /**
    * \brief Get the bottom coordinate for FitRectangle destination.
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if bottom coordinate is null, VANILLAPDF_ERROR_SUCCESS otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_GetBottom(FitRectangleDestinationHandle* handle, ObjectHandle** result);

    /**
    * \brief Get the right coordinate for FitRectangle destination.
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if right coordinate is null, VANILLAPDF_ERROR_SUCCESS otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_GetRight(FitRectangleDestinationHandle* handle, ObjectHandle** result);

    /**
    * \brief Get the top coordinate for FitRectangle destination.
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if top coordinate is null, VANILLAPDF_ERROR_SUCCESS otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_GetTop(FitRectangleDestinationHandle* handle, ObjectHandle** result);

    VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_Release(FitRectangleDestinationHandle* handle);

    /** @} */

    /**
    * \memberof FitBoundingBoxDestinationHandle
    * @{
    */

    VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxDestination_FromDestination(DestinationHandle* handle, FitBoundingBoxDestinationHandle** result);
    VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxDestination_ToDestination(FitBoundingBoxDestinationHandle* handle, DestinationHandle** result);
    VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxDestination_Release(FitBoundingBoxDestinationHandle* handle);

    /** @} */

    /**
    * \memberof FitBoundingBoxHorizontalDestinationHandle
    * @{
    */

    VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxHorizontalDestination_FromDestination(DestinationHandle* handle, FitBoundingBoxHorizontalDestinationHandle** result);
    VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxHorizontalDestination_ToDestination(FitBoundingBoxHorizontalDestinationHandle* handle, DestinationHandle** result);

    /**
    * \brief Get the top coordinate for FitBoundingBoxHorizontal destination (null means no change).
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if top coordinate is null, VANILLAPDF_ERROR_SUCCESS otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxHorizontalDestination_GetTop(FitBoundingBoxHorizontalDestinationHandle* handle, ObjectHandle** result);

    VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxHorizontalDestination_Release(FitBoundingBoxHorizontalDestinationHandle* handle);

    /** @} */

    /**
    * \memberof FitBoundingBoxVerticalDestinationHandle
    * @{
    */

    VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxVerticalDestination_FromDestination(DestinationHandle* handle, FitBoundingBoxVerticalDestinationHandle** result);
    VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxVerticalDestination_ToDestination(FitBoundingBoxVerticalDestinationHandle* handle, DestinationHandle** result);

    /**
    * \brief Get the left coordinate for FitBoundingBoxVertical destination (null means no change).
    * \return VANILLAPDF_ERROR_OBJECT_MISSING if left coordinate is null, VANILLAPDF_ERROR_SUCCESS otherwise
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxVerticalDestination_GetLeft(FitBoundingBoxVerticalDestinationHandle* handle, ObjectHandle** result);

    VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxVerticalDestination_Release(FitBoundingBoxVerticalDestinationHandle* handle);

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
