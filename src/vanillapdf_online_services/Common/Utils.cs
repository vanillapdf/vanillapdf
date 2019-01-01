using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;
using vanillapdf.net;

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

        public static string ValidateDocument(string filename, string hash, string path)
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine($"Filename: {filename}");
            sb.AppendLine($"SHA-256: {hash}");

            using (var file = PdfFile.Open(path)) {

                // Fixed int latest version
                file.Initialize();

                using (var document = PdfDocument.OpenFile(file))
                using (var catalog = document.GetCatalog())
                using (var pageTree = catalog.GetPageTree()) {

                    var isEncrypted = file.IsEncrypted();
                    var version = catalog.GetVersion();
                    var pageCount = pageTree.GetPageCount();

                    string versionString = (version.HasValue ? Convert.ToString(version.Value) : "Not specified");
                    string encryptedString = isEncrypted ? "Yes" : "No";

                    sb.AppendLine($"Version: {versionString}");
                    sb.AppendLine($"Encrypted: {encryptedString}");
                    sb.AppendLine($"Pages: {pageCount}");

                    for (ulong i = 1; i <= pageCount; ++i) {
                        using (var page = pageTree.GetPage(i))
                        using (var annotations = page.GetAnnotations())
                        using (var contents = page.GetContents()) {

                            ulong instructionsSize = 0;
                            ulong annotationsSize = 0;

                            if (contents != null) {
                                instructionsSize = contents.GetInstructionsSize();
                            }

                            if (annotations != null) {
                                instructionsSize = annotations.Size();
                            }

                            sb.AppendLine(Environment.NewLine);
                            sb.AppendLine($"Page: {i}");
                            sb.AppendLine($"Instructions: {instructionsSize}");
                            sb.AppendLine($"Annotations: {annotationsSize}");
                        }
                    }
                }
            }

            return sb.ToString();
        }
    }
}
