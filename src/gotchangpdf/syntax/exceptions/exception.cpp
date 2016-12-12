#include "precompiled.h"
#include "exception.h"

namespace gotchangpdf
{
	ExceptionBase::ExceptionBase(const std::string& msg) : m_msg(msg) {}
	ExceptionBase::ExceptionBase(const char * const & msg) : m_msg(msg) {}
	ExceptionBase::~ExceptionBase() {}

	const char * ExceptionBase::what() const noexcept
	{
		return m_msg.c_str();
	}

	GeneralException::GeneralException(const char * const & msg) : ExceptionBase(msg) {}
	GeneralException::GeneralException(const std::string& msg) : ExceptionBase(msg) {}

	NotSupportedException::NotSupportedException(const char * const & msg) : ExceptionBase(msg) {}
	NotSupportedException::NotSupportedException(const std::string& msg) : ExceptionBase(msg) {}

	UserCancelledException::UserCancelledException(const char * const & msg) : ExceptionBase(msg) {}
	UserCancelledException::UserCancelledException(const std::string& msg) : ExceptionBase(msg) {}

	ConversionException::ConversionException(const char * const & msg) : ExceptionBase(msg) {}
	ConversionException::ConversionException(const std::string& msg) : ExceptionBase(msg) {}

	namespace syntax
	{
		ParseException::ParseException(types::stream_offset offset)
			: ExceptionBase("Could not parse object at offset " + std::to_string(offset)) {}

		ZlibDataErrorException::ZlibDataErrorException(types::stream_size size) : ZlibDataErrorException(size, std::string()) {}
		ZlibDataErrorException::ZlibDataErrorException(types::stream_size size, const std::string& message) : m_size(size),
			ExceptionBase("Zlib encountered corrupted data after " + std::to_string(size) + " bytes. Error: " + message) {}

		FileDisposedException::FileDisposedException() : ExceptionBase("Current file has been already disposed") {}

		FileNotInitializedException::FileNotInitializedException(const char * const & filename)
			: ExceptionBase("File " + std::string(filename) + " has not been initialized") {}
		FileNotInitializedException::FileNotInitializedException(const std::string& filename)
			: FileNotInitializedException(filename.c_str()) {}

		ObjectMissingException::ObjectMissingException(types::big_uint objNumber)
			: ExceptionBase("Object " + std::to_string(objNumber) + " is missing") {}

		ObjectMissingException::ObjectMissingException(types::big_uint objNumber, types::ushort genNumber)
			: ExceptionBase("Object " + std::to_string(objNumber) + " " + std::to_string(genNumber) + " is missing") {}
	}
}

