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

template <typename T>
class BaseIterator;

// Attributes
class IAttribute;
class IAttributePtr;
class AttributeList;

class IEncryptionKey; using IEncryptionKeyPtr = Deferred<IEncryptionKey>;
class ISigningKey; using ISigningKeyPtr = Deferred<ISigningKey>;
class PKCS12Key; using PKCS12KeyPtr = Deferred<PKCS12Key>;

// Streams
class IInputStream; using IInputStreamPtr = Deferred<IInputStream>;
class InputStream; using InputStreamPtr = Deferred<InputStream>;
class InputReverseStream; using InputReverseStreamPtr = Deferred<InputReverseStream>;

class IOutputStream; using IOutputStreamPtr = Deferred<IOutputStream>;
class OutputStream; using OutputStreamPtr = Deferred<OutputStream>;

class IInputOutputStream; using IInputOutputStreamPtr = Deferred<IInputOutputStream>;
class InputOutputStream; using InputOutputStreamPtr = Deferred<InputOutputStream>;

// STD wrappers
template <typename T, typename size_type_template>
class CustomSizeAllocator;

template <typename T, typename size_type_template>
class CustomSizeVector;

} // gotchangpdf

#endif /* _UTILS_FWD_H */
