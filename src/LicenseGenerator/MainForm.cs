using System;
using System.IO;
using System.Reflection;
using System.Security.Cryptography.X509Certificates;
using System.Security.Cryptography.Xml;
using System.Windows.Forms;
using System.Xml;
using System.Linq;
using System.Collections.Generic;
using Newtonsoft.Json;
using System.Security.Cryptography;
using System.Text;
using System.Globalization;

namespace LicenseGenerator
{
    public partial class MainForm : Form
    {
        const int LICENSE_VERSION_MAJOR = 1;
        const int LICENSE_VERSION_MINOR = 0;
        const string MASTER_CERTIFICATE_NAME = "Vanilla.PDF_LicenseGenerator.master.crt";
        const string CERTIFICATE_THUMBPRINT = "F3D7484309E47CE7733D0C49D20E22A3454B5F20";
        //const string CERTIFICATE_THUMBPRINT = "51CA45BD73DB6DB35D41B63D56BB81A1FAEED763";

        const string DIGEST_METHOD_URI = "http://www.w3.org/2001/04/xmlenc#sha256";
        const string SIGNATURE_METHOD_URI = "http://www.w3.org/2001/04/xmldsig-more#rsa-sha256";

        const string HASH_ALGORITHM = "SHA256";

        public MainForm()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //GenerateXMLLicense();
            GenerateJSONLicense();
        }

        private void GenerateJSONLicense()
        {
            string owner = OwnerText.Text;
            string note = NoteText.Text;
            string serial = SerialText.Text;
            int years = Convert.ToInt32(ExpirationYears.Value);
            string validUntil = DateTimeOffset.Now.AddYears(years).ToString("yyyy-MM-dd HH:mm:ssZ", CultureInfo.InvariantCulture);

            if (String.IsNullOrEmpty(owner)) {
                MessageBox.Show("Owner cannot be empty");
                return;
            }

            if (String.IsNullOrEmpty(serial)) {
                MessageBox.Show("Serial cannot be empty");
                return;
            }

            string licenseFile = $"{owner}.lic";

            using (X509Store store = new X509Store(StoreName.My, StoreLocation.CurrentUser)) {
                store.Open(OpenFlags.ReadOnly);

                var certificates = store.Certificates.Find(X509FindType.FindByThumbprint, CERTIFICATE_THUMBPRINT, false);
                if (certificates.Count != 1 || !certificates[0].HasPrivateKey) {
                    MessageBox.Show("No certificate was selected");
                    return;
                }

                LicenseData data = new LicenseData();
                data.Owner = owner;
                data.Note = note;
                data.Serial = serial;
                data.Expiration = validUntil;

                StringBuilder signedDataBuilder = new StringBuilder();
                signedDataBuilder.Append(owner);
                signedDataBuilder.Append(note);
                signedDataBuilder.Append(serial);
                signedDataBuilder.Append(validUntil);

                string signedDataString = signedDataBuilder.ToString();
                byte[] signedData = Encoding.UTF8.GetBytes(signedDataString);

                RSACryptoServiceProvider csp = certificates[0].PrivateKey as RSACryptoServiceProvider;
                byte[] signature = csp.SignData(signedData, CryptoConfig.MapNameToOID(HASH_ALGORITHM));
                bool verified = csp.VerifyData(signedData, CryptoConfig.MapNameToOID(HASH_ALGORITHM), signature);

                if (!verified) {
                    MessageBox.Show("Could not verify signature");
                    return;
                }

                License license = new License();
                license.Version = $"{LICENSE_VERSION_MAJOR}.{LICENSE_VERSION_MINOR}";
                license.Data = data;
                license.Signature = Convert.ToBase64String(signature);

                X509Chain chain = new X509Chain();
                chain.Build(certificates[0]);

                license.Certificates = new List<string>();
                foreach (X509ChainElement el in chain.ChainElements) {
                    byte[] chainElementBytes = el.Certificate.Export(X509ContentType.Cert);
                    string chainElement = Convert.ToBase64String(chainElementBytes, Base64FormattingOptions.InsertLineBreaks);

                    StringBuilder builder = new StringBuilder();

                    builder.AppendLine("-----BEGIN CERTIFICATE-----");
                    builder.AppendLine(chainElement);
                    builder.AppendLine("-----END CERTIFICATE-----");

                    license.Certificates.Add(builder.ToString());
                }

                string licenseString = JsonConvert.SerializeObject(license, Newtonsoft.Json.Formatting.Indented);
                byte[] licenseBytes = Encoding.UTF8.GetBytes(licenseString);

                File.WriteAllBytes(licenseFile, licenseBytes);
                MessageBox.Show("License was successfully generated");
            }
        }

