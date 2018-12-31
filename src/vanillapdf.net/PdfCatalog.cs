using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfCatalog : PdfUnknown
    {
        internal PdfCatalog(IntPtr handle)
        {
            Handle = handle;
        }

        static PdfCatalog()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public PdfVersion GetVersion()
        {
            UInt32 result = NativeMethods.Catalog_GetVersion(Handle, out int param);
            if (result == PdfReturnValues.ERROR_OBJECT_MISSING) {
                throw PdfBaseException.GetException(result);
            }

            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return EnumUtil<PdfVersion>.CheckedCast(param);
        }

        public bool TryGetVersion(ref PdfVersion version)
        {
            UInt32 result = NativeMethods.Catalog_GetVersion(Handle, out int param);
            if (result == PdfReturnValues.ERROR_OBJECT_MISSING) {
                return false;
            }

            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            version = EnumUtil<PdfVersion>.CheckedCast(param);
            return true;
        }

        public PdfPageTree GetPageTree()
        {
            UInt32 result = NativeMethods.Catalog_GetPages(Handle, out IntPtr tree);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfPageTree(tree);
        }

        protected override UInt32 Release(IntPtr data)
        {
            return NativeMethods.Catalog_Release(data);
        }

        private static class NativeMethods
        {
            public static CatalogGetPagesDelgate Catalog_GetPages = LibraryInstance.GetFunction<CatalogGetPagesDelgate>("Catalog_GetPages");
            public static CatalogGetVersionDelgate Catalog_GetVersion = LibraryInstance.GetFunction<CatalogGetVersionDelgate>("Catalog_GetVersion");
            public static CatalogReleaseDelgate Catalog_Release = LibraryInstance.GetFunction<CatalogReleaseDelgate>("Catalog_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 CatalogGetPagesDelgate(IntPtr handle, out IntPtr version);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 CatalogGetVersionDelgate(IntPtr handle, out int version);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 CatalogReleaseDelgate(IntPtr handle);
        }
    }
}
