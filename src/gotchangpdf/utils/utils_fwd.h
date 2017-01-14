#ifndef _UTILS_FWD_H
#define _UTILS_FWD_H

namespace gotchangpdf
{
	template <typename T>
	class Deferred;

	template <typename T>
	class DeferredContainer;

	template <typename T>
	class DeferredIterator;

	template <typename T>
	class OutputPointer;

	class Buffer;
	using BufferPtr = DeferredContainer<Buffer>;

	// Exceptions
	class ExceptionBase;
	class GeneralException;
	class ConversionException;

	class IEncryptionKey;

	class IModifyObserver;
	class IModifyObservable;
}

#endif /* _UTILS_FWD_H */
