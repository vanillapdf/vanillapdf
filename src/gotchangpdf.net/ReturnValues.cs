using System;

namespace gotchangpdf.net
{
    public static class ReturnValues
    {
        public static string GetValueName(int value)
        {
            if (value == ERROR_SUCCES) {
                return GOTCHANG_PDF_ERROR_SUCCES;
            }

            if (value == ERROR_PARAMETER_VALUE) {
                return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;
            }

            if (value == ERROR_NOT_SUPPORTED) {
                return GOTCHANG_PDF_ERROR_NOT_SUPPORTED;
            }

            if (value == ERROR_GENERAL) {
                return GOTCHANG_PDF_ERROR_GENERAL;
            }

            if (value == ERROR_CONVERSION) {
                return GOTCHANG_PDF_ERROR_CONVERSION;
            }

            if (value == ERROR_FILE_DISPOSED) {
                return GOTCHANG_PDF_ERROR_FILE_DISPOSED;
            }

            if (value == ERROR_FILE_NOT_INITIALIZED) {
                return GOTCHANG_PDF_ERROR_FILE_NOT_INITIALIZED;
            }

            if (value == ERROR_OBJECT_MISSING) {
                return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
            }

            if (value == ERROR_INVALID_PASSWORD) {
                return GOTCHANG_PDF_ERROR_INVALID_PASSWORD;
            }

            throw new Exception("Unknown return value");
        }

        public static BaseException GetException(int value)
        {
            if (value == ERROR_PARAMETER_VALUE) {
                return ParameterValueException.Create();
            }

            if (value == ERROR_NOT_SUPPORTED) {
                return NotSupportedException.Create();
            }

            if (value == ERROR_GENERAL) {
                return GeneralException.Create();
            }

            if (value == ERROR_CONVERSION) {
                return ConversionException.Create();
            }

            if (value == ERROR_FILE_DISPOSED) {
                return FileDisposedException.Create();
            }

            if (value == ERROR_FILE_NOT_INITIALIZED) {
                return FileNotInitializedException.Create();
            }

            if (value == ERROR_OBJECT_MISSING) {
                return ObjectMissingException.Create();
            }

            if (value == ERROR_INVALID_PASSWORD) {
                return InvalidPasswordException.Create();
            }

            throw new Exception("Unknown return value");
        }

        public static UInt32 ERROR_SUCCES = LibraryInstance.GetConstant(GOTCHANG_PDF_ERROR_SUCCES);
        public static UInt32 ERROR_PARAMETER_VALUE = LibraryInstance.GetConstant(GOTCHANG_PDF_ERROR_PARAMETER_VALUE);
        public static UInt32 ERROR_NOT_SUPPORTED = LibraryInstance.GetConstant(GOTCHANG_PDF_ERROR_NOT_SUPPORTED);
        public static UInt32 ERROR_GENERAL = LibraryInstance.GetConstant(GOTCHANG_PDF_ERROR_GENERAL);
        public static UInt32 ERROR_CONVERSION = LibraryInstance.GetConstant(GOTCHANG_PDF_ERROR_CONVERSION);
        public static UInt32 ERROR_FILE_DISPOSED = LibraryInstance.GetConstant(GOTCHANG_PDF_ERROR_FILE_DISPOSED);
        public static UInt32 ERROR_FILE_NOT_INITIALIZED = LibraryInstance.GetConstant(GOTCHANG_PDF_ERROR_FILE_NOT_INITIALIZED);
        public static UInt32 ERROR_OBJECT_MISSING = LibraryInstance.GetConstant(GOTCHANG_PDF_ERROR_OBJECT_MISSING);
        public static UInt32 ERROR_INVALID_PASSWORD = LibraryInstance.GetConstant(GOTCHANG_PDF_ERROR_INVALID_PASSWORD);

        private const string GOTCHANG_PDF_ERROR_SUCCES = "GOTCHANG_PDF_ERROR_SUCCES";
        private const string GOTCHANG_PDF_ERROR_PARAMETER_VALUE = "GOTCHANG_PDF_ERROR_PARAMETER_VALUE";
        private const string GOTCHANG_PDF_ERROR_NOT_SUPPORTED = "GOTCHANG_PDF_ERROR_NOT_SUPPORTED";
        private const string GOTCHANG_PDF_ERROR_GENERAL = "GOTCHANG_PDF_ERROR_GENERAL";
        private const string GOTCHANG_PDF_ERROR_CONVERSION = "GOTCHANG_PDF_ERROR_CONVERSION";
        private const string GOTCHANG_PDF_ERROR_FILE_DISPOSED = "GOTCHANG_PDF_ERROR_FILE_DISPOSED";
        private const string GOTCHANG_PDF_ERROR_FILE_NOT_INITIALIZED = "GOTCHANG_PDF_ERROR_FILE_NOT_INITIALIZED";
        private const string GOTCHANG_PDF_ERROR_OBJECT_MISSING = "GOTCHANG_PDF_ERROR_OBJECT_MISSING";
        private const string GOTCHANG_PDF_ERROR_INVALID_PASSWORD = "GOTCHANG_PDF_ERROR_INVALID_PASSWORD";
    }
}
