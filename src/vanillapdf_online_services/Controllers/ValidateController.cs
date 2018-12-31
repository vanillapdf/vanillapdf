using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.IO;
using System.Text;
using vanillapdf.net;
using vanillapdf_online_services.Common;
using vanillapdf_online_services.Models.ViewModels;

namespace vanillapdf_online_services.Controllers
{
    public class ValidateController : BaseController
    {
        private readonly ILogger _Logger;
        private readonly ICustomLocalizer _Loc;

        public ValidateController(
            ILogger<ValidateController> logger,
            ICustomLocalizer loc)
        {
            _Logger = logger;
            _Loc = loc;
        }

        // GET: Tools
        public ActionResult Index()
        {
            return View();
        }

        // POST: Tools/Validate
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Index(ValidateFileViewModel model, IFormFile UploadedFile)
        {
            if (!ModelState.IsValid) {
                return View(model);
            }

            if (UploadedFile == null || UploadedFile.Length == 0) {
                ModelState.AddModelError("UploadedFile", _Loc["No file was selected"]);
                return View(model);
            }

            string fileHash;
            string downloadPath = Path.Combine("TemporaryDownloads", UploadedFile.FileName);

            using (var uploadStream = UploadedFile.OpenReadStream()) {
                string dirPath = Path.GetDirectoryName(downloadPath);
                Directory.CreateDirectory(dirPath);

                using (var fileStream = new FileStream(downloadPath, FileMode.Create)) {
                    uploadStream.CopyTo(fileStream);

                    fileStream.Position = 0;
                    fileHash = Utils.CalculateSha256(fileStream);
                }
            }

            // TODO: check db

            string hashPath = Path.Combine("FileDownloads", fileHash);
            if (!System.IO.File.Exists(hashPath)) {
                string dirPath = Path.GetDirectoryName(hashPath);
                Directory.CreateDirectory(dirPath);

                System.IO.File.Move(downloadPath, hashPath);
            }

            if (System.IO.File.Exists(downloadPath)) {
                System.IO.File.Delete(downloadPath);
            }

            StringBuilder sb = new StringBuilder();
            sb.AppendLine($"Filename: {UploadedFile.FileName}");
            sb.AppendLine($"SHA-256: {fileHash}");

            using (var file = PdfFile.Open(hashPath)) {
                file.Initialize();

                using (var document = PdfDocument.OpenFile(file)) {
                    var catalog = document.GetCatalog();

                    var version = catalog.GetVersion();
                    var pageTree = catalog.GetPageTree();

                    int pageCount = pageTree.GetPageCount();

                    string versionString = (version.HasValue ? Convert.ToString(version.Value) : "Not specified");

                    sb.AppendLine($"Version: {versionString}");
                    sb.AppendLine($"Pages: {pageCount}");
                }
            }

            ViewData["ValidatedFile"] = sb.ToString();

            SuccessMessage("File was successfully validated");
            return View();
        }
    }
}
