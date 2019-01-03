using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfDocument : PdfUnknown
    {
        internal PdfDocumentSafeHandle Handle { get; }

        internal PdfDocument(PdfDocumentSafeHandle handle)
        {
            Handle = handle;
        }

        static PdfDocument()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public static PdfDocument Open(string filename)
        {
            UInt32 result = NativeMethods.Document_Open(filename, out var data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfDocument(data);
        }

        public static PdfDocument Create(string filename)
        {
            UInt32 result = NativeMethods.Document_Create(filename, out var data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return new PdfDocument(data);
        }

        public static PdfDocument OpenFile(PdfFile file)
        {
            UInt32 result = NativeMethods.Document_OpenFile(file.Handle, out var data);
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
            UInt32 result = NativeMethods.Document_GetCatalog(Handle, out var data);
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

        protected override void ReleaseManagedResources()
        {
            Handle.Dispose();
        }

        private static class NativeMethods
        {
            public static DocumentOpenDelgate Document_Open = LibraryInstance.GetFunction<DocumentOpenDelgate>("Document_Open");
            public static DocumentCreateDelgate Document_Create = LibraryInstance.GetFunction<DocumentCreateDelgate>("Document_Create");
            public static DocumentOpenFileDelgate Document_OpenFile = LibraryInstance.GetFunction<DocumentOpenFileDelgate>("Document_OpenFile");
            public static DocumentAppendDocumentDelgate Document_AppendDocument = LibraryInstance.GetFunction<DocumentAppendDocumentDelgate>("Document_AppendDocument");
            public static DocumentGetCatalogDelgate Document_GetCatalog = LibraryInstance.GetFunction<DocumentGetCatalogDelgate>("Document_GetCatalog");
            public static DocumentSaveDelgate Document_Save = LibraryInstance.GetFunction<DocumentSaveDelgate>("Document_Save");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentOpenDelgate(string filename, out PdfDocumentSafeHandle data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentCreateDelgate(string filename, out PdfDocumentSafeHandle data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentOpenFileDelgate(PdfFileSafeHandle file, out PdfDocumentSafeHandle data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentAppendDocumentDelgate(PdfDocumentSafeHandle handle, PdfDocumentSafeHandle source);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentGetCatalogDelgate(PdfDocumentSafeHandle handle, out PdfCatalogSafeHandle data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentSaveDelgate(PdfDocumentSafeHandle handle, string filename);
        }
    }
}
