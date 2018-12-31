using System;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class Document : IUnknown
    {
        internal Document(IntPtr handle)
        {
            Handle = handle;
        }

        public static Document OpenNew(string filename)
        {
            UInt32 result = NativeMethods.Document_OpenNew(filename, out IntPtr data);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return new Document(data);
        }

        public static Document OpenExisting(File file)
        {
            UInt32 result = NativeMethods.Document_OpenExisting(file.Handle, out IntPtr data);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return new Document(data);
        }

        public void AppendDocument(Document source)
        {
            UInt32 result = NativeMethods.Document_AppendDocument(Handle, source.Handle);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }
        }

        public Catalog GetCatalog()
        {
            UInt32 result = NativeMethods.Document_GetCatalog(Handle, out IntPtr data);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }

            return new Catalog(data);
        }

        public void Save(string filename)
        {
            UInt32 result = NativeMethods.Document_Save(Handle, filename);
            if (result != ReturnValues.ERROR_SUCCESS) {
                throw Errors.GetLastErrorException();
            }
        }

        protected override UInt32 Release(IntPtr data)
        {
            return NativeMethods.Document_Release(data);
        }

        private static class NativeMethods
        {
            public static DocumentOpenNewDelgate Document_OpenNew = LibraryInstance.GetFunction<DocumentOpenNewDelgate>("Document_OpenNew");
            public static DocumentOpenExistingDelgate Document_OpenExisting = LibraryInstance.GetFunction<DocumentOpenExistingDelgate>("Document_OpenExisting");
            public static DocumentAppendDocumentDelgate Document_AppendDocument = LibraryInstance.GetFunction<DocumentAppendDocumentDelgate>("Document_AppendDocument");
            public static DocumentGetCatalogDelgate Document_GetCatalog = LibraryInstance.GetFunction<DocumentGetCatalogDelgate>("Document_GetCatalog");
            public static DocumentSaveDelgate Document_Save = LibraryInstance.GetFunction<DocumentSaveDelgate>("Document_Save");
            public static DocumentReleaseDelgate Document_Release = LibraryInstance.GetFunction<DocumentReleaseDelgate>("Document_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentOpenNewDelgate(string filename, out IntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 DocumentOpenExistingDelgate(IntPtr file, out IntPtr data);

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
