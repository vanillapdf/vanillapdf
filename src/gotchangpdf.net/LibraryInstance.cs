using System;
using System.Collections;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;

namespace gotchangpdf.net
{
    internal static class LibraryInstance
    {
        private const string PATH_ENVIRONMENT_VARIABLE = "Path";
        private const string X86_LIBRARY_PATH = "x86\\gotchangpdf.dll";
        private const string X64_LIBRARY_PATH = "x64\\gotchangpdf.dll";

        private static IntPtr m_handle;
        public static IntPtr Handle
        {
            get
            {
                if (m_handle == IntPtr.Zero) {
                    Intialize();
                }

                return m_handle;
            }

            private set
            {
                m_handle = value;
            }
        }

        public static bool IsInitialized()
        {
            return (m_handle != IntPtr.Zero);
        }

        public static void Intialize()
        {
            // Already initialize
            if (m_handle != IntPtr.Zero) {
                return;
            }

            // Find the correct library path depending on the process
            string libraryPath = Path.GetFullPath(X86_LIBRARY_PATH);
            if (Environment.Is64BitProcess) {
                libraryPath = Path.GetFullPath(X64_LIBRARY_PATH);
            }

            // Determine if the libary path is included in environment PATH variable
            IDictionary environmentVars = Environment.GetEnvironmentVariables(EnvironmentVariableTarget.Process);
            string pathVariable = environmentVars[PATH_ENVIRONMENT_VARIABLE] as string;
            string[] pathVariables = pathVariable.Split(';');

            // Append path, if not included
            string libraryDirectoryName = Path.GetDirectoryName(libraryPath);
            if (!pathVariables.Contains(libraryDirectoryName)) {
                string newPathVariable = $"{pathVariable};{libraryDirectoryName}";
                Environment.SetEnvironmentVariable(PATH_ENVIRONMENT_VARIABLE, newPathVariable);
            }

            // Call the load library native function
            m_handle = LoadLibrary(libraryPath);

            // Could not library or it's dependencies
            if (m_handle == IntPtr.Zero) {
                int error = Marshal.GetLastWin32Error();
                throw new Win32Exception(error);
            }
        }

        public static void Release()
        {
            // Not yer initialized
            if (m_handle == IntPtr.Zero) {
                return;
            }

            bool freed = FreeLibrary(m_handle);
            if (!freed) {
                int error = Marshal.GetLastWin32Error();
                throw new Win32Exception(error);
            }

            m_handle = IntPtr.Zero;
        }

        public static T GetFunction<T>(string procName)
        {
            IntPtr procAddress = GetProcAddress(Handle, procName);
            if (procAddress == IntPtr.Zero) {
                throw new Exception($"Could not find procedure {procName}");
            }

            return Marshal.GetDelegateForFunctionPointer<T>(procAddress);
        }

        public static int GetConstant(string constantName)
        {
            IntPtr constantAddress = GetProcAddress(Handle, constantName);
            if (constantAddress == IntPtr.Zero) {
                throw new Exception($"Could not find procedure {constantName}");
            }

            return Marshal.ReadInt32(constantAddress);
        }

        [DllImport("kernel32", SetLastError = true, CharSet = CharSet.Ansi)]
        static extern IntPtr LoadLibrary([MarshalAs(UnmanagedType.LPStr)]string lpFileName);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool FreeLibrary(IntPtr hModule);

        [DllImport("kernel32", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        static extern IntPtr GetProcAddress(IntPtr hModule, string procName);
    }
}
