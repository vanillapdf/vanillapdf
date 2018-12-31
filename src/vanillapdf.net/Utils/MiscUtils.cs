using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace vanillapdf.net.Utils
{
    public static class MiscUtils
    {
        public const CallingConvention LibraryCallingConvention = CallingConvention.Cdecl;

        public static void InitializeClasses()
        {
            RuntimeHelpers.RunClassConstructor(typeof(PdfUnknown).TypeHandle);
            RuntimeHelpers.RunClassConstructor(typeof(PdfBuffer).TypeHandle);
            RuntimeHelpers.RunClassConstructor(typeof(PdfInputstream).TypeHandle);
            RuntimeHelpers.RunClassConstructor(typeof(PdfLogging).TypeHandle);
            RuntimeHelpers.RunClassConstructor(typeof(PdfErrors).TypeHandle);
            RuntimeHelpers.RunClassConstructor(typeof(PdfReturnValues).TypeHandle);

            RuntimeHelpers.RunClassConstructor(typeof(PdfFile).TypeHandle);

            RuntimeHelpers.RunClassConstructor(typeof(PdfCatalog).TypeHandle);
            RuntimeHelpers.RunClassConstructor(typeof(PdfDocument).TypeHandle);
            RuntimeHelpers.RunClassConstructor(typeof(PdfPageObject).TypeHandle);
            RuntimeHelpers.RunClassConstructor(typeof(PdfPageTree).TypeHandle);
        }
    }
}
