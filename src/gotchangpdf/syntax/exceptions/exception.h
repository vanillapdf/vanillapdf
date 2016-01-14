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
		enum class Type : unsigned int
		{
			// global
			Success = 0,
			InvalidParameter,
			NotSupported,

			// syntax
			Conversion = 0x00010000,
			FileDisposed,
			FileNotInitialized,
			ObjectMissing,
			ParseException,

			// semantic
			OptionalEntryMissing = 0x10000000,
			SemanticContext,

			// global
			General = 0xFFFFFFFF
		};

	public:
		ExceptionBase(const std::string& msg, Type type);
		ExceptionBase(const char * const & msg, Type type);

		virtual inline Type code() const { return _type; }
		virtual ~ExceptionBase() = 0;

	protected:
		Type _type = Type::General;
	};

	class GeneralException : public ExceptionBase
	{
	public:
		explicit GeneralException(const char * const & msg);
		explicit GeneralException(const std::string& msg);
	};

	class NotSupportedException : public ExceptionBase
	{
	public:
		explicit NotSupportedException(const char * const & msg);
		explicit NotSupportedException(const std::string& msg);
	};

	namespace syntax
	{
		class ParseException : public ExceptionBase
		{
		public:
			explicit ParseException(types::stream_offset offset);
		};

		class FileDisposedException : public ExceptionBase
		{
		public:
			FileDisposedException();
		};

		class FileNotInitializedException : public ExceptionBase
		{
		public:
			explicit FileNotInitializedException(const char * const & filename);
			explicit FileNotInitializedException(const std::string& filename);
		};

		class ObjectMissingException : public ExceptionBase
		{
		public:
			ObjectMissingException(types::uinteger objNumber, types::ushort genNumber);
		};

		class ConversionException : public ExceptionBase
		{
		public:
			explicit ConversionException(const char * const & msg);
			explicit ConversionException(const std::string& msg);
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
				static ConversionException Construct(const SourceT&)
				{
					std::string source_name(typeid(SourceT).name());
					std::string dest_name(typeid(DestT).name());
					std::string msg("Could not convert object of type " + source_name + " to type " + dest_name);
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
