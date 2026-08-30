#ifndef _C_MATRIX_H
#define _C_MATRIX_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_matrix.h
    * \brief This file contains class definitions for \ref MatrixHandle
    */

    /**
    * \class MatrixHandle
    * \extends IUnknownHandle
    * \ingroup group_common_data_structures
    * \brief
    * Represents a transformation matrix [a b c d e f] mapping one
    * coordinate space into another.
    *
    * For more details please visit [section 8.3.3 - Common Transformations](PDF32000_2008.pdf#G7.3801630).
    */

    /**
    * \memberof MatrixHandle
    * @{
    */

    /**
    * \brief Create a new identity matrix [1 0 0 1 0 0]
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_Create(MatrixHandle** result);

    /**
    * \brief Get the A coefficient - horizontal scaling
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetA(MatrixHandle* handle, real_type* result);

    /**
    * \brief Set the A coefficient - horizontal scaling
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetA(MatrixHandle* handle, real_type data);

    /**
    * \brief Get the B coefficient - vertical skewing
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetB(MatrixHandle* handle, real_type* result);

    /**
    * \brief Set the B coefficient - vertical skewing
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetB(MatrixHandle* handle, real_type data);

    /**
    * \brief Get the C coefficient - horizontal skewing
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetC(MatrixHandle* handle, real_type* result);

    /**
    * \brief Set the C coefficient - horizontal skewing
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetC(MatrixHandle* handle, real_type data);

    /**
    * \brief Get the D coefficient - vertical scaling
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetD(MatrixHandle* handle, real_type* result);

    /**
    * \brief Set the D coefficient - vertical scaling
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetD(MatrixHandle* handle, real_type data);

    /**
    * \brief Get the E coefficient - horizontal translation
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetE(MatrixHandle* handle, real_type* result);

    /**
    * \brief Set the E coefficient - horizontal translation
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetE(MatrixHandle* handle, real_type data);

    /**
    * \brief Get the F coefficient - vertical translation
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetF(MatrixHandle* handle, real_type* result);

    /**
    * \brief Set the F coefficient - vertical translation
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetF(MatrixHandle* handle, real_type data);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_ToUnknown(MatrixHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref MatrixHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_FromUnknown(IUnknownHandle* handle, MatrixHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Matrix_Release(MatrixHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_MATRIX_H */
