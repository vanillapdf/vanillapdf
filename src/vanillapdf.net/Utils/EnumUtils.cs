using System;

namespace vanillapdf.net.Utils
{
    public static class EnumUtil<T>
        where T : struct, IConvertible
    {
        static EnumUtil()
        {
            if (!typeof(T).IsEnum) {
                throw new Exception(typeof(T).FullName + " is not an enum type.");
            }
        }

        public static T CheckedCast(object enumValue)
        {
            if (!Enum.IsDefined(typeof(T), enumValue))
                throw new InvalidCastException(enumValue + " is not a defined value for enum type " +
                                               typeof(T).FullName);

            return (T)enumValue;
        }

        public static bool IsDefined(T enumValue)
        {
            return Enum.IsDefined(typeof(T), enumValue);
        }

        public static int GetIntegralValue(T enumvalue)
        {
            var underlyingType = Enum.GetUnderlyingType(typeof(LoggingSeverity));
            return (int)Convert.ChangeType(enumvalue, underlyingType);
        }
    }

    public static class EnumExtensions
    {
        public static bool IsDefined<T>(this T enumValue)
            where T : struct, IConvertible
        {
            return EnumUtil<T>.IsDefined(enumValue);
        }

        public static int GetIntegralValue<T>(this T enumValue)
            where T : struct, IConvertible
        {
            return EnumUtil<T>.GetIntegralValue(enumValue);
        }
    }
}
