using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfPageObject : PdfUnknown
    {
        internal PdfPageObject(IntPtr handle)
        {
            Handle = handle;
        }

        static PdfPageObject()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        protected override UInt32 Release(IntPtr data)
        {
            return NativeMethods.PageObject_Release(data);
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
