using System;
using System.Runtime.InteropServices;

namespace vanillapdf.net
{
    public class File : IUnknown
    {
        internal IntPtr Handle { get; private set; }

        internal File(IntPtr handle)
        {
            Handle = handle;
        }

        public static File Open(string filename)
        {
            UInt32 result = NativeMethods.File_Open(filename, out IntPtr handle);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return new File(handle);
        }

        public static File Create(string filename)
        {
            UInt32 result = NativeMethods.File_Open(filename, out IntPtr handle);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return new File(handle);
        }

        public void Initialize()
        {
            UInt32 result = NativeMethods.File_Initialize(Handle);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }
        }

        protected override void Dispose(bool disposing)
        {
            if (Handle != IntPtr.Zero) {
                UInt32 result = NativeMethods.File_Release(Handle);
                if (result != ReturnValues.ERROR_SUCCESS) {
                    throw Errors.GetLastErrorException();
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
            public delegate UInt32 FileOpenDelgate(string filename, out IntPtr handle);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate UInt32 FileCreateDelgate(string filename, out IntPtr handle);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate UInt32 FileInitializeDelgate(IntPtr handle);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate UInt32 FileReleaseDelgate(IntPtr handle);
        }
    }
}