        private void VerifyJSONLicense()
        {

        }

        private void GenerateXMLLicense()
        {
            string owner = OwnerText.Text;
            string serial = SerialText.Text;
            int years = Convert.ToInt32(ExpirationYears.Value);
            string validUntil = DateTimeOffset.Now.AddYears(years).ToString("R");

            if (String.IsNullOrEmpty(owner)) {
                MessageBox.Show("Owner cannot be empty");
                return;
            }

            if (String.IsNullOrEmpty(serial)) {
                MessageBox.Show("Serial cannot be empty");
                return;
            }

            string licenseFile = $"{owner}.lic";
            //using (XmlWriter xmlWriter = XmlWriter.Create(licenseFile)) {
            //    xmlWriter.WriteStartDocument();
            //    xmlWriter.WriteStartElement("License");
            //    xmlWriter.WriteStartElement("Data");

            //    xmlWriter.WriteElementString("LicenseOwner", owner);
            //    xmlWriter.WriteElementString("ValidUntil", validUntil);

            //    xmlWriter.WriteEndElement(); // Data

            //    xmlWriter.WriteStartElement("Signature");
            //    xmlWriter.WriteEndElement();

            //    xmlWriter.WriteEndDocument();
            //    xmlWriter.Close();
            //}

            XmlDocument xmlDoc = new XmlDocument();

            XmlElement ownerNode = xmlDoc.CreateElement("LicenseOwner");
            ownerNode.InnerText = owner;

            XmlElement serialNode = xmlDoc.CreateElement("SerialNumber");
            serialNode.InnerText = serial;

            XmlElement expirationNode = xmlDoc.CreateElement("Expiration");
            expirationNode.InnerText = validUntil;

            XmlElement versionNode = xmlDoc.CreateElement("LicenseVersion");
            versionNode.InnerText = $"{LICENSE_VERSION_MAJOR}.{LICENSE_VERSION_MINOR}";

            XmlElement dataNode = xmlDoc.CreateElement("Data");
            dataNode.AppendChild(ownerNode);
            dataNode.AppendChild(serialNode);
            dataNode.AppendChild(expirationNode);
            dataNode.AppendChild(versionNode);

            XmlElement rootNode = xmlDoc.CreateElement("License");
            rootNode.AppendChild(dataNode);
            xmlDoc.AppendChild(rootNode);

            using (X509Store store = new X509Store(StoreName.My, StoreLocation.CurrentUser)) {
                store.Open(OpenFlags.ReadOnly);

                var certificates = store.Certificates.Find(X509FindType.FindByThumbprint, CERTIFICATE_THUMBPRINT, false);
                if (certificates.Count != 1 || !certificates[0].HasPrivateKey) {
                    MessageBox.Show("No certificate was selected");
                    return;
                }

                Reference reference = new Reference();
                reference.Uri = "";

                reference.AddTransform(new XmlDsigEnvelopedSignatureTransform());

                //reference.DigestMethod = DIGEST_METHOD_URI;

                // Create a SignedXml object.
                SignedXml signedXml = new SignedXml(dataNode);
                signedXml.SigningKey = certificates[0].PrivateKey;
                //signedXml.SignedInfo.SignatureMethod = SIGNATURE_METHOD_URI;
                signedXml.AddReference(reference);

                // Create a new KeyInfo object.
                KeyInfo keyInfo = new KeyInfo();

                // Load the certificate into a KeyInfoX509Data object
                // and add it to the KeyInfo object.
                keyInfo.AddClause(new KeyInfoX509Data(certificates[0], X509IncludeOption.WholeChain));

                // Add the KeyInfo object to the SignedXml object.
                signedXml.KeyInfo = keyInfo;

                // Compute the signature for whole document
                signedXml.ComputeSignature();

                XmlElement xmlDigitalSignature = signedXml.GetXml();
                //xmlDoc.DocumentElement.AppendChild(xmlDoc.ImportNode(xmlDigitalSignature, true));
                rootNode.AppendChild(xmlDigitalSignature);
            }

            bool valid = VerifyXMLLicense(xmlDoc);
            if (!valid) {
                MessageBox.Show("License file has invalid signature");
                return;
            }

            xmlDoc.Save(licenseFile);
            MessageBox.Show("License was successfully generated");
        }

