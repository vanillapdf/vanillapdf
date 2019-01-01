using System;
using System.Runtime.InteropServices;

namespace vanillapdf.net.Utils
{
    class EnumMarshaller<T> : ICustomMarshaler where T : struct, IConvertible
    {
        public void CleanUpManagedData(object ManagedObj)
        {
            throw new NotImplementedException();
        }

        public void CleanUpNativeData(IntPtr pNativeData)
        {
            throw new NotImplementedException();
        }

        public int GetNativeDataSize()
        {
            return IntPtr.Size;
        }

        public IntPtr MarshalManagedToNative(object ManagedObj)
        {
            throw new NotImplementedException();
        }

        public object MarshalNativeToManaged(IntPtr pNativeData)
        {
            var rawValue = Marshal.ReadIntPtr(pNativeData).ToInt64();
            return EnumUtil<T>.CheckedCast(rawValue);
        }
    }
}
