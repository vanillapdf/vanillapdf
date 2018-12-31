using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace vanillapdf_online_services.Common
{
    public static class Utils
    {
        public static string CalculateSha256(byte[] data)
        {
            using (MemoryStream stream = new MemoryStream(data)) {
                return CalculateSha256(stream);
            }
        }

        public static string CalculateSha256(Stream data)
        {
            using (var sha256Hash = SHA256.Create()) {
                var hashBytes = sha256Hash.ComputeHash(data);
                return ToHex(hashBytes, true);
            }
        }

        public static string ToHex(byte[] bytes, bool upperCase)
        {
            StringBuilder result = new StringBuilder(bytes.Length * 2);
            for (int i = 0; i < bytes.Length; i++) {
                result.Append(bytes[i].ToString(upperCase ? "X2" : "x2"));
            }

            return result.ToString();
        }
    }
}
