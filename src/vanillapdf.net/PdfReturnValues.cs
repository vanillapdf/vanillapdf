using System;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public static class PdfReturnValues
    {
        public static string GetValueName(uint value)
        {
            if (value == ERROR_SUCCESS) {
                return VANILLAPDF_ERROR_SUCCESS;
            }

            if (value == ERROR_PARAMETER_VALUE) {
                return VANILLAPDF_ERROR_PARAMETER_VALUE;
            }

            if (value == ERROR_NOT_SUPPORTED) {
                return VANILLAPDF_ERROR_NOT_SUPPORTED;
            }

            if (value == ERROR_GENERAL) {
                return VANILLAPDF_ERROR_GENERAL;
            }

            if (value == ERROR_CONVERSION) {
                return VANILLAPDF_ERROR_CONVERSION;
            }

            if (value == ERROR_FILE_DISPOSED) {
                return VANILLAPDF_ERROR_FILE_DISPOSED;
            }

            if (value == ERROR_FILE_NOT_INITIALIZED) {
                return VANILLAPDF_ERROR_FILE_NOT_INITIALIZED;
            }

            if (value == ERROR_OBJECT_MISSING) {
                return VANILLAPDF_ERROR_OBJECT_MISSING;
            }

            if (value == ERROR_INVALID_PASSWORD) {
                return VANILLAPDF_ERROR_INVALID_PASSWORD;
            }

            throw new Exception("Unknown return value");
        }

        public static UInt32 ERROR_SUCCESS = LibraryInstance.GetConstant(VANILLAPDF_ERROR_SUCCESS);
        public static UInt32 ERROR_PARAMETER_VALUE = LibraryInstance.GetConstant(VANILLAPDF_ERROR_PARAMETER_VALUE);
        public static UInt32 ERROR_NOT_SUPPORTED = LibraryInstance.GetConstant(VANILLAPDF_ERROR_NOT_SUPPORTED);
        public static UInt32 ERROR_GENERAL = LibraryInstance.GetConstant(VANILLAPDF_ERROR_GENERAL);
        public static UInt32 ERROR_CONVERSION = LibraryInstance.GetConstant(VANILLAPDF_ERROR_CONVERSION);
        public static UInt32 ERROR_FILE_DISPOSED = LibraryInstance.GetConstant(VANILLAPDF_ERROR_FILE_DISPOSED);
        public static UInt32 ERROR_FILE_NOT_INITIALIZED = LibraryInstance.GetConstant(VANILLAPDF_ERROR_FILE_NOT_INITIALIZED);
        public static UInt32 ERROR_OBJECT_MISSING = LibraryInstance.GetConstant(VANILLAPDF_ERROR_OBJECT_MISSING);
        public static UInt32 ERROR_INVALID_PASSWORD = LibraryInstance.GetConstant(VANILLAPDF_ERROR_INVALID_PASSWORD);

        private const string VANILLAPDF_ERROR_SUCCESS = "VANILLAPDF_ERROR_SUCCESS";
        private const string VANILLAPDF_ERROR_PARAMETER_VALUE = "VANILLAPDF_ERROR_PARAMETER_VALUE";
        private const string VANILLAPDF_ERROR_NOT_SUPPORTED = "VANILLAPDF_ERROR_NOT_SUPPORTED";
        private const string VANILLAPDF_ERROR_GENERAL = "VANILLAPDF_ERROR_GENERAL";
        private const string VANILLAPDF_ERROR_CONVERSION = "VANILLAPDF_ERROR_CONVERSION";
        private const string VANILLAPDF_ERROR_FILE_DISPOSED = "VANILLAPDF_ERROR_FILE_DISPOSED";
        private const string VANILLAPDF_ERROR_FILE_NOT_INITIALIZED = "VANILLAPDF_ERROR_FILE_NOT_INITIALIZED";
        private const string VANILLAPDF_ERROR_OBJECT_MISSING = "VANILLAPDF_ERROR_OBJECT_MISSING";
        private const string VANILLAPDF_ERROR_INVALID_PASSWORD = "VANILLAPDF_ERROR_INVALID_PASSWORD";
    }
}
