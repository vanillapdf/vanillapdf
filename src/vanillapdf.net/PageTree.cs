using System;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PageTree : IUnknown
    {
        internal IntPtr Handle { get; private set; }

        internal PageTree(IntPtr handle)
        {
            Handle = handle;
        }

        public int GetPageCount()
        {
            UInt32 result = NativeMethods.PageTree_GetPageCount(Handle, out int count);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return count;
        }

        public PageObject GetPage(int index)
        {
            UInt32 result = NativeMethods.PageTree_GetPage(Handle, out IntPtr page);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return new PageObject(page);
        }

        protected override void Dispose(bool disposing)
        {
            if (Handle != IntPtr.Zero) {
                UInt32 result = NativeMethods.PageTree_Release(Handle);
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
            public static PageTreeReleaseDelgate PageTree_Release = LibraryInstance.GetFunction<PageTreeReleaseDelgate>("PageTree_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeGetPageDelgate(IntPtr handle, out IntPtr page);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeGetPageCountDelgate(IntPtr handle, out int count);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageTreeReleaseDelgate(IntPtr handle);
        }
    }
}
