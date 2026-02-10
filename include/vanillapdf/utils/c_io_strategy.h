#ifndef _C_IO_STRATEGY_H
#define _C_IO_STRATEGY_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_io_strategy.h
    * \brief This file contains definitions for \ref IOStrategyType
    */

    /**
    * \brief Specifies the I/O strategy used for file access.
    * \ingroup group_utils
    *
    * Controls how the library accesses file data:
    * - IOStrategy_Undefined: Uses the default implementation (currently fstream)
    * - IOStrategy_Memory: Loads entire file into memory (not yet supported)
    * - IOStrategy_MemoryMapped: Uses memory-mapped file access (not yet supported)
    * - IOStrategy_FileStream: Uses FILE*-based streaming I/O
    */
    typedef enum {

        /**
        * \brief Uses the default implementation (currently fstream).
        */
        IOStrategy_Undefined = 0,

        /**
        * \brief Loads entire file into memory.
        *
        * Not yet supported, returns NotSupported error.
        */
        IOStrategy_Memory,

        /**
        * \brief Uses memory-mapped file access.
        *
        * Not yet supported, returns NotSupported error.
        */
        IOStrategy_MemoryMapped,

        /**
        * \brief Uses FILE*-based streaming I/O.
        */
        IOStrategy_FileStream,
    } IOStrategyType;

#ifdef __cplusplus
};
#endif

#endif /* _C_IO_STRATEGY_H */
