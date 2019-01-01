using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfPageTree : PdfUnknown
    {
        internal PdfPageTree(IntPtr handle) : base(handle)
        {
        }

        static PdfPageTree()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public UInt64 GetPageCount()
        {
            UInt32 result = NativeMethods.PageTree_GetPageCount(Handle, out UIntPtr count);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return count.ToUInt64();
        }

        public PdfPageObject GetPage(UInt64 index)
        {
            UInt32 result = NativeMethods.PageTree_GetPage(Handle, new UIntPtr(index), out IntPtr data);
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
            public delegate UInt32 PageTreeGetPageDelgate(IntPtr handle, UIntPtr at, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeGetPageCountDelgate(IntPtr handle, out UIntPtr count);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeReleaseDelgate(IntPtr handle);
        }
    }
}
