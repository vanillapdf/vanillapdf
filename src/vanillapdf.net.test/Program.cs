using System;

namespace vanillapdf.net.test
{
    class Program
    {
        static void Main(string[] args)
        {
            Logging.Enable();

            using (Document document = Document.OpenNew(args[0])) {
                Catalog catalog = document.GetCatalog();
                PageTree tree = catalog.GetPageTree();
                int count = tree.GetPageCount();

                for (int i = 1; i < args.Length; ++i) {
                    using (Document other_document = Document.OpenNew(args[i])) {
                        document.AppendDocument(other_document);
                    }
                }

                document.Save("example.pdf");
            }

            try {
                using (Document document = Document.OpenNew("aa")) {
                }
                }catch (Exception ex) {

            }

            uint test = Errors.GetLastError();
            string message = Errors.GetLastErrorMessage();
        }
    }
}