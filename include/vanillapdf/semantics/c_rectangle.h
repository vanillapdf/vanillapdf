#ifndef _C_RECTANGLE_H
#define _C_RECTANGLE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_rectangle.h
    * \brief This file contains class definitions for \ref RectangleHandle
    */

    /**
    * \class RectangleHandle
    * \extends IUnknownHandle
    * \ingroup group_common_data_structures
    * \brief Represents rectangle in 2D space
    */

    /**
    * \memberof RectangleHandle
    * @{
    */

    /**
    * \brief Create a new rectangle with default values
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_Create(RectangleHandle** result);

    /**
    * \brief Get X-coordinate of lower-left corner
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_GetLowerLeftX(RectangleHandle* handle, bigint_type* result);

    /**
    * \brief Set X-coordinate of lower-left corner
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_SetLowerLeftX(RectangleHandle* handle, bigint_type data);

    /**
    * \brief Get Y-coordinate of lower-left corner
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_GetLowerLeftY(RectangleHandle* handle, bigint_type* result);

    /**
    * \brief Set Y-coordinate of lower-left corner
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_SetLowerLeftY(RectangleHandle* handle, bigint_type data);

    /**
    * \brief Get X-coordinate of upper-right corner
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_GetUpperRightX(RectangleHandle* handle, bigint_type* result);

    /**
    * \brief Set X-coordinate of upper-right corner
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_SetUpperRightX(RectangleHandle* handle, bigint_type data);

    /**
    * \brief Get Y-coordinate of upper-right corner
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_GetUpperRightY(RectangleHandle* handle, bigint_type* result);

    /**
    * \brief Set Y-coordinate of upper-right corner
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_SetUpperRightY(RectangleHandle* handle, bigint_type data);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_ToUnknown(RectangleHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref ObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_FromUnknown(IUnknownHandle* handle, RectangleHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_Release(RectangleHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_RECTANGLE_H */
