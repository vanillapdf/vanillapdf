using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public enum PdfAnnotationType
    {
        Text = 0,
        Link,
        FreeText,
        Line,
        Square,
        Circle,
        Polygon,
        PolyLine,
        Highlight,
        Underline,
        Squiggly,
        StrikeOut,
        RubberStamp,
        Caret,
        Ink,
        Popup,
        FileAttachment,
        Sound,
        Movie,
        Widget,
        Screen,
        PrinterMark,
        TrapNetwork,
        Watermark,
        TripleD,
        Redaction,
    };

    public class PdfAnnotation : PdfUnknown
    {
        internal PdfAnnotation(IntPtr handle) : base(handle)
        {
        }

        static PdfAnnotation()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public PdfAnnotationType GetAnnotationType()
        {
            UInt32 result = NativeMethods.Annotation_GetType(Handle, out int data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return EnumUtil<PdfAnnotationType>.CheckedCast(data);
        }

        protected override UInt32 Release(IntPtr data)
        {
            return NativeMethods.Annotation_Release(data);
        }

        private static class NativeMethods
        {
            public static AnnotationGetTypeDelgate Annotation_GetType = LibraryInstance.GetFunction<AnnotationGetTypeDelgate>("Annotation_GetType");
            public static AnnotationReleaseDelgate Annotation_Release = LibraryInstance.GetFunction<AnnotationReleaseDelgate>("Annotation_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 AnnotationGetTypeDelgate(IntPtr handle, out int data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 AnnotationReleaseDelgate(IntPtr handle);
        }
    }
}
