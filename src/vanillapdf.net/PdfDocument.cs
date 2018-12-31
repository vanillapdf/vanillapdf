using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfDocument : PdfUnknown
    {
        internal PdfDocument(IntPtr handle)
        {
            Handle = handle;
        }

        static PdfDocument()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public static PdfDocument Open(string filename)
        {
            UInt32 result = NativeMethods.Document_Open(filename, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfDocument(data);
        }

        public static PdfDocument Create(string filename)
        {
            UInt32 result = NativeMethods.Document_Create(filename, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfDocument(data);
        }

        public static PdfDocument OpenFile(PdfFile file)
        {
            UInt32 result = NativeMethods.Document_OpenFile(file.Handle, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfDocument(data);
        }

        public void AppendDocument(PdfDocument source)
        {
            UInt32 result = NativeMethods.Document_AppendDocument(Handle, source.Handle);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }
        }

        public PdfCatalog GetCatalog()
        {
            UInt32 result = NativeMethods.Document_GetCatalog(Handle, out IntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfCatalog(data);
        }

        public void Save(string filename)
        {
            UInt32 result = NativeMethods.Document_Save(Handle, filename);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }
        }

        protected override UInt32 Release(IntPtr data)
        {
            return NativeMethods.Document_Release(data);
        }

        private static class NativeMethods
        {
            public static DocumentOpenDelgate Document_Open = LibraryInstance.GetFunction<DocumentOpenDelgate>("Document_Open");
            public static DocumentCreateDelgate Document_Create = LibraryInstance.GetFunction<DocumentCreateDelgate>("Document_Create");
            public static DocumentOpenFileDelgate Document_OpenFile = LibraryInstance.GetFunction<DocumentOpenFileDelgate>("Document_OpenFile");
            public static DocumentAppendDocumentDelgate Document_AppendDocument = LibraryInstance.GetFunction<DocumentAppendDocumentDelgate>("Document_AppendDocument");
            public static DocumentGetCatalogDelgate Document_GetCatalog = LibraryInstance.GetFunction<DocumentGetCatalogDelgate>("Document_GetCatalog");
            public static DocumentSaveDelgate Document_Save = LibraryInstance.GetFunction<DocumentSaveDelgate>("Document_Save");
            public static DocumentReleaseDelgate Document_Release = LibraryInstance.GetFunction<DocumentReleaseDelgate>("Document_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentOpenDelgate(string filename, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentCreateDelgate(string filename, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentOpenFileDelgate(IntPtr file, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentAppendDocumentDelgate(IntPtr handle, IntPtr source);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentGetCatalogDelgate(IntPtr handle, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentSaveDelgate(IntPtr handle, string filename);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentReleaseDelgate(IntPtr handle);
        }
    }
}
