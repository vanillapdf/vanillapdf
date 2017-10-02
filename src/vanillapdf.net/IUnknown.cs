using System;
using System.Runtime.InteropServices;

namespace vanillapdf.net
{
    public abstract class IUnknown : IDisposable
    {
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected abstract void Dispose(bool disposing);

        private static class NativeMethods
        {
            public static IUnknownAddRefDelgate IUnknown_AddRef = LibraryInstance.GetFunction<IUnknownAddRefDelgate>("IUnknown_AddRef");
            public static IUnknownReleaseDelgate IUnknown_Release = LibraryInstance.GetFunction<IUnknownReleaseDelgate>("IUnknown_Release");

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate UInt32 IUnknownAddRefDelgate(IntPtr handle);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate UInt32 IUnknownReleaseDelgate(IntPtr handle);
        }
    }
}
