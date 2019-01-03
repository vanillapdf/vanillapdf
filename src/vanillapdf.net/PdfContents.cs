using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfContents : PdfUnknown
    {
        internal PdfContentsSafeHandle Handle { get; }

        internal PdfContents(PdfContentsSafeHandle handle)
        {
            Handle = handle;
        }

        static PdfContents()
        {
            RuntimeHelpers.RunClassConstructor(typeof(NativeMethods).TypeHandle);
        }

        public UInt64 GetInstructionsSize()
        {
            UInt32 result = NativeMethods.Contents_GetInstructionsSize(Handle, out UIntPtr data);
            if (result != PdfReturnValues.ERROR_SUCCESS) {
                throw PdfErrors.GetLastErrorException();
            }

            return data.ToUInt64();
        }

        protected override void ReleaseManagedResources()
        {
            Handle.Dispose();
        }

        private static class NativeMethods
        {
            public static ContentsGetInstructionsSizeDelgate Contents_GetInstructionsSize = LibraryInstance.GetFunction<ContentsGetInstructionsSizeDelgate>("Contents_GetInstructionsSize");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 ContentsGetInstructionsSizeDelgate(PdfContentsSafeHandle handle, out UIntPtr data);
        }
    }
}
