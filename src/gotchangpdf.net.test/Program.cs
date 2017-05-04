using System;

namespace gotchangpdf.net.test
{
    class Program
    {
        static void Main(string[] args)
        {
            Logging.Enable();

            using (Document document = Document.OpenNew(args[0])) {

                for (int i = 1; i < args.Length; ++i) {
                    using (Document other_document = Document.OpenNew(args[i])) {
                        document.AppendDocument(other_document);
                    }
                }

                document.Save("example.pdf");
            }
        }
    }
}