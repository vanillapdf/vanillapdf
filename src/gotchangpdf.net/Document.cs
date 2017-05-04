using System;
using System.Runtime.InteropServices;

namespace gotchangpdf.net
{
    public class Document : IUnknown
    {
        internal IntPtr Handle { get; private set; }

        private Document(IntPtr handle)
        {
            Handle = handle;
        }

        public static Document OpenNew(string filename)
        {
            int result = NativeMethods.Document_OpenNew(filename, out IntPtr handle);
            if (result != ReturnValues.ERROR_SUCCES) {
                throw new Exception("Could not open new document");
            }

            return new Document(handle);
        }

        public static Document OpenExisting(File file)
        {
            int result = NativeMethods.Document_OpenExisting(file.Handle, out IntPtr handle);
            if (result != ReturnValues.ERROR_SUCCES) {
                throw new Exception("Could not open existing document");
            }

            return new Document(handle);
        }

        public void AppendDocument(Document source)
        {
            int result = NativeMethods.Document_AppendDocument(Handle, source.Handle);
            if (result != ReturnValues.ERROR_SUCCES) {
                throw new Exception("Could not append document");
            }
        }

        public void Save(string filename)
        {
            int result = NativeMethods.Document_Save(Handle, filename);
            if (result != ReturnValues.ERROR_SUCCES) {
                throw new Exception("Could not save document");
            }
        }

        protected override void Dispose(bool disposing)
        {
            if (Handle != IntPtr.Zero) {
                int result = NativeMethods.Document_Release(Handle);
                if (result != ReturnValues.ERROR_SUCCES) {
                    throw new Exception("Could not release document");
                }

                Handle = IntPtr.Zero;
            }
        }

        private static class NativeMethods
        {
            public static DocumentOpenNewDelgate Document_OpenNew = LibraryInstance.GetFunction<DocumentOpenNewDelgate>("Document_OpenNew");
            public static DocumentOpenExistingDelgate Document_OpenExisting = LibraryInstance.GetFunction<DocumentOpenExistingDelgate>("Document_OpenExisting");
            public static DocumentAppendDocumentDelgate Document_AppendDocument = LibraryInstance.GetFunction<DocumentAppendDocumentDelgate>("Document_AppendDocument");
            public static DocumentSaveDelgate Document_Save = LibraryInstance.GetFunction<DocumentSaveDelgate>("Document_Save");
            public static DocumentReleaseDelgate Document_Release = LibraryInstance.GetFunction<DocumentReleaseDelgate>("Document_Release");

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int DocumentOpenNewDelgate(string filename, out IntPtr handle);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int DocumentOpenExistingDelgate(IntPtr file, out IntPtr handle);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int DocumentAppendDocumentDelgate(IntPtr handle, IntPtr source);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int DocumentSaveDelgate(IntPtr handle, string filename);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate int DocumentReleaseDelgate(IntPtr handle);
        }
    }
}
