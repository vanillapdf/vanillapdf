using Microsoft.AspNetCore.Http;
using System.ComponentModel.DataAnnotations;

namespace vanillapdf_online_services.Models.ViewModels
{
    public class ValidateFileViewModel
    {
        [Required]
        public IFormFile UploadedFile { get; set; }
    }
}
