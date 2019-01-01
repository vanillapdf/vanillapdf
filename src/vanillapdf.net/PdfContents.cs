using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using vanillapdf.net.Utils;

namespace vanillapdf.net
{
    public class PdfContents : PdfUnknown
    {
        internal PdfContents(IntPtr handle) : base(handle)
        {
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

        protected override UInt32 Release(IntPtr data)
        {
            return NativeMethods.Contents_Release(data);
        }

        private static class NativeMethods
        {
            public static ContentsGetInstructionsSizeDelgate Contents_GetInstructionsSize = LibraryInstance.GetFunction<ContentsGetInstructionsSizeDelgate>("Contents_GetInstructionsSize");
            public static ContentsReleaseDelgate Contents_Release = LibraryInstance.GetFunction<ContentsReleaseDelgate>("Contents_Release");

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 ContentsGetInstructionsSizeDelgate(IntPtr handle, out UIntPtr data);

            [UnmanagedFunctionPointer(MiscUtils.LibraryCallingConvention)]
            public delegate UInt32 ContentsReleaseDelgate(IntPtr handle);
        }
    }
}
