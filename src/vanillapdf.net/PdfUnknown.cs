using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public abstract class PdfUnknown : IDisposable
    {
        protected internal IntPtr Handle { get; protected set; }

        static PdfUnknown()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected abstract UInt32 Release(IntPtr data);
        protected virtual void Dispose(bool disposing)
        {
            if (Handle != IntPtr.Zero) {
                UInt32 result = Release(Handle);
                if (result != PdfReturnValues.ERROR_SUCCESS) {
                    throw PdfErrors.GetLastErrorException();
                }

                Handle = IntPtr.Zero;
            }
        }

        private static class NativeMethods
        {
            public static IUnknownAddRefDelgate IUnknown_AddRef = LibraryInstance.GetFunction<IUnknownAddRefDelgate>("IUnknown_AddRef");
            public static IUnknownReleaseDelgate IUnknown_Release = LibraryInstance.GetFunction<IUnknownReleaseDelgate>("IUnknown_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 IUnknownAddRefDelgate(IntPtr handle);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 IUnknownReleaseDelgate(IntPtr handle);
        }
    }
}
