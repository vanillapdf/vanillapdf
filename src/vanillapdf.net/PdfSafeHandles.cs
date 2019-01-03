using System;
using System.Runtime.ConstrainedExecution;
using System.Runtime.InteropServices;
using System.Security;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    internal abstract class PdfSafeHandle : SafeHandle
    {
        //private GenericReleaseDelgate ReleaseMethod { get; }
        protected abstract GenericReleaseDelgate ReleaseDelegate { get; }

        public PdfSafeHandle() : base(IntPtr.Zero, true)
        {
        }

        //public PdfSafeHandle(IntPtr value) : base(IntPtr.Zero, true)
        //{
        //    if (value == IntPtr.Zero) {
        //        throw new ArgumentNullException(nameof(value));
        //    }

        //    SetHandle(value);
        //    ReleaseMethod = LibraryInstance.GetFunction<GenericReleaseDelgate>(ReleaseFunctionName);
        //}

        public override bool IsInvalid
        {
            [PrePrepareMethod]
            [ReliabilityContract(Consistency.WillNotCorruptState, Cer.Success)]
            get { return (handle == IntPtr.Zero); }
        }

        [PrePrepareMethod]
        [ReliabilityContract(Consistency.WillNotCorruptState, Cer.MayFail)]
        protected override bool ReleaseHandle()
        {
            if (ReleaseDelegate == null) {
                return false;
            }

            return (ReleaseDelegate(handle) == PdfReturnValues.ERROR_SUCCESS);
        }

        [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention), SuppressUnmanagedCodeSecurity]
        public delegate UInt32 GenericReleaseDelgate(IntPtr handle);
    }

    internal sealed class PdfUnknownSafeHandle : PdfSafeHandle
    {
        private static GenericReleaseDelgate StaticReleaseDelegate = LibraryInstance.GetFunction<GenericReleaseDelgate>("IUnknown_Release");
        protected override GenericReleaseDelgate ReleaseDelegate => StaticReleaseDelegate;
    }

    internal sealed class PdfBufferSafeHandle : PdfSafeHandle
    {
        private static GenericReleaseDelgate StaticReleaseDelegate = LibraryInstance.GetFunction<GenericReleaseDelgate>("Buffer_Release");
        protected override GenericReleaseDelgate ReleaseDelegate => StaticReleaseDelegate;
    }

    internal sealed class PdfAnnotationSafeHandle : PdfSafeHandle
    {
        private static GenericReleaseDelgate StaticReleaseDelegate = LibraryInstance.GetFunction<GenericReleaseDelgate>("Annotation_Release");
        protected override GenericReleaseDelgate ReleaseDelegate => StaticReleaseDelegate;
    }

    internal sealed class PdfCatalogSafeHandle : PdfSafeHandle
    {
        private static GenericReleaseDelgate StaticReleaseDelegate = LibraryInstance.GetFunction<GenericReleaseDelgate>("Catalog_Release");
        protected override GenericReleaseDelgate ReleaseDelegate => StaticReleaseDelegate;
    }

    internal sealed class PdfContentsSafeHandle : PdfSafeHandle
    {
        private static GenericReleaseDelgate StaticReleaseDelegate = LibraryInstance.GetFunction<GenericReleaseDelgate>("Contents_Release");
        protected override GenericReleaseDelgate ReleaseDelegate => StaticReleaseDelegate;
    }

    internal sealed class PdfDocumentSafeHandle : PdfSafeHandle
    {
        private static GenericReleaseDelgate StaticReleaseDelegate = LibraryInstance.GetFunction<GenericReleaseDelgate>("Document_Release");
        protected override GenericReleaseDelgate ReleaseDelegate => StaticReleaseDelegate;
    }

    internal sealed class PdfFileSafeHandle : PdfSafeHandle
    {
        private static GenericReleaseDelgate StaticReleaseDelegate = LibraryInstance.GetFunction<GenericReleaseDelgate>("File_Release");
        protected override GenericReleaseDelgate ReleaseDelegate => StaticReleaseDelegate;
    }

    internal sealed class PdfInputStreamSafeHandle : PdfSafeHandle
    {
        private static GenericReleaseDelgate StaticReleaseDelegate = LibraryInstance.GetFunction<GenericReleaseDelgate>("InputStream_Release");
        protected override GenericReleaseDelgate ReleaseDelegate => StaticReleaseDelegate;
    }

    internal sealed class PdfPageAnnotationsSafeHandle : PdfSafeHandle
    {
        private static GenericReleaseDelgate StaticReleaseDelegate = LibraryInstance.GetFunction<GenericReleaseDelgate>("PageAnnotations_Release");
        protected override GenericReleaseDelgate ReleaseDelegate => StaticReleaseDelegate;
    }

    internal sealed class PdfPageObjectSafeHandle : PdfSafeHandle
    {
        private static GenericReleaseDelgate StaticReleaseDelegate = LibraryInstance.GetFunction<GenericReleaseDelgate>("PageObject_Release");
        protected override GenericReleaseDelgate ReleaseDelegate => StaticReleaseDelegate;
    }

    internal sealed class PdfPageTreeSafeHandle : PdfSafeHandle
    {
        private static GenericReleaseDelgate StaticReleaseDelegate = LibraryInstance.GetFunction<GenericReleaseDelgate>("PageTree_Release");
        protected override GenericReleaseDelgate ReleaseDelegate => StaticReleaseDelegate;
    }
}
