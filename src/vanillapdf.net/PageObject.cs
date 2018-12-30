using System;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PageObject : IUnknown
    {
        internal IntPtr Handle { get; private set; }

        internal PageObject(IntPtr handle)
        {
            Handle = handle;
        }

        protected override void Dispose(bool disposing)
        {
            if (Handle != IntPtr.Zero) {
                UInt32 result = NativeMethods.PageObject_Release(Handle);
                if (result != ReturnValues.ERROR_SUCCESS) {
                    throw Errors.GetLastErrorException();
                }

                Handle = IntPtr.Zero;
            }
        }

        private static class NativeMethods
        {
            public static PageTreeGetPageDelgate PageTree_GetPage = LibraryInstance.GetFunction<PageTreeGetPageDelgate>("PageTree_GetPage");
            public static PageTreeGetPageCountDelgate PageTree_GetPageCount = LibraryInstance.GetFunction<PageTreeGetPageCountDelgate>("PageTree_GetPageCount");
            public static PageObjectReleaseDelgate PageObject_Release = LibraryInstance.GetFunction<PageObjectReleaseDelgate>("PageObject_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeGetPageDelgate(IntPtr handle, out int count);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeGetPageCountDelgate(IntPtr handle, out int count);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageObjectReleaseDelgate(IntPtr handle);
        }
    }
}
