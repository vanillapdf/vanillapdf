#ifndef _IO_STRATEGY_H
#define _IO_STRATEGY_H

namespace vanillapdf {

/**
* \brief Specifies the I/O strategy used for file access.
*
* Controls how the library accesses file data:
* - Undefined: Caller must pick a concrete strategy (throws InvalidParameterException)
* - Memory: Loads entire file into an fmt::memory_buffer, all parsing happens from memory
* - MemoryMapped: Uses memory-mapped file access (not yet supported)
* - FileStream: Uses fstream-based streaming I/O (default for Open/Create)
*/
enum class IOStrategy {
    Undefined,
    Memory,
    MemoryMapped,
    FileStream
};

} // vanillapdf

#endif /* _IO_STRATEGY_H */
