using Microsoft.AspNetCore.Mvc.Localization;
using Microsoft.Extensions.Localization;

namespace vanillapdf_online_services.Common
{
    public interface ICustomLocalizer
    {
        LocalizedString this[string name] { get; }
        LocalizedString this[string name, params object[] arguments] { get; }
    }

    public interface ICustomHtmlLocalizer
    {
        LocalizedHtmlString this[string name] { get; }
        LocalizedHtmlString this[string name, params object[] arguments] { get; }
    }
}
