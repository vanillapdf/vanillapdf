#ifndef _IO_STRATEGY_H
#define _IO_STRATEGY_H

namespace vanillapdf {

/**
* \brief Specifies the I/O strategy used for file access.
*
* Controls how the library accesses file data:
* - Undefined: Uses the default implementation (currently fstream)
* - Memory: Loads entire file into memory (not yet supported)
* - MemoryMapped: Uses memory-mapped file access (not yet supported)
* - FileStream: Uses FILE*-based streaming I/O
*/
enum class IOStrategy {
    Undefined,
    Memory,
    MemoryMapped,
    FileStream
};

} // vanillapdf

#endif /* _IO_STRATEGY_H */
