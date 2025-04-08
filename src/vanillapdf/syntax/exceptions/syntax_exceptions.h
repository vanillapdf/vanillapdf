#ifndef _SYNTAX_EXCEPTIONS_H
#define _SYNTAX_EXCEPTIONS_H

#include "utils/exceptions.h"

namespace vanillapdf {
namespace syntax {

class ParseException : public ExceptionBase {
public:
    explicit ParseException(types::stream_offset offset);
    virtual Type code() const noexcept { return Type::ParseException; }
};

class FileDisposedException : public ExceptionBase {
public:
    FileDisposedException();
    virtual Type code() const noexcept { return Type::FileDisposed; }
};

class FileNotInitializedException : public ExceptionBase {
public:
    explicit FileNotInitializedException(const char * const & filename);
    explicit FileNotInitializedException(const std::string& filename);
    virtual Type code() const noexcept { return Type::FileNotInitialized; }
};

class ObjectMissingException : public ExceptionBase {
public:
    explicit ObjectMissingException(types::big_uint objNumber);
    ObjectMissingException(types::big_uint objNumber, types::ushort genNumber);
    virtual Type code() const noexcept { return Type::ObjectMissing; }
};

class DuplicateKeyException : public ExceptionBase {
public:
    explicit DuplicateKeyException(const std::string& key);
    virtual Type code() const noexcept { return Type::DuplicateKey; }
};

} // syntax
} // vanillapdf

#endif /* _SYNTAX_EXCEPTIONS_H */
