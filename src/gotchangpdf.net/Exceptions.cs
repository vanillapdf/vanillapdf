using System;

namespace gotchangpdf.net
{
    public abstract class BaseException : Exception
    {
        public UInt32 ErrorCode { get; protected set; }

        public BaseException(string message, UInt32 errorCode)
            : base(message)
        {
            ErrorCode = errorCode;
        }

        public static BaseException GetException(uint value, string errorMessage)
        {
            if (value == ReturnValues.ERROR_PARAMETER_VALUE) {
                return ParameterValueException.Create(errorMessage);
            }

            if (value == ReturnValues.ERROR_NOT_SUPPORTED) {
                return NotSupportedException.Create(errorMessage);
            }

            if (value == ReturnValues.ERROR_GENERAL) {
                return GeneralException.Create(errorMessage);
            }

            if (value == ReturnValues.ERROR_CONVERSION) {
                return ConversionException.Create(errorMessage);
            }

            if (value == ReturnValues.ERROR_FILE_DISPOSED) {
                return FileDisposedException.Create(errorMessage);
            }

            if (value == ReturnValues.ERROR_FILE_NOT_INITIALIZED) {
                return FileNotInitializedException.Create(errorMessage);
            }

            if (value == ReturnValues.ERROR_OBJECT_MISSING) {
                return ObjectMissingException.Create(errorMessage);
            }

            if (value == ReturnValues.ERROR_INVALID_PASSWORD) {
                return InvalidPasswordException.Create(errorMessage);
            }

            throw new Exception("Unknown return value");
        }
    }

    public class ParameterValueException : BaseException
    {
        public static ParameterValueException Create(string message)
        {
            UInt32 code = ReturnValues.ERROR_PARAMETER_VALUE;
            return new ParameterValueException(message, code);
        }

        private ParameterValueException(string message, UInt32 errorCode)
            : base(message, errorCode)
        {
        }
    }

    public class NotSupportedException : BaseException
    {
        public static NotSupportedException Create(string message)
        {
            UInt32 code = ReturnValues.ERROR_NOT_SUPPORTED;
            return new NotSupportedException(message, code);
        }

        private NotSupportedException(string message, UInt32 errorCode)
            : base(message, errorCode)
        {
        }
    }

    public class GeneralException : BaseException
    {
        public static GeneralException Create(string message)
        {
            UInt32 code = ReturnValues.ERROR_GENERAL;
            return new GeneralException(message, code);
        }

        private GeneralException(string message, UInt32 errorCode)
            : base(message, errorCode)
        {
        }
    }

    public class ConversionException : BaseException
    {
        public static ConversionException Create(string message)
        {
            UInt32 code = ReturnValues.ERROR_CONVERSION;
            return new ConversionException(message, code);
        }

        private ConversionException(string message, UInt32 errorCode)
            : base(message, errorCode)
        {
        }
    }

    public class FileDisposedException : BaseException
    {
        public static FileDisposedException Create(string message)
        {
            UInt32 code = ReturnValues.ERROR_FILE_DISPOSED;
            return new FileDisposedException(message, code);
        }

        private FileDisposedException(string message, UInt32 errorCode)
            : base(message, errorCode)
        {
        }
    }

    public class FileNotInitializedException : BaseException
    {
        public static FileNotInitializedException Create(string message)
        {
            UInt32 code = ReturnValues.ERROR_FILE_NOT_INITIALIZED;
            return new FileNotInitializedException(message, code);
        }

        private FileNotInitializedException(string message, UInt32 errorCode)
            : base(message, errorCode)
        {
        }
    }

    public class ObjectMissingException : BaseException
    {
        public static ObjectMissingException Create(string message)
        {
            UInt32 code = ReturnValues.ERROR_OBJECT_MISSING;
            return new ObjectMissingException(message, code);
        }

        private ObjectMissingException(string message, UInt32 errorCode)
            : base(message, errorCode)
        {
        }
    }

    public class InvalidPasswordException : BaseException
    {
        public static InvalidPasswordException Create(string message)
        {
            UInt32 code = ReturnValues.ERROR_INVALID_PASSWORD;
            return new InvalidPasswordException(message, code);
        }

        private InvalidPasswordException(string message, UInt32 errorCode)
            : base(message, errorCode)
        {
        }
    }
}
