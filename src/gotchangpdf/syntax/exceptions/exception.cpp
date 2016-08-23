#include "precompiled.h"
#include "exception.h"

namespace gotchangpdf
{

	ExceptionBase::ExceptionBase(const std::string& msg, Type type) : exception(msg.c_str()), _type(type) {}
	ExceptionBase::ExceptionBase(const char * const & msg, Type type) : exception(msg), _type(type) {}
	ExceptionBase::~ExceptionBase() {}

	GeneralException::GeneralException(const char * const & msg) : ExceptionBase(msg, Type::General) {}
	GeneralException::GeneralException(const std::string& msg) : ExceptionBase(msg, Type::General) {}

	NotSupportedException::NotSupportedException(const char * const & msg) : ExceptionBase(msg, Type::NotSupported) {}
	NotSupportedException::NotSupportedException(const std::string& msg) : ExceptionBase(msg, Type::NotSupported) {}

	UserCancelledException::UserCancelledException(const char * const & msg) : ExceptionBase(msg, Type::UserCancelled) {}
	UserCancelledException::UserCancelledException(const std::string& msg) : ExceptionBase(msg, Type::UserCancelled) {}

	namespace syntax
	{
		ParseException::ParseException(types::stream_offset offset)
			: ExceptionBase("Could not parse object at offset " + std::to_string(offset), Type::ParseException) {}

		ZlibDataErrorException::ZlibDataErrorException(types::stream_size size) : ZlibDataErrorException(size, std::string()) {}
		ZlibDataErrorException::ZlibDataErrorException(types::stream_size size, const std::string& message) : m_size(size),
			ExceptionBase("Zlib encountered corrupted data after " + std::to_string(size) + " bytes. Error: " + message, Type::ZlibDataError) {}

		FileDisposedException::FileDisposedException() : ExceptionBase("Current file has been already disposed", Type::FileDisposed) {}

		FileNotInitializedException::FileNotInitializedException(const char * const & filename)
			: ExceptionBase("File " + std::string(filename) + " has not been initialized", Type::FileNotInitialized) {}
		FileNotInitializedException::FileNotInitializedException(const std::string& filename)
			: FileNotInitializedException(filename.c_str()) {}

		ObjectMissingException::ObjectMissingException(types::big_uint objNumber)
			: ExceptionBase("Object " + std::to_string(objNumber) + " is missing", Type::ObjectMissing) {}

		ObjectMissingException::ObjectMissingException(types::big_uint objNumber, types::ushort genNumber)
			: ExceptionBase("Object " + std::to_string(objNumber) + " " + std::to_string(genNumber) + " is missing", Type::ObjectMissing) {}

		ConversionException::ConversionException(const char * const & msg) : ExceptionBase(msg, Type::Conversion) {}
		ConversionException::ConversionException(const std::string& msg) : ExceptionBase(msg, Type::Conversion) {}
	}
}

