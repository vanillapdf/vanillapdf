using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfPageTree : PdfUnknown
    {
        internal PdfPageTree(IntPtr handle)
        {
            Handle = handle;
        }

        static PdfPageTree()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public int GetPageCount()
        {
            UInt32 result = NativeMethods.PageTree_GetPageCount(Handle, out int count);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return count;
        }

        public PdfPageObject GetPage(int index)
        {
            UInt32 result = NativeMethods.PageTree_GetPage(Handle, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfPageObject(data);
        }

        protected override UInt32 Release(IntPtr handle)
        {
            return NativeMethods.PageTree_Release(handle);
        }

        private static class NativeMethods
        {
            public static PageTreeGetPageDelgate PageTree_GetPage = LibraryInstance.GetFunction<PageTreeGetPageDelgate>("PageTree_GetPage");
            public static PageTreeGetPageCountDelgate PageTree_GetPageCount = LibraryInstance.GetFunction<PageTreeGetPageCountDelgate>("PageTree_GetPageCount");
            public static PageTreeReleaseDelgate PageTree_Release = LibraryInstance.GetFunction<PageTreeReleaseDelgate>("PageTree_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeGetPageDelgate(IntPtr handle, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeGetPageCountDelgate(IntPtr handle, out int count);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeReleaseDelgate(IntPtr handle);
        }
    }
}
