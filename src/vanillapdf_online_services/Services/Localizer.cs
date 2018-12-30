using Microsoft.AspNetCore.Mvc.Localization;
using Microsoft.Extensions.Localization;
using vanillapdf_online_services.Common;

namespace vanillapdf_online_services.Services
{
    // These are our custom localizers. We have them because the default localizers (IHtmlLocalizer and
    // IStringLocalizer) return empty string if the target text is in the resource file but is not
    // translated. We want to return the default language (english) text in such a case.
    //
    // Warning: When we create the LocalizedString and LocalizedHtmlString, we pass a copy of the key
    // as a value:
    //     new LocalizedString(name, string.Copy(name))
    //     new LocalizedHtmlString(name, string.Copy(name))
    // This is probably not needed but it is there just in case to prevent strange behavior that happened
    // when we where returning non-copied string in the case of the indexer with arguments. The cshtml
    // template just stopped rendering when it reached the indexer (when the resource was not translated).
    // The strange behaviour stopped when we started replacing the text arguments in the original language:
    //     return new LocalizedHtmlString(name, string.Format(name, arguments))

    public class Localizer : ICustomLocalizer
    {
        private readonly IStringLocalizer<Txt> _Loc;

        public Localizer(IStringLocalizer<Txt> loc)
        {
            _Loc = loc;
        }

        public LocalizedString this[string name]
        {
            get
            {
                var str = _Loc[name];
                if (str.Value == "") {
                    // If not translated, return default language version.
                    return new LocalizedString(name, string.Copy(name));
                }
                return str;
            }
        }

        public LocalizedString this[string name, params object[] arguments]
        {
            get
            {
                var str = _Loc[name, arguments];
                if (str.Value == "") {
                    // If not translated, return default language version.
                    return new LocalizedString(name, string.Format(name, arguments));
                }
                return str;
            }
        }
    }

    public class HtmlLocalizer : ICustomHtmlLocalizer
    {
        private readonly IHtmlLocalizer<Txt> _Loc;

        public HtmlLocalizer(IHtmlLocalizer<Txt> loc)
        {
            _Loc = loc;
        }

        public LocalizedHtmlString this[string name]
        {
            get
            {
                var str = _Loc[name];
                if (str.Value == "") {
                    // If not translated, return default language version.
                    return new LocalizedHtmlString(name, string.Copy(name));
                }
                return str;
            }
        }

        public LocalizedHtmlString this[string name, params object[] arguments]
        {
            get
            {
                var str = _Loc[name, arguments];
                if (str.Value == "") {
                    // If not translated, return default language version.
                    return new LocalizedHtmlString(name, string.Format(name, arguments));
                }
                return str;
            }
        }
    }
}
