#include "precompiled.h"

#include "utils/exceptions.h"

namespace vanillapdf {

ExceptionBase::ExceptionBase(const std::string& msg) : m_msg(msg) {}
ExceptionBase::ExceptionBase(const char * const & msg) : m_msg(msg) {}
ExceptionBase::~ExceptionBase() {}

const char * ExceptionBase::what() const noexcept {
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

ZlibDataErrorException::ZlibDataErrorException(types::stream_size size) : ZlibDataErrorException(size, std::string()) {}
ZlibDataErrorException::ZlibDataErrorException(types::stream_size size, const std::string& message) : m_size(size),
ExceptionBase("Zlib encountered corrupted data after " + std::to_string(size) + " bytes. Error: " + message) {
}

InvalidLicenseException::InvalidLicenseException(const char * const & msg) : ExceptionBase(msg) {}
InvalidLicenseException::InvalidLicenseException(const std::string& msg) : ExceptionBase(msg) {}

} // vanillapdf

