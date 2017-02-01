#ifndef _UTILS_FWD_H
#define _UTILS_FWD_H

namespace gotchangpdf {

template <typename T>
class Deferred;

template <typename T>
class DeferredContainer;

template <typename T>
class DeferredIterator;

class Buffer;
using BufferPtr = DeferredContainer<Buffer>;

// Exceptions
class ExceptionBase;
class GeneralException;
class ConversionException;

class IModifyObserver;
class IModifyObservable;

class IUnknown;
class WeakReferenceCounter;

template <typename T>
class WeakReference;

template <typename T>
class IWeakReferenceable;

} // gotchangpdf

#endif /* _UTILS_FWD_H */
