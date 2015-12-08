#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include "deferred.h"
#include "constants.h"

#include <exception>
#include <string>

namespace gotchangpdf
{
	class ExceptionBase : public std::exception
	{
	public:
		enum class Type
		{
			General = 0,
			Conversion,
			NotSupported,
			FileDisposed,
			FileNotInitialized,
			ObjectMissing,
			ParseException,
			SemanticContext
		};

	public:
		explicit ExceptionBase(const std::string& msg, Type type) : exception(msg.c_str()), _type(type) {}
		ExceptionBase(const char * const & msg, Type type) : exception(msg), _type(type) {}

		virtual inline Type code() const { return _type; }
		virtual ~ExceptionBase() = 0;

	protected:
		Type _type = Type::General;
	};

	inline ExceptionBase::~ExceptionBase() {}

	class GeneralException : public ExceptionBase
	{
	public:
		explicit GeneralException(const char * const & msg) : ExceptionBase(msg, Type::General) {}
		explicit GeneralException(const std::string& msg) : ExceptionBase(msg, Type::General) {}
	};

	class NotSupportedException : public ExceptionBase
	{
	public:
		explicit NotSupportedException(const char * const & msg) : ExceptionBase(msg, Type::NotSupported) {}
		explicit NotSupportedException(const std::string& msg) : ExceptionBase(msg, Type::NotSupported) {}
	};

	namespace syntax
	{
		class ParseException : public ExceptionBase
		{
		public:
			explicit ParseException(types::stream_offset offset)
				: ExceptionBase("Could not parse object at offset " + std::to_string(offset), Type::ParseException) {}
		};

		class FileDisposedException : public ExceptionBase
		{
		public:
			explicit FileDisposedException() : ExceptionBase("Current file has been already disposed", Type::FileDisposed) {}
		};

		class FileNotInitializedException : public ExceptionBase
		{
		public:
			explicit FileNotInitializedException(const char * const & filename)
				: ExceptionBase("File " + std::string(filename) + "has not been initialized", Type::FileNotInitialized) {}
			explicit FileNotInitializedException(const std::string& filename)
				: FileNotInitializedException(filename.c_str()) {}
		};

		class ObjectMissingException : public ExceptionBase
		{
		public:
			ObjectMissingException(types::integer objNumber, types::ushort genNumber)
				: ExceptionBase("Object " + std::to_string(objNumber) + " " + std::to_string(genNumber) + "is missing", Type::ObjectMissing) {}
		};

		class ConversionException : public ExceptionBase
		{
		public:
			explicit ConversionException(const char * const & msg) : ExceptionBase(msg, Type::Conversion) {}
			explicit ConversionException(const std::string& msg) : ExceptionBase(msg, Type::Conversion) {}
		};

		template <typename DestT>
		class ConversionExceptionFactory
		{
		public:
			template <typename SourceT>
			static ConversionException Construct(const SourceT& obj) { return Constructor<SourceT>::Construct(obj); }

		private:
			template <typename SourceT, bool = std::is_integral<SourceT>::value && std::is_integral<DestT>::value>
			class Constructor
			{
			};

			template <typename SourceT>
			class Constructor<SourceT, false>
			{
			public:
				static ConversionException Construct(const SourceT& obj)
				{
					std::string msg("Could not convert object of type " + typeid(SourceT).name() + " to type " + typeid(DestT).name());
					return ConversionException(msg);
				}
			};

			template <typename SourceT>
			class Constructor<Deferred<SourceT>, false>
			{
			public:
				static ConversionException Construct(const Deferred<SourceT>& obj)
				{
					auto ptr = obj.Content.get();
					std::string source_name = (nullptr == ptr ? "nullptr" : typeid(*ptr).name());
					std::string msg("Could not convert object of type " + source_name + " to type " + typeid(DestT).name());
					return ConversionException(msg);
				}
			};

			template <typename SourceT>
			class Constructor<SourceT, true>
			{
			public:
				template <typename = std::enable_if_t<std::is_integral<SourceT>::value>,
					typename = std::enable_if_t<std::is_integral<DestT>::value >>
					static ConversionException Construct(const SourceT& value)
				{
					std::string msg("Could not convert value " + std::to_string(value) + " of type " + typeid(SourceT).name() + " to type " + typeid(DestT).name());
					return ConversionException(msg);
				}
			};
		};
	}
}

#endif /* _EXCEPTION_H */
