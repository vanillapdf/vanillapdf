using System;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class Catalog : IUnknown
    {
        internal IntPtr Handle { get; private set; }

        internal Catalog(IntPtr handle)
        {
            Handle = handle;
        }

        public PDFVersion GetVersion()
        {
            UInt32 result = NativeMethods.Catalog_GetVersion(Handle, out int param);
            if (result == ReturnValues.ERROR_OBJECT_MISSING) {
                throw BaseException.GetException(result);
            }

            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return EnumUtil<PDFVersion>.CheckedCast(param);
        }

        public bool TryGetVersion(ref PDFVersion version)
        {
            UInt32 result = NativeMethods.Catalog_GetVersion(Handle, out int param);
            if (result == ReturnValues.ERROR_OBJECT_MISSING) {
                return false;
            }

            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            version = EnumUtil<PDFVersion>.CheckedCast(param);
            return true;
        }

        public PageTree GetPageTree()
        {
            UInt32 result = NativeMethods.Catalog_GetPages(Handle, out IntPtr tree);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return new PageTree(tree);
        }

        protected override void Dispose(bool disposing)
        {
            if (Handle != IntPtr.Zero) {
                UInt32 result = NativeMethods.Catalog_Release(Handle);
                if (result != ReturnValues.ERROR_SUCCESS) {
                    throw Errors.GetLastErrorException();
                }

                Handle = IntPtr.Zero;
            }
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
