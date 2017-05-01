using System;
using System.Runtime.InteropServices;

namespace gotchangpdf.net
{
    public class File : IUnknown
    {
        internal IntPtr Handle { get; private set; }

        private File(IntPtr handle)
        {
            Handle = handle;
        }

        public static File Open(string filename)
        {
            int result = NativeMethods.File_Open(filename, out IntPtr handle);
            if (result != ReturnValues.ERROR_SUCCES) {
                throw new Exception("Could not open file");
            }

            return new File(handle);
        }

        public static File Create(string filename)
        {
            int result = NativeMethods.File_Open(filename, out IntPtr handle);
            if (result != ReturnValues.ERROR_SUCCES) {
                throw new Exception("Could not create file");
            }

            return new File(handle);
        }

        public void Initialize()
        {
            int result = NativeMethods.File_Initialize(Handle);
            if (result != ReturnValues.ERROR_SUCCES) {
                throw new Exception("Could not initialize file");
            }
        }

        protected override void Dispose(bool disposing)
        {
            if (Handle != IntPtr.Zero) {
                int result = NativeMethods.File_Release(Handle);
                if (result != ReturnValues.ERROR_SUCCES) {
                    throw new Exception("Could not release document");
                }

                Handle = IntPtr.Zero;
            }
        }

        private static class NativeMethods
        {
            public static FileOpenDelgate File_Open = LibraryInstance.GetFunction<FileOpenDelgate>("File_Open");
            public static FileCreateDelgate File_Create = LibraryInstance.GetFunction<FileCreateDelgate>("File_Create");
            public static FileInitializeDelgate File_Initialize = LibraryInstance.GetFunction<FileInitializeDelgate>("File_Initialize");
            public static FileReleaseDelgate File_Release = LibraryInstance.GetFunction<FileReleaseDelgate>("File_Release");

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int FileOpenDelgate(string filename, out IntPtr handle);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int FileCreateDelgate(string filename, out IntPtr handle);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int FileInitializeDelgate(IntPtr handle);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int FileReleaseDelgate(IntPtr handle);
        }
    }
}
