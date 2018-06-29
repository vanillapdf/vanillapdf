using System;
using System.Runtime.InteropServices;

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

    public static class Logging
    {
        public static LoggingSeverity Severity
        {
            get { return GetSeverity(); }
            set { SetSeverity(value); }
        }

        public static bool IsEnabled()
        {
            UInt32 result = NativeMethods.Logging_IsEnabled(out bool enabled);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return enabled;
        }

        public static void Enable()
        {
            UInt32 result = NativeMethods.Logging_Enable();
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }
        }

        public static void Disable()
        {
            UInt32 result = NativeMethods.Logging_Disable();
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }
        }

        public static LoggingSeverity GetSeverity()
        {
            UInt32 result = NativeMethods.Logging_GetSeverity(out LoggingSeverity severity);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return severity;
        }

        public static void SetSeverity(LoggingSeverity severity)
        {
            UInt32 result = NativeMethods.Logging_SetSeverity(severity);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }
        }

        private static class NativeMethods
        {
            public static LoggingIsEnabledDelgate Logging_IsEnabled = LibraryInstance.GetFunction<LoggingIsEnabledDelgate>("Logging_IsEnabled");
            public static LoggingEnableDelgate Logging_Enable = LibraryInstance.GetFunction<LoggingEnableDelgate>("Logging_Enable");
            public static LoggingDisableDelgate Logging_Disable = LibraryInstance.GetFunction<LoggingDisableDelgate>("Logging_Disable");
            public static LoggingGetSeverityDelgate Logging_GetSeverity = LibraryInstance.GetFunction<LoggingGetSeverityDelgate>("Logging_GetSeverity");
            public static LoggingSetSeverityDelgate Logging_SetSeverity = LibraryInstance.GetFunction<LoggingSetSeverityDelgate>("Logging_SetSeverity");

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate UInt32 LoggingIsEnabledDelgate(out bool result);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate UInt32 LoggingEnableDelgate();

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate UInt32 LoggingDisableDelgate();

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate UInt32 LoggingGetSeverityDelgate(out LoggingSeverity severity);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate UInt32 LoggingSetSeverityDelgate(LoggingSeverity severity);
        }
    }
}
