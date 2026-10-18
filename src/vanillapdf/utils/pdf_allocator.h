#ifndef _PDF_ALLOCATOR_H
#define _PDF_ALLOCATOR_H

#include <memory>

#if defined(VANILLAPDF_HAVE_MIMALLOC)
#include <mimalloc.h>
#endif

namespace vanillapdf {

/**
 * @brief The allocator of the containers the library allocates from most often.
 *
 * It follows the same rule as the operator new of IUnknown: the allocator is an
 * internal detail of a type the library owns end to end, so it never receives a
 * block that another module allocated. Containers that are handed out to, or
 * received from, a dependency keep the default allocator.
 *
 * Measured on the x64-windows-static-md Release build, as the median of 3
 * repetitions in each of 2 interleaved passes, relative to the default
 * allocator. Geometric mean over the 72 benchmarks:
 *
 *   IUnknown alone                      0.884
 *   IUnknown and the buffer storage     0.799
 *
 * The buffer carries the decoded streams and the string and name payloads, so
 * the paths that read them gain the most:
 *
 *   BM_StringGetValue_Literal/string_basic      0.844 -> 0.542
 *   BM_BaseFontRange_GetMappedValue_4byte       0.965 -> 0.537
 *   BM_HexadecimalStringObjectToPdf/unpaired    0.900 -> 0.599
 *   BM_DCTDecode_Large                          1.015 -> 0.733
 *
 * A full corpus run through vanillapdf.test is 0.896 of the default allocator,
 * against 0.904 for IUnknown alone - the process startup and the file IO of the
 * 282 documents hide the difference that the benchmarks above isolate.
 */
#if defined(VANILLAPDF_HAVE_MIMALLOC)

template <typename T>
using pdf_allocator = mi_stl_allocator<T>;

#else

template <typename T>
using pdf_allocator = std::allocator<T>;

#endif /* VANILLAPDF_HAVE_MIMALLOC */

} // vanillapdf

#endif /* _PDF_ALLOCATOR_H */
