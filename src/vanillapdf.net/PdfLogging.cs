using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public enum LoggingSeverity
    {
        Debug = 0,
        Info,
        Warning,
        Error,
        Fatal
    }

    public static class PdfLogging
    {
        static PdfLogging()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public static LoggingSeverity Severity
        {
            get { return GetSeverity(); }
            set { SetSeverity(value); }
        }

        public static bool IsEnabled()
        {
            UInt32 result = NativeMethods.Logging_IsEnabled(out bool enabled);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return enabled;
        }

        public static void Enable()
        {
            UInt32 result = NativeMethods.Logging_Enable();
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }
        }

        public static void Disable()
        {
            UInt32 result = NativeMethods.Logging_Disable();
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }
        }

        public static LoggingSeverity GetSeverity()
        {
            UInt32 result = NativeMethods.Logging_GetSeverity(out int severity);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return EnumUtil<LoggingSeverity>.CheckedCast(severity);
        }

        public static void SetSeverity(LoggingSeverity severity)
        {
            UInt32 result = NativeMethods.Logging_SetSeverity(severity.GetIntegralValue());
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }
        }

        private static class NativeMethods
        {
            public static LoggingIsEnabledDelgate Logging_IsEnabled = LibraryInstance.GetFunction<LoggingIsEnabledDelgate>("Logging_IsEnabled");
            public static LoggingEnableDelgate Logging_Enable = LibraryInstance.GetFunction<LoggingEnableDelgate>("Logging_Enable");
            public static LoggingDisableDelgate Logging_Disable = LibraryInstance.GetFunction<LoggingDisableDelgate>("Logging_Disable");
            public static LoggingGetSeverityDelgate Logging_GetSeverity = LibraryInstance.GetFunction<LoggingGetSeverityDelgate>("Logging_GetSeverity");
            public static LoggingSetSeverityDelgate Logging_SetSeverity = LibraryInstance.GetFunction<LoggingSetSeverityDelgate>("Logging_SetSeverity");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 LoggingIsEnabledDelgate(out bool result);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 LoggingEnableDelgate();

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 LoggingDisableDelgate();

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 LoggingGetSeverityDelgate(out int severity);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 LoggingSetSeverityDelgate(int severity);
        }
    }
}
