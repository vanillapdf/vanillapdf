using Microsoft.AspNetCore.Mvc;

namespace vanillapdf_online_services.Controllers
{
    public class BaseController : Controller
    {
        protected void SuccessMessage(string message)
        {
            TempData["SuccessMessage"] = message;
        }

        protected void ErrorMessage(string message)
        {
            TempData["ErrorMessage"] = message;
        }
    }
}
