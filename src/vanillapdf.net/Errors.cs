using System;
using System.Runtime.InteropServices;
using System.Text;

namespace vanillapdf.net
{
    public static class Errors
    {
        public static UInt32 GetLastError()
        {
            UInt32 result = NativeMethods.Errors_GetLastError(out UInt32 code);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw new Exception("Could not get last error");
            }

            return code;
        }

        public static string GetPrintableErrorText(UInt32 error)
        {
            UInt32 lengthResult = NativeMethods.Errors_GetPrintableErrorTextLength(error, out UInt32 length);
            if (lengthResult != ReturnValues.ERROR_SUCCESS) {
                throw new Exception("Could not get last error message length");
            }

            int convertedLength = Convert.ToInt32(length);
            StringBuilder sb = new StringBuilder(convertedLength);

            UInt32 messageResult = NativeMethods.Errors_GetPrintableErrorText(error, sb, length);
            if (messageResult != ReturnValues.ERROR_SUCCESS) {
                throw new Exception("Could not get last error message");
            }

            return sb.ToString();
        }

        public static string GetLastErrorMessage()
        {
            UInt32 lengthResult = NativeMethods.Errors_GetLastErrorMessageLength(out UInt32 length);
            if (lengthResult != ReturnValues.ERROR_SUCCESS) {
                throw new Exception("Could not get last error message length");
            }

            int convertedLength = Convert.ToInt32(length);
            StringBuilder sb = new StringBuilder(convertedLength);

            UInt32 messageResult = NativeMethods.Errors_GetLastErrorMessage(sb, length);
            if (messageResult != ReturnValues.ERROR_SUCCESS) {
                throw new Exception("Could not get last error message");
            }

            return sb.ToString();
        }

        public static BaseException GetLastErrorException()
        {
            uint value = GetLastError();
            string message = GetLastErrorMessage();

            return BaseException.GetException(value, message);
        }

        private static class NativeMethods
        {
            public static ErrorsGetLastErrorDelgate Errors_GetLastError = LibraryInstance.GetFunction<ErrorsGetLastErrorDelgate>("Errors_GetLastError");
            public static ErrorsGetPrintableErrorTextDelegate Errors_GetPrintableErrorText = LibraryInstance.GetFunction<ErrorsGetPrintableErrorTextDelegate>("Errors_GetPrintableErrorText");
            public static ErrorsGetPrintableErrorTextLengthDelegate Errors_GetPrintableErrorTextLength = LibraryInstance.GetFunction<ErrorsGetPrintableErrorTextLengthDelegate>("Errors_GetPrintableErrorTextLength");
            public static ErrorsGetLastErrorMessageDelgate Errors_GetLastErrorMessage = LibraryInstance.GetFunction<ErrorsGetLastErrorMessageDelgate>("Errors_GetLastErrorMessage");
            public static ErrorsGetLastErrorMessageLengthDelgate Errors_GetLastErrorMessageLength = LibraryInstance.GetFunction<ErrorsGetLastErrorMessageLengthDelgate>("Errors_GetLastErrorMessageLength");

            [UnmanagedFunctionPointer(Utils.LibraryCallingConvention)]
            public delegate UInt32 ErrorsGetLastErrorDelgate(out UInt32 result);

            [UnmanagedFunctionPointer(Utils.LibraryCallingConvention)]
            public delegate UInt32 ErrorsGetPrintableErrorTextDelegate(UInt32 error, StringBuilder sb, UInt32 size);

            [UnmanagedFunctionPointer(Utils.LibraryCallingConvention)]
            public delegate UInt32 ErrorsGetPrintableErrorTextLengthDelegate(UInt32 error, out UInt32 result);

            [UnmanagedFunctionPointer(Utils.LibraryCallingConvention)]
            public delegate UInt32 ErrorsGetLastErrorMessageDelgate(StringBuilder sb, UInt32 size);

            [UnmanagedFunctionPointer(Utils.LibraryCallingConvention)]
            public delegate UInt32 ErrorsGetLastErrorMessageLengthDelgate(out UInt32 result);
        }
    }
}
