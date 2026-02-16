#ifndef _UTILS_FWD_H
#define _UTILS_FWD_H

namespace vanillapdf {

template <typename T>
class Deferred;

template <typename T>
class DeferredContainer;

template <typename T>
class DeferredIterator;

class Buffer;
using BufferPtr = DeferredContainer<Buffer>;

class BufferArray;
using BufferArrayPtr = DeferredContainer<BufferArray>;

// Exceptions
class ExceptionBase;
class GeneralException;
class ConversionException;

class PdfTextString; using PdfTextStringPtr = Deferred<PdfTextString>;
using OutputPdfTextStringPtr = PdfTextStringPtr&;

class IUnknown;
class WeakReferenceCounter;

template <typename T>
class WeakReference;

template <typename T>
class IWeakReferenceable;

template <typename T>
class BaseIterator;

class IEncryptionKey; using IEncryptionKeyPtr = Deferred<IEncryptionKey>;
class ISigningKey; using ISigningKeyPtr = Deferred<ISigningKey>;
class ISignatureVerifier; using ISignatureVerifierPtr = Deferred<ISignatureVerifier>;
class PKCS12Key; using PKCS12KeyPtr = Deferred<PKCS12Key>;
class TrustedCertificateStore; using TrustedCertificateStorePtr = Deferred<TrustedCertificateStore>;

// Signature verification
class SignatureVerificationResult; using SignatureVerificationResultPtr = Deferred<SignatureVerificationResult>;
class SignatureVerificationSettings; using SignatureVerificationSettingsPtr = Deferred<SignatureVerificationSettings>;

// Streams
class IInputStream; using IInputStreamPtr = Deferred<IInputStream>;
class InputStream; using InputStreamPtr = Deferred<InputStream>;
class InputReverseStream; using InputReverseStreamPtr = Deferred<InputReverseStream>;

class IOutputStream; using IOutputStreamPtr = Deferred<IOutputStream>;
class OutputStream; using OutputStreamPtr = Deferred<OutputStream>;
class MemoryBufferOutputStream; using MemoryBufferOutputStreamPtr = Deferred<MemoryBufferOutputStream>;

class IInputOutputStream; using IInputOutputStreamPtr = Deferred<IInputOutputStream>;
class InputOutputStream; using InputOutputStreamPtr = Deferred<InputOutputStream>;

class MemoryBufferInputStream; using MemoryBufferInputStreamPtr = Deferred<MemoryBufferInputStream>;
class MemoryBufferInputOutputStream; using MemoryBufferInputOutputStreamPtr = Deferred<MemoryBufferInputOutputStream>;

// STD wrappers
template <typename T, typename size_type_template>
class CustomSizeAllocator;

template <typename T, typename size_type_template>
class CustomSizeVector;

} // vanillapdf

#endif /* _UTILS_FWD_H */
