using System;
using System.Runtime.InteropServices;

namespace gotchangpdf.net
{
    public static class Logging
    {
        public enum Severity
        {
            Debug = 0,
            Info,
            Warning,
            Error,
            Fatal
        }

        public static bool IsEnabled()
        {
            int result = NativeMethods.Logging_IsEnabled(out bool enabled);
            if (result != ReturnValues.ERROR_SUCCES) {
                throw new Exception("Could not initialize file");
            }

            return enabled;
        }

        public static void Enable()
        {
            int result = NativeMethods.Logging_Enable();
            if (result != ReturnValues.ERROR_SUCCES) {
                throw new Exception("Could not enable logging");
            }
        }

        public static void Disable()
        {
            int result = NativeMethods.Logging_Disable();
            if (result != ReturnValues.ERROR_SUCCES) {
                throw new Exception("Could not disable logging");
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
            public delegate int LoggingIsEnabledDelgate(out bool result);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int LoggingEnableDelgate();

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int LoggingDisableDelgate();

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int LoggingGetSeverityDelgate(out Severity severity);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int LoggingSetSeverityDelgate(Severity severity);
        }
    }
}
