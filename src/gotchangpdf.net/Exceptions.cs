using System;

namespace gotchangpdf.net
{
    public abstract class BaseException : Exception
    {
        public UInt32 ErrorCode { get; protected set; }

        public BaseException(UInt32 errorCode)
        {
            ErrorCode = errorCode;
        }
    }

    public class ParameterValueException : BaseException
    {
        public static ParameterValueException Create()
        {
            UInt32 code = ReturnValues.ERROR_PARAMETER_VALUE;
            return new ParameterValueException(code);
        }

        private ParameterValueException(UInt32 errorCode) : base(errorCode)
        {
        }
    }

    public class NotSupportedException : BaseException
    {
        public static NotSupportedException Create()
        {
            UInt32 code = ReturnValues.ERROR_NOT_SUPPORTED;
            return new NotSupportedException(code);
        }

        private NotSupportedException(UInt32 errorCode) : base(errorCode)
        {
        }
    }

    public class GeneralException : BaseException
    {
        public static GeneralException Create()
        {
            UInt32 code = ReturnValues.ERROR_GENERAL;
            return new GeneralException(code);
        }

        private GeneralException(UInt32 errorCode) : base(errorCode)
        {
        }
    }

    public class ConversionException : BaseException
    {
        public static ConversionException Create()
        {
            UInt32 code = ReturnValues.ERROR_CONVERSION;
            return new ConversionException(code);
        }

        private ConversionException(UInt32 errorCode) : base(errorCode)
        {
        }
    }

    public class FileDisposedException : BaseException
    {
        public static FileDisposedException Create()
        {
            UInt32 code = ReturnValues.ERROR_FILE_DISPOSED;
            return new FileDisposedException(code);
        }

        private FileDisposedException(UInt32 errorCode) : base(errorCode)
        {
        }
    }

    public class FileNotInitializedException : BaseException
    {
        public static FileNotInitializedException Create()
        {
            UInt32 code = ReturnValues.ERROR_FILE_NOT_INITIALIZED;
            return new FileNotInitializedException(code);
        }

        private FileNotInitializedException(UInt32 errorCode) : base(errorCode)
        {
        }
    }

    public class ObjectMissingException : BaseException
    {
        public static ObjectMissingException Create()
        {
            UInt32 code = ReturnValues.ERROR_OBJECT_MISSING;
            return new ObjectMissingException(code);
        }

        private ObjectMissingException(UInt32 errorCode) : base(errorCode)
        {
        }
    }

    public class InvalidPasswordException : BaseException
    {
        public static InvalidPasswordException Create()
        {
            UInt32 code = ReturnValues.ERROR_INVALID_PASSWORD;
            return new InvalidPasswordException(code);
        }

        private InvalidPasswordException(UInt32 errorCode) : base(errorCode)
        {
        }
    }
}
