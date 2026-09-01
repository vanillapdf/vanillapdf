#include "precompiled.h"
#include "syntax/exceptions/syntax_exceptions.h"

namespace vanillapdf {
namespace syntax {

ParseException::ParseException(types::stream_offset offset)
    : ExceptionBase("Could not parse object at offset " + std::to_string(offset)) {
}
ParseException::ParseException(const char * const & msg) : ExceptionBase(msg) {}
ParseException::ParseException(const std::string& msg) : ExceptionBase(msg) {}

FileDisposedException::FileDisposedException() : ExceptionBase("Current file has been already disposed") {}

FileNotInitializedException::FileNotInitializedException(const char * const & filename)
    : ExceptionBase("File " + std::string(filename) + " has not been initialized") {
}
FileNotInitializedException::FileNotInitializedException(const std::string& filename)
    : FileNotInitializedException(filename.c_str()) {
}

ObjectMissingException::ObjectMissingException(types::big_uint objNumber)
    : ExceptionBase("Object " + std::to_string(objNumber) + " is missing") {
}

ObjectMissingException::ObjectMissingException(types::big_uint objNumber, types::ushort genNumber)
    : ExceptionBase("Object " + std::to_string(objNumber) + " " + std::to_string(genNumber) + " is missing") {
}
ObjectMissingException::ObjectMissingException(const char * const & msg) : ExceptionBase(msg) {}
ObjectMissingException::ObjectMissingException(const std::string& msg) : ExceptionBase(msg) {}

DuplicateKeyException::DuplicateKeyException(const char * const & msg) : ExceptionBase(msg) {}
DuplicateKeyException::DuplicateKeyException(const std::string& msg) : ExceptionBase(msg) {}

} // syntax
} // vanillapdf
