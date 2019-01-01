using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfPageObject : PdfUnknown
    {
        internal PdfPageObject(IntPtr handle) : base(handle)
        {
        }

        static PdfPageObject()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public PdfContents GetContents()
        {
            UInt32 result = NativeMethods.PageObject_GetContents(Handle, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfContents(data);
        }

        public PdfPageAnnotations GetAnnotations()
        {
            UInt32 result = NativeMethods.PageObject_GetAnnotations(Handle, out IntPtr data);
            if (result == PdfReturnValues.ERROR_OBJECT_MISSING) {
                return null;
            }

            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfPageAnnotations(data);
        }

        protected override UInt32 Release(IntPtr data)
        {
            return NativeMethods.PageObject_Release(data);
        }

        private static class NativeMethods
        {
            public static PageObjectGetContentsDelgate PageObject_GetContents = LibraryInstance.GetFunction<PageObjectGetContentsDelgate>("PageObject_GetContents");
            public static PageObjectGetAnnotationsDelgate PageObject_GetAnnotations = LibraryInstance.GetFunction<PageObjectGetAnnotationsDelgate>("PageObject_GetAnnotations");
            public static PageObjectReleaseDelgate PageObject_Release = LibraryInstance.GetFunction<PageObjectReleaseDelgate>("PageObject_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageObjectGetContentsDelgate(IntPtr handle, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageObjectGetAnnotationsDelgate(IntPtr handle, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageObjectReleaseDelgate(IntPtr handle);
        }
    }
}
