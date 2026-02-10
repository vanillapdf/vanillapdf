#include "precompiled.h"
#include "utils/unknown_interface.h"

#include "vanillapdf/utils/c_object_diagnostics.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION ObjectDiagnostics_GetActiveObjectCount(bigint_type* result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = IUnknown::ActiveObjectCount();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectDiagnostics_GetPeakObjectCount(bigint_type* result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = IUnknown::PeakObjectCount();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectDiagnostics_GetTotalObjectsCreated(bigint_type* result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = IUnknown::TotalObjectsCreated();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectDiagnostics_ResetCounters() {
    try {
        IUnknown::ResetDiagnosticCounters();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}
