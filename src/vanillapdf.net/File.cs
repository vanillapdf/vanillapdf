using System;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class File : IUnknown
    {
        internal File(IntPtr handle)
        {
            Handle = handle;
        }

        public static File Open(string filename)
        {
            UInt32 result = NativeMethods.File_Open(filename, out IntPtr data);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return new File(data);
        }

        public static File OpenStream(Inputstream stream, string filename)
        {
            UInt32 result = NativeMethods.File_OpenStream(stream.Handle, filename, out IntPtr data);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return new File(data);
        }

        public static File Create(string filename)
        {
            UInt32 result = NativeMethods.File_Create(filename, out IntPtr data);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return new File(data);
        }

        public static File CreateStream(Inputstream stream, string name)
        {
            UInt32 result = NativeMethods.File_CreateStream(stream.Handle, name, out IntPtr data);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return new File(data);
        }

        public void Initialize()
        {
            UInt32 result = NativeMethods.File_Initialize(Handle);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }
        }

        protected override UInt32 Release(IntPtr data)
        {
            return NativeMethods.File_Release(data);
        }

        private static class NativeMethods
        {
            public static FileOpenDelgate File_Open = LibraryInstance.GetFunction<FileOpenDelgate>("File_Open");
            public static FileOpenStreamDelgate File_OpenStream = LibraryInstance.GetFunction<FileOpenStreamDelgate>("File_OpenStream");
            public static FileCreateDelgate File_Create = LibraryInstance.GetFunction<FileCreateDelgate>("File_Create");
            public static FileCreateStreamDelgate File_CreateStream = LibraryInstance.GetFunction<FileCreateStreamDelgate>("File_CreateStream");
            public static FileInitializeDelgate File_Initialize = LibraryInstance.GetFunction<FileInitializeDelgate>("File_Initialize");
            public static FileReleaseDelgate File_Release = LibraryInstance.GetFunction<FileReleaseDelgate>("File_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 FileOpenDelgate(string filename, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 FileOpenStreamDelgate(IntPtr input_stream, string name, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 FileCreateDelgate(string filename, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 FileCreateStreamDelgate(IntPtr input_stream, string name, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 FileInitializeDelgate(IntPtr handle);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 FileReleaseDelgate(IntPtr handle);
        }
    }
}
