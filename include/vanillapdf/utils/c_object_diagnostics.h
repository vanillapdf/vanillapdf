#ifndef _C_OBJECT_DIAGNOSTICS_H
#define _C_OBJECT_DIAGNOSTICS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_object_diagnostics.h
    * \brief Runtime diagnostics for tracking live object counts
    */

    /**
    * \class ObjectDiagnostics
    * \ingroup group_utils
    * \brief
    * Provides runtime introspection of the object lifecycle.
    *
    * All objects in the library derive from IUnknown and are reference-counted.
    * This class exposes thread-safe counters that track the number of live objects,
    * which can be used to detect memory leaks or monitor allocation patterns.
    *
    * Typical usage: check that ActiveObjectCount returns to its baseline
    * after releasing all handles to verify that no objects were leaked.
    */

    /**
    * \memberof ObjectDiagnostics
    * @{
    */

    /**
    * \brief Get the number of currently live IUnknown-derived objects.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ObjectDiagnostics_GetActiveObjectCount(bigint_type* result);

    /**
    * \brief Get the peak number of simultaneously live objects since startup or last reset.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ObjectDiagnostics_GetPeakObjectCount(bigint_type* result);

    /**
    * \brief Get the total number of objects created since startup or last reset.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ObjectDiagnostics_GetTotalObjectsCreated(bigint_type* result);

    /**
    * \brief Reset peak and total counters to current active count.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ObjectDiagnostics_ResetCounters();

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_DIAGNOSTICS_H */
