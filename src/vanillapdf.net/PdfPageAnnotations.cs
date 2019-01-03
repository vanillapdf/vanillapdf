using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfPageAnnotations : PdfUnknown
    {
        internal PdfPageAnnotationsSafeHandle Handle { get; }

        internal PdfPageAnnotations(PdfPageAnnotationsSafeHandle handle)
        {
            Handle = handle;
        }

        static PdfPageAnnotations()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public UInt64 Size()
        {
            UInt32 result = NativeMethods.PageAnnotations_Size(Handle, out UIntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return data.ToUInt64();
        }

        public PdfAnnotation At(UInt64 index)
        {
            UInt32 result = NativeMethods.PageAnnotations_At(Handle, new UIntPtr(index), out PdfAnnotationSafeHandle data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfAnnotation(data);
        }

        protected override void ReleaseManagedResources()
        {
            Handle.Dispose();
        }

        private static class NativeMethods
        {
            public static PageAnnotationsSizeDelgate PageAnnotations_Size = LibraryInstance.GetFunction<PageAnnotationsSizeDelgate>("PageAnnotations_Size");
            public static PageAnnotationsAtDelgate PageAnnotations_At = LibraryInstance.GetFunction<PageAnnotationsAtDelgate>("PageAnnotations_At");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageAnnotationsSizeDelgate(PdfPageAnnotationsSafeHandle handle, out UIntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageAnnotationsAtDelgate(PdfPageAnnotationsSafeHandle handle, UIntPtr index, out PdfAnnotationSafeHandle data);
        }
    }
}
