using System;

namespace gotchangpdf.net.test
{
    class Program
    {
        static void Main(string[] args)
        {
            Logging.Enable();

            File file = File.Open(args[0]);
            file.Initialize();

            Document doc = Document.OpenExisting(file);
            doc.Save("example.pdf");
        }
    }
}