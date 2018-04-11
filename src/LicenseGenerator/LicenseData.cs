using Newtonsoft.Json;
using System.Collections.Generic;

namespace LicenseGenerator
{
    class LicenseData
    {
        [JsonProperty("owner")]
        public string Owner { get; set; }

        [JsonProperty("note")]
        public string Note { get; set; }

        [JsonProperty("serial")]
        public string Serial { get; set; }

        [JsonProperty("updates_expiration")]
        public string Expiration { get; set; }

        [JsonProperty("temporary_expiration")]
        public string TemporaryExpiration { get; set; }
    }

    class License
    {
        [JsonProperty("version")]
        public string Version { get; set; }

        [JsonProperty("data")]
        public LicenseData Data { get; set; }

        [JsonProperty("signature")]
        public string Signature { get; set; }

        [JsonProperty("certificates")]
        public List<string> Certificates { get; set; }
    }
}
