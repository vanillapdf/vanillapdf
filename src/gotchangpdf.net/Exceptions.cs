using System;

namespace gotchangpdf.net
{
    public abstract class BaseException : Exception
    {
        public int ErrorCode { get; protected set; }

        public BaseException(int errorCode)
        {
            ErrorCode = errorCode;
        }
    }

    public class ParameterValueException : BaseException
    {
        public static ParameterValueException Create()
        {
            int code = ReturnValues.ERROR_PARAMETER_VALUE;
            return new ParameterValueException(code);
        }

        private ParameterValueException(int errorCode) : base(errorCode)
        {
        }
    }

    public class NotSupportedException : BaseException
    {
        public static NotSupportedException Create()
        {
            int code = ReturnValues.ERROR_NOT_SUPPORTED;
            return new NotSupportedException(code);
        }

        private NotSupportedException(int errorCode) : base(errorCode)
        {
        }
    }

    public class GeneralException : BaseException
    {
        public static GeneralException Create()
        {
            int code = ReturnValues.ERROR_GENERAL;
            return new GeneralException(code);
        }

        private GeneralException(int errorCode) : base(errorCode)
        {
        }
    }

    public class ConversionException : BaseException
    {
        public static ConversionException Create()
        {
            int code = ReturnValues.ERROR_CONVERSION;
            return new ConversionException(code);
        }

        private ConversionException(int errorCode) : base(errorCode)
        {
        }
    }

    public class FileDisposedException : BaseException
    {
        public static FileDisposedException Create()
        {
            int code = ReturnValues.ERROR_FILE_DISPOSED;
            return new FileDisposedException(code);
        }

        private FileDisposedException(int errorCode) : base(errorCode)
        {
        }
    }

    public class FileNotInitializedException : BaseException
    {
        public static FileNotInitializedException Create()
        {
            int code = ReturnValues.ERROR_FILE_NOT_INITIALIZED;
            return new FileNotInitializedException(code);
        }

        private FileNotInitializedException(int errorCode) : base(errorCode)
        {
        }
    }

    public class ObjectMissingException : BaseException
    {
        public static ObjectMissingException Create()
        {
            int code = ReturnValues.ERROR_OBJECT_MISSING;
            return new ObjectMissingException(code);
        }

        private ObjectMissingException(int errorCode) : base(errorCode)
        {
        }
    }

    public class InvalidPasswordException : BaseException
    {
        public static InvalidPasswordException Create()
        {
            int code = ReturnValues.ERROR_INVALID_PASSWORD;
            return new InvalidPasswordException(code);
        }

        private InvalidPasswordException(int errorCode) : base(errorCode)
        {
        }
    }

    public class OptionalEntryMissingException : BaseException
    {
        public static OptionalEntryMissingException Create()
        {
            int code = ReturnValues.ERROR_OPTIONAL_ENTRY_MISSING;
            return new OptionalEntryMissingException(code);
        }

        private OptionalEntryMissingException(int errorCode) : base(errorCode)
        {
        }
    }
}
