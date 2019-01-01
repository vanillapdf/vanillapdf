using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfFile : PdfUnknown
    {
        internal PdfFile(IntPtr handle) : base(handle)
        {
        }

        static PdfFile()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public static PdfFile Open(string filename)
        {
            UInt32 result = NativeMethods.File_Open(filename, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfFile(data);
        }

        public static PdfFile OpenStream(PdfInputstream stream, string filename)
        {
            UInt32 result = NativeMethods.File_OpenStream(stream.Handle, filename, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfFile(data);
        }

        public static PdfFile Create(string filename)
        {
            UInt32 result = NativeMethods.File_Create(filename, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfFile(data);
        }

        public static PdfFile CreateStream(PdfInputstream stream, string name)
        {
            UInt32 result = NativeMethods.File_CreateStream(stream.Handle, name, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfFile(data);
        }

        public void Initialize()
        {
            UInt32 result = NativeMethods.File_Initialize(Handle);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }
        }

        public bool IsEncrypted()
        {
            UInt32 result = NativeMethods.File_IsEncrypted(Handle, out bool data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return data;
        }

        public void SetEncryptionPassword(string password)
        {
            UInt32 result = NativeMethods.File_SetEncryptionPassword(Handle, password);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
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
            public static FileIsEncryptedDelgate File_IsEncrypted = LibraryInstance.GetFunction<FileIsEncryptedDelgate>("File_IsEncrypted");
            public static FileSetEncryptionPasswordDelgate File_SetEncryptionPassword = LibraryInstance.GetFunction<FileSetEncryptionPasswordDelgate>("File_SetEncryptionPassword");
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
            public delegate UInt32 FileIsEncryptedDelgate(IntPtr handle, out bool data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 FileSetEncryptionPasswordDelgate(IntPtr handle, string password);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 FileReleaseDelgate(IntPtr handle);
        }
    }
}
