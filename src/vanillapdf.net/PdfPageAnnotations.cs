using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfPageAnnotations : PdfUnknown
    {
        internal PdfPageAnnotations(IntPtr handle) : base(handle)
        {
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

        public UInt64 At(UInt64 index)
        {
            UInt32 result = NativeMethods.PageAnnotations_At(Handle, new UIntPtr(index), out UIntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return data.ToUInt64();
        }

        protected override UInt32 Release(IntPtr data)
        {
            return NativeMethods.PageAnnotations_Release(data);
        }

        private static class NativeMethods
        {
            public static PageAnnotationsSizeDelgate PageAnnotations_Size = LibraryInstance.GetFunction<PageAnnotationsSizeDelgate>("PageAnnotations_Size");
            public static PageAnnotationsAtDelgate PageAnnotations_At = LibraryInstance.GetFunction<PageAnnotationsAtDelgate>("PageAnnotations_At");
            public static PageAnnotationsReleaseDelgate PageAnnotations_Release = LibraryInstance.GetFunction<PageAnnotationsReleaseDelgate>("PageAnnotations_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageAnnotationsSizeDelgate(IntPtr handle, out UIntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageAnnotationsAtDelgate(IntPtr handle, UIntPtr index, out UIntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 PageAnnotationsReleaseDelgate(IntPtr handle);
        }
    }
}
