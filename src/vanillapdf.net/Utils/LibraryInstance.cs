using System;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace vanillapdf.net.Utils
{
    internal static class LibraryInstance
    {
        private const string X86_LIBRARY_PATH = "x86\\libvanillapdf.dll";
        private const string X64_LIBRARY_PATH = "x64\\libvanillapdf.dll";

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
            // Already initialized
            if (m_handle != IntPtr.Zero) {
                return;
            }

            // Root path of the entry assembly
            string rootAssemblyPath = Assembly.GetEntryAssembly().Location;
            string rootPath = Path.GetDirectoryName(rootAssemblyPath);

            // Find the correct library path depending on the process
            string libraryPath = Path.Combine(rootPath, X86_LIBRARY_PATH);
            if (Environment.Is64BitProcess) {
                libraryPath = Path.Combine(rootPath, X64_LIBRARY_PATH);
            }

            // Call the load library native function
            m_handle = LoadLibraryEx(libraryPath, IntPtr.Zero, LoadLibraryFlags.LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LoadLibraryFlags.LOAD_LIBRARY_SEARCH_SYSTEM32);

            // Could not library or it's dependencies
            if (m_handle == IntPtr.Zero) {
                int error = Marshal.GetLastWin32Error();
                throw new Win32Exception(error);
            }
        }

        public static void Release()
        {
            // Not yet initialized
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

        public static UInt32 GetConstant(string constantName)
        {
            IntPtr constantAddress = GetProcAddress(Handle, constantName);
            if (constantAddress == IntPtr.Zero) {
                throw new Exception($"Could not find procedure {constantName}");
            }

            byte[] bytes = new byte[4];
            for (int i = 0; i < 4; ++i) {
                bytes[i] = Marshal.ReadByte(constantAddress, i);
            }

            //int value = Marshal.ReadInt32(constantAddress);
            //byte[] bytes = BitConverter.get
            return BitConverter.ToUInt32(bytes, 0);
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern IntPtr LoadLibraryEx(string lpFileName, IntPtr hReservedNull, LoadLibraryFlags dwFlags);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool FreeLibrary(IntPtr hModule);

        [DllImport("kernel32", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        static extern IntPtr GetProcAddress(IntPtr hModule, string procName);

        [Flags]
        enum LoadLibraryFlags : uint
        {
            DONT_RESOLVE_DLL_REFERENCES = 0x00000001,
            LOAD_IGNORE_CODE_AUTHZ_LEVEL = 0x00000010,
            LOAD_LIBRARY_AS_DATAFILE = 0x00000002,
            LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE = 0x00000040,
            LOAD_LIBRARY_AS_IMAGE_RESOURCE = 0x00000020,
            LOAD_LIBRARY_SEARCH_APPLICATION_DIR = 0x00000200,
            LOAD_LIBRARY_SEARCH_DEFAULT_DIRS = 0x00001000,
            LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR = 0x00000100,
            LOAD_LIBRARY_SEARCH_SYSTEM32 = 0x00000800,
            LOAD_LIBRARY_SEARCH_USER_DIRS = 0x00000400,
            LOAD_WITH_ALTERED_SEARCH_PATH = 0x00000008
        }
    }
}
