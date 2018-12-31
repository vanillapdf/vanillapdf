using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfInputstream : PdfUnknown
    {
        internal PdfInputstream(IntPtr handle)
        {
            Handle = handle;
        }

        static PdfInputstream()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public Int64 InputPosition
        {
            get { return GetInputPosition(); }
            set { SetInputPosition(value); }
        }

        public static PdfInputstream CreateFromFile(string filename)
        {
            UInt32 result = NativeMethods.InputStream_CreateFromFile(filename, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfInputstream(data);
        }

        public static PdfInputstream CreateFromBuffer(PdfBuffer buffer)
        {
            UInt32 result = NativeMethods.InputStream_CreateFromBuffer(buffer.Handle, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfInputstream(data);
        }

        public PdfBuffer ToBuffer()
        {
            UInt32 result = NativeMethods.InputStream_ToBuffer(Handle, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfBuffer(data);
        }

        public Int64 GetInputPosition()
        {
            UInt32 result = NativeMethods.InputStream_GetInputPosition(Handle, out Int64 data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return data;
        }

        public void SetInputPosition(Int64 data)
        {
            UInt32 result = NativeMethods.InputStream_SetInputPosition(Handle, data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }
        }

        //public static implicit operator Inputstream(Stream stream)
        //{
        //    return new Inputstream(IntPtr.Zero);
        //}

        protected override UInt32 Release(IntPtr data)
        {
            return NativeMethods.InputStream_Release(data);
        }

        private static class NativeMethods
        {
            public static InputStreamCreateFromFileDelgate InputStream_CreateFromFile = LibraryInstance.GetFunction<InputStreamCreateFromFileDelgate>("InputStream_CreateFromFile");
            public static InputStreamCreateFromBufferDelgate InputStream_CreateFromBuffer = LibraryInstance.GetFunction<InputStreamCreateFromBufferDelgate>("InputStream_CreateFromBuffer");
            public static InputStreamToBufferDelgate InputStream_ToBuffer = LibraryInstance.GetFunction<InputStreamToBufferDelgate>("InputStream_ToBuffer");
            public static InputStreamGetInputPositionDelgate InputStream_GetInputPosition = LibraryInstance.GetFunction<InputStreamGetInputPositionDelgate>("InputStream_GetInputPosition");
            public static InputStreamSetInputPositionDelgate InputStream_SetInputPosition = LibraryInstance.GetFunction<InputStreamSetInputPositionDelgate>("InputStream_SetInputPosition");
            public static InputStreamReleaseDelgate InputStream_Release = LibraryInstance.GetFunction<InputStreamReleaseDelgate>("InputStream_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 InputStreamCreateFromFileDelgate(string filename, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 InputStreamCreateFromBufferDelgate(IntPtr buffer, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 InputStreamToBufferDelgate(IntPtr handle, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 InputStreamGetInputPositionDelgate(IntPtr handle, out Int64 data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 InputStreamSetInputPositionDelgate(IntPtr handle, Int64 data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 InputStreamReleaseDelgate(IntPtr handle);
        }
    }
}
