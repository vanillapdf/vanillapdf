using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfBuffer : PdfUnknown
    {
        internal PdfBufferSafeHandle Handle { get; }

        internal PdfBuffer(PdfBufferSafeHandle handle)
        {
            Handle = handle;
        }

        static PdfBuffer()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public byte[] Data
        {
            get { return GetData(); }
            set { SetData(value); }
        }

        public static PdfBuffer Create()
        {
            UInt32 result = NativeMethods.Buffer_Create(out PdfBufferSafeHandle handle);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfBuffer(handle);
        }

        public byte[] GetData()
        {
            UInt32 result = NativeMethods.Buffer_GetData(Handle, out IntPtr data, out UIntPtr size);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            // TODO: might overflow
            var rawSize = size.ToUInt64();
            var sizeConverted = Convert.ToInt32(rawSize);

            byte[] allocatedBuffer = new byte[sizeConverted];
            Marshal.Copy(data, allocatedBuffer, 0, sizeConverted);

            return allocatedBuffer;
        }

        public void SetData(byte[] data)
        {
            IntPtr allocator = IntPtr.Zero;
            UInt32 result = PdfReturnValues.ERROR_GENERAL;

            try {
                allocator = Marshal.AllocHGlobal(data.Length);
                Marshal.Copy(data, 0, allocator, data.Length);

                var dataSize = Convert.ToUInt64(data.Length);
                result = NativeMethods.Buffer_SetData(Handle, allocator, new UIntPtr(dataSize));

            } finally {
                if (allocator != IntPtr.Zero) {
                    Marshal.FreeHGlobal(allocator);
                    allocator = IntPtr.Zero;
                }
            }

            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }
        }

        public PdfInputStream ToInputStream(string name)
        {
            UInt32 result = NativeMethods.Buffer_ToInputStream(Handle, name, out PdfInputStreamSafeHandle handle);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfInputStream(handle);
        }

        protected override void ReleaseManagedResources()
        {
            Handle.Dispose();
        }

        private static class NativeMethods
        {
            public static BufferCreateDelgate Buffer_Create = LibraryInstance.GetFunction<BufferCreateDelgate>("Buffer_Create");
            public static BufferGetDataDelgate Buffer_GetData = LibraryInstance.GetFunction<BufferGetDataDelgate>("Buffer_GetData");
            public static BufferSetDataDelgate Buffer_SetData = LibraryInstance.GetFunction<BufferSetDataDelgate>("Buffer_SetData");
            public static BufferToInputStreamDelgate Buffer_ToInputStream = LibraryInstance.GetFunction<BufferToInputStreamDelgate>("Buffer_ToInputStream");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 BufferCreateDelgate(out PdfBufferSafeHandle handle);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 BufferGetDataDelgate(PdfBufferSafeHandle handle, out IntPtr data, out UIntPtr size);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 BufferSetDataDelgate(PdfBufferSafeHandle handle, IntPtr data, UIntPtr size);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 BufferToInputStreamDelgate(PdfBufferSafeHandle handle, string name, out PdfInputStreamSafeHandle data);
        }
    }
}
