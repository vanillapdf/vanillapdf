using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace vanillapdf_online_services.Common
{
    /// <summary>
    ///     This is a dummy class which we use as a common type for localization.
    ///     We pass this class as a type parameter to each IStringLocalizer (and similar services)
    ///     so that they all search for localization strings in the same resources file:
    ///         Resources/Txt.<lang>.resx
    /// </summary>
    public class Txt
    {
    }

    /// <summary>
    ///     A dummy localization class for data annotation strings, such as:
    ///         [Required(ErrorMessage = "The Name field is required")]  
    ///         [Display(Name = "Name")]
    ///     The "The Name field is required" and "Name" will be used as keys when searching
    ///     the in the following file:
    ///         Resources/DataAnnotationTxt.<lang>.resx
    ///     See ConfigureServices() in Setup.cs for the code that sets this up.
    /// </summary>
    public class DataAnnotationTxt
    {
    }
}
