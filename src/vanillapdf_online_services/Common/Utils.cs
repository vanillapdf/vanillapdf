using System;
using System.IO;
using System.Security.Cryptography;

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
                return Convert.ToBase64String(hashBytes);
            }
        }
    }
}
