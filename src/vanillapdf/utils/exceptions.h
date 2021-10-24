#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include "utils/deferred.h"
#include "utils/constants.h"

#include <exception>
#include <string>
#include <typeinfo>

namespace vanillapdf {

class ExceptionBase : public std::exception {
public:
	enum class Type : uint32_t {
		// global
		Success = 0,
		InvalidParameter,
		NotSupported,
		UserCancelled,
		ZlibDataError,
		InvalidLicense,
		LicenseRequired,

		// syntax
		Conversion = 0x00010000,
		FileDisposed,
		FileNotInitialized,
		ObjectMissing,
		ParseException,
		InvalidPassword,

		// semantic
		OptionalEntryMissing = 0x10000000,
		SemanticContext,

		// global
		General = 0xFFFFFFFF
	};

public:
	ExceptionBase() = default;
	explicit ExceptionBase(const std::string& msg);
	explicit ExceptionBase(const char * const & msg);

	virtual const char * what() const noexcept override;
	virtual Type code() const noexcept = 0;
	virtual ~ExceptionBase() = 0;

protected:
	std::string m_msg;
};

class GeneralException : public ExceptionBase {
public:
	explicit GeneralException(const char * const & msg);
	explicit GeneralException(const std::string& msg);
	virtual Type code() const noexcept override { return Type::General; }
};

class NotSupportedException : public ExceptionBase {
public:
	explicit NotSupportedException(const char * const & msg);
	explicit NotSupportedException(const std::string& msg);
	virtual Type code() const noexcept override { return Type::NotSupported; }
};

class UserCancelledException : public ExceptionBase {
public:
	explicit UserCancelledException(const char * const & msg);
	explicit UserCancelledException(const std::string& msg);
	virtual Type code() const noexcept override { return Type::UserCancelled; }
};

class ConversionException : public ExceptionBase {
public:
	explicit ConversionException(const char * const & msg);
	explicit ConversionException(const std::string& msg);
	virtual Type code() const noexcept override { return Type::Conversion; }
};

class ZlibDataErrorException : public ExceptionBase {
public:
	explicit ZlibDataErrorException(types::stream_size size);
	explicit ZlibDataErrorException(types::stream_size size, const std::string& message);
	virtual Type code() const noexcept override { return Type::ZlibDataError; }

	types::stream_size Size(void) const { return m_size; }

private:
	types::stream_size m_size;
};

class InvalidLicenseException : public ExceptionBase {
public:
	explicit InvalidLicenseException(const char * const & msg);
	explicit InvalidLicenseException(const std::string& msg);
	virtual Type code() const noexcept override { return Type::InvalidLicense; }
};

class LicenseRequiredException : public ExceptionBase {
public:
	explicit LicenseRequiredException(const char * const & msg);
	explicit LicenseRequiredException(const std::string& msg);
	virtual Type code() const noexcept override { return Type::LicenseRequired; }
};

class InvalidPasswordException : public ExceptionBase {
public:
	explicit InvalidPasswordException(const char * const & msg);
	explicit InvalidPasswordException(const std::string& msg);
	virtual Type code() const noexcept override { return Type::InvalidPassword; }
};

template <typename DestT>
class ConversionExceptionFactory {
public:
	template <typename SourceT>
	static ConversionException Construct(const SourceT& obj) { return Constructor<SourceT>::Construct(obj); }

private:
	template <typename SourceT, bool = std::is_integral<SourceT>::value && std::is_integral<DestT>::value>
	class Constructor {
	};

	template <typename SourceT>
	class Constructor<SourceT, false> {
	public:
		static ConversionException Construct(const SourceT&) {
			std::string source_name(typeid(SourceT).name());
			std::string dest_name(typeid(DestT).name());
			std::string msg("Could not convert object of type " + source_name + " to type " + dest_name);
			return ConversionException(msg);
		}
	};

	template <typename SourceT>
	class Constructor<Deferred<SourceT>, false> {
	public:
		static ConversionException Construct(const Deferred<SourceT>& obj) {
			auto ptr = obj.get();
			std::string source_name = (nullptr == ptr ? "nullptr" : typeid(*ptr).name());
			std::string msg("Could not convert object of type " + source_name + " to type " + typeid(DestT).name());
			return ConversionException(msg);
		}
	};

	template <typename SourceT>
	class Constructor<SourceT, true> {
	public:
		template <
			typename = typename std::enable_if<std::is_integral<SourceT>::value>::type,
			typename = typename std::enable_if<std::is_integral<DestT>::value>::type
		>
			static ConversionException Construct(const SourceT& value) {
			std::string msg("Could not convert value " + std::to_string(value) + " of type " + typeid(SourceT).name() + " to type " + typeid(DestT).name());
			return ConversionException(msg);
		}
	};
};

} // vanillapdf

#endif /* _EXCEPTIONS_H */
