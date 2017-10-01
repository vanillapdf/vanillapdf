using Newtonsoft.Json;
using System.Collections.Generic;

namespace LicenseGenerator
{
    class LicenseData
    {
        [JsonProperty("owner")]
        public string Owner { get; set; }

        [JsonProperty("serial")]
        public string Serial { get; set; }

        [JsonProperty("expiration")]
        public string Expiration { get; set; }
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
