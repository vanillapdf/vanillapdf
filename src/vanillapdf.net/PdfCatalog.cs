using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfCatalog : PdfUnknown
    {
        internal PdfCatalogSafeHandle Handle { get; }

        internal PdfCatalog(PdfCatalogSafeHandle handle)
        {
            Handle = handle;
        }

        static PdfCatalog()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public PdfVersion? Version
        {
            get { return GetVersion(); }
        }

        public PdfVersion? GetVersion()
        {
            UInt32 result = NativeMethods.Catalog_GetVersion(Handle, out int data);
            if (result == PdfReturnValues.ERROR_OBJECT_MISSING) {
                return null;
            }

            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return EnumUtil<PdfVersion>.CheckedCast(data);
        }

        public PdfPageTree GetPageTree()
        {
            UInt32 result = NativeMethods.Catalog_GetPages(Handle, out PdfPageTreeSafeHandle data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfPageTree(data);
        }

        protected override void ReleaseManagedResources()
        {
            Handle.Dispose();
        }

        private static class NativeMethods
        {
            public static CatalogGetPagesDelgate Catalog_GetPages = LibraryInstance.GetFunction<CatalogGetPagesDelgate>("Catalog_GetPages");
            public static CatalogGetVersionDelgate Catalog_GetVersion = LibraryInstance.GetFunction<CatalogGetVersionDelgate>("Catalog_GetVersion");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 CatalogGetPagesDelgate(PdfCatalogSafeHandle handle, out PdfPageTreeSafeHandle data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 CatalogGetVersionDelgate(PdfCatalogSafeHandle handle, out int data);
        }
    }
}
