using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfPageTree : PdfUnknown
    {
        internal PdfPageTreeSafeHandle Handle { get; }

        internal PdfPageTree(PdfPageTreeSafeHandle handle)
        {
            Handle = handle;
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
            UInt32 result = NativeMethods.PageTree_GetPage(Handle, new UIntPtr(index), out PdfPageObjectSafeHandle data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfPageObject(data);
        }

        protected override void ReleaseManagedResources()
        {
            Handle.Dispose();
        }

        private static class NativeMethods
        {
            public static PageTreeGetPageDelgate PageTree_GetPage = LibraryInstance.GetFunction<PageTreeGetPageDelgate>("PageTree_GetPage");
            public static PageTreeGetPageCountDelgate PageTree_GetPageCount = LibraryInstance.GetFunction<PageTreeGetPageCountDelgate>("PageTree_GetPageCount");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeGetPageDelgate(PdfPageTreeSafeHandle handle, UIntPtr at, out PdfPageObjectSafeHandle data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeGetPageCountDelgate(PdfPageTreeSafeHandle handle, out UIntPtr count);
        }
    }
}
