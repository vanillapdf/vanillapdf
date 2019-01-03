using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfPageObject : PdfUnknown
    {
        internal PdfPageObjectSafeHandle Handle { get; }

        internal PdfPageObject(PdfPageObjectSafeHandle handle)
        {
            Handle = handle;
        }

        static PdfPageObject()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public PdfContents GetContents()
        {
            UInt32 result = NativeMethods.PageObject_GetContents(Handle, out var data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfContents(data);
        }

        public PdfPageAnnotations GetAnnotations()
        {
            UInt32 result = NativeMethods.PageObject_GetAnnotations(Handle, out var data);
            if (result == PdfReturnValues.ERROR_OBJECT_MISSING) {
                var aa = data.DangerousGetHandle();

                data.SetHandleAsInvalid();
                data.Close();
                data.Dispose();
                return null;
            }

            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            //PdfPageAnnotationsSafeHandle newHandle = new PdfPageAnnotationsSafeHandle(data);
            return new PdfPageAnnotations(data);
        }

        protected override void ReleaseManagedResources()
        {
            Handle.Dispose();
        }

        private static class NativeMethods
        {
            public static PageObjectGetContentsDelgate PageObject_GetContents = LibraryInstance.GetFunction<PageObjectGetContentsDelgate>("PageObject_GetContents");
            public static PageObjectGetAnnotationsDelgate PageObject_GetAnnotations = LibraryInstance.GetFunction<PageObjectGetAnnotationsDelgate>("PageObject_GetAnnotations");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageObjectGetContentsDelgate(PdfPageObjectSafeHandle handle, out PdfContentsSafeHandle data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageObjectGetAnnotationsDelgate(PdfPageObjectSafeHandle handle, out PdfPageAnnotationsSafeHandle data);
        }
    }
}
