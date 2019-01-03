using System;
using vanillapdf.net.Utils;

namespace vanillapdf.net.test
{
    class Program
    {
        static void Main(string[] args)
        {
            MiscUtils.InitializeClasses();

             PdfLogging.Enable();

            var severity = PdfLogging.GetSeverity();

            using (PdfFile file = PdfFile.Open("aa")) {
                file.Initialize();
            }

            using (PdfDocument document = PdfDocument.Open("aa")) {
                PdfCatalog catalog = document.GetCatalog();
                PdfPageTree tree = catalog.GetPageTree();
                var count = tree.GetPageCount();
            }

            uint test = PdfErrors.GetLastError();
            string message = PdfErrors.GetLastErrorMessage();
        }
    }
}