        private bool VerifyXMLLicense(XmlDocument xmlDocument)
        {
            // Create a new SignedXml object and pass it
            // the XML document class.
            SignedXml signedXml = new SignedXml(xmlDocument);

            // Find the "Signature" node and create a new
            // XmlNodeList object.
            XmlNodeList nodeList = xmlDocument.GetElementsByTagName("Signature");

            // Load the signature node.
            signedXml.LoadXml((XmlElement)nodeList[0]);

            if (signedXml.KeyInfo == null || signedXml.KeyInfo.Count == 0) {
                return false;
            }

            List<X509Certificate2> certificates = new List<X509Certificate2>();
            foreach (KeyInfoClause data in signedXml.KeyInfo) {
                if (data is KeyInfoX509Data) {
                    KeyInfoX509Data converted = data as KeyInfoX509Data;
                    foreach (X509Certificate2 cert in converted.Certificates) {
                        certificates.Add(cert);
                    }
                }
            }

            X509Chain chain = new X509Chain();
            chain.ChainPolicy.RevocationMode = X509RevocationMode.NoCheck;
            chain.ChainPolicy.VerificationFlags = X509VerificationFlags.AllowUnknownCertificateAuthority;
            chain.ChainPolicy.ExtraStore.AddRange(certificates.Skip(1).ToArray());
            bool built = chain.Build(certificates[0]);

            if (!built) {
                return false;
            }

            X509Certificate2 lastCertificate = chain.ChainElements[chain.ChainElements.Count - 1].Certificate;
            X509Certificate2 master = GetMasterCertificate();
            bool masterCertificateValid = lastCertificate.RawData.SequenceEqual(master.RawData);
            if (!masterCertificateValid) {
                return false;
            }

            // Check the signature and return the result.
            return signedXml.CheckSignature();
        }

        private X509Certificate2 GetMasterCertificate()
        {
            var assembly = Assembly.GetExecutingAssembly();
            using (MemoryStream memStream = new MemoryStream()) {
                using (Stream stream = assembly.GetManifestResourceStream(MASTER_CERTIFICATE_NAME)) {
                    stream.CopyTo(memStream);
                }

                return new X509Certificate2(memStream.ToArray());
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            RSAPKCS1SHA256SignatureDescription.Register();

            GenerateSerial();
            UpdateLicenseVersion();
        }

        private void GenerateSerial()
        {
            SerialText.Text = Guid.NewGuid().ToString().ToUpperInvariant();
        }

        private void UpdateLicenseVersion()
        {
            VersionText.Text = $"{LICENSE_VERSION_MAJOR}.{LICENSE_VERSION_MINOR}";
        }
    }
}
