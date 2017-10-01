using System.Security.Cryptography;

namespace LicenseGenerator
{
    public class RSAPKCS1SHA256SignatureDescription : SignatureDescription
    {
        public static void Register()
        {
            CryptoConfig.AddAlgorithm(
                typeof(RSAPKCS1SHA256SignatureDescription),
                "http://www.w3.org/2001/04/xmldsig-more#rsa-sha256");
        }

        public RSAPKCS1SHA256SignatureDescription()
        {
            base.KeyAlgorithm = "System.Security.Cryptography.RSACryptoServiceProvider";
            base.DigestAlgorithm = "System.Security.Cryptography.SHA256CryptoServiceProvider";
            base.FormatterAlgorithm = "System.Security.Cryptography.RSAPKCS1SignatureFormatter";
            base.DeformatterAlgorithm = "System.Security.Cryptography.RSAPKCS1SignatureDeformatter";
        }

        public override AsymmetricSignatureDeformatter CreateDeformatter(AsymmetricAlgorithm key)
        {
            var asymmetricSignatureDeformatter =
                (AsymmetricSignatureDeformatter)CryptoConfig.CreateFromName(DeformatterAlgorithm);
            asymmetricSignatureDeformatter.SetKey(key);
            asymmetricSignatureDeformatter.SetHashAlgorithm("SHA256");
            return asymmetricSignatureDeformatter;
        }

        public override AsymmetricSignatureFormatter CreateFormatter(AsymmetricAlgorithm key)
        {
            var asymmetricSignatureFormatter =
                (AsymmetricSignatureFormatter)CryptoConfig.CreateFromName(FormatterAlgorithm);
            asymmetricSignatureFormatter.SetKey(key);
            asymmetricSignatureFormatter.SetHashAlgorithm("SHA256");
            return asymmetricSignatureFormatter;
        }
    }
}
