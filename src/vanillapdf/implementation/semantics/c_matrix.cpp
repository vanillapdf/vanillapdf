#include "precompiled.h"
#include "semantics/objects/matrix.h"

#include "vanillapdf/semantics/c_matrix.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_Create(MatrixHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        MatrixPtr matrix;
        auto ptr = matrix.AddRefGet();
        *result = reinterpret_cast<MatrixHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetA(MatrixHandle* handle, real_type* result)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetA();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetA(MatrixHandle* handle, real_type data)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->SetA(data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetB(MatrixHandle* handle, real_type* result)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetB();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetB(MatrixHandle* handle, real_type data)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->SetB(data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetC(MatrixHandle* handle, real_type* result)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetC();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetC(MatrixHandle* handle, real_type data)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->SetC(data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetD(MatrixHandle* handle, real_type* result)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetD();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetD(MatrixHandle* handle, real_type data)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->SetD(data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetE(MatrixHandle* handle, real_type* result)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetE();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetE(MatrixHandle* handle, real_type data)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->SetE(data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_GetF(MatrixHandle* handle, real_type* result)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetF();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_SetF(MatrixHandle* handle, real_type data)
{
    Matrix* obj = reinterpret_cast<Matrix*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->SetF(data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_ToUnknown(MatrixHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<Matrix, IUnknown, MatrixHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_FromUnknown(IUnknownHandle* handle, MatrixHandle** result) {
    return SafeObjectConvert<IUnknown, Matrix, IUnknownHandle, MatrixHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Matrix_Release(MatrixHandle* handle) {
    return ObjectRelease<Matrix, MatrixHandle>(handle);
}
