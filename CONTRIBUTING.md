# Contributing to Vanilla.PDF

Thank you for considering contributing to **Vanilla.PDF**! Your involvement helps improve the project for everyone. Whether you're fixing bugs, improving documentation, or suggesting new features—we welcome your input.

---

## 🐞 Reporting Bugs

If you encounter a bug or unexpected behavior, we'd appreciate your report. Please follow these steps:

1. Visit our [GitHub Issues](https://github.com/vanillapdf/vanillapdf/issues) to report bugs or check for existing reports.
2. If you're a customer with a paid support plan, please use the dedicated support section.
3. Include the following in your report:
   - Version of Vanilla.PDF
   - Operating system and version
   - Clear description of:
     - What’s happening
     - What you expected to happen
   - Relevant code snippet
   - Sample PDF (anonymized if needed using our document anonymizer)

Still unsure? Reach out to [info@vanillapdf.com](mailto:info@vanillapdf.com)

---

## 💡 Feedback & Feature Requests

Missing a key feature? Have a great idea?

We’d love to hear it — send us a note at [jzikmund@vanillapdf.com](mailto:jzikmund@vanillapdf.com)

---

## 🔁 Submitting Pull Requests

We welcome contributions! Please follow these guidelines:

**🚨 MANDATORY: Branch and PR Workflow**
- **ALL changes must go through a branch and pull request** - this is required by repository permissions
- **NEVER commit directly to `main` or `release/*` branches** - they are protected
- Create a new branch for every change, no matter how small
- Follow naming convention: `feature/description` or `fix/description`

**Pull Request Guidelines:**
- 🧠 **Discuss major changes first**: For anything beyond a bug fix, contact the team before investing time.
- 🌳 **Base your PR on the `main` branch** (or appropriate `release/*` branch for hotfixes)
- 🧹 **Match the existing coding style**
- ✅ **Sign your commits** (see below)
- 🧪 **Ensure tests pass**: Run the test suite before submitting
- 📝 **Use clear commit messages**: Follow conventional commit format when possible

---

## ✅ Developer Certificate of Origin (DCO)

By submitting a contribution, you certify the following:

> "I certify that I have the right to submit this code under the open source license indicated in this repository and that I am doing so in good faith."

All commits must be signed off with a `Signed-off-by:` line. You can do this automatically by committing with the `-s` flag:

```bash
git commit -s -m "Fix PDF metadata encoding"
```

This will append a line like:

```
Signed-off-by: Your Name <you@example.com>
```

For more information, visit [developercertificate.org](https://developercertificate.org)

---

## 🔐 Release Tag Signing

All release tags must be GPG-signed by a maintainer. This allows users to cryptographically verify that a release was created by a trusted maintainer and has not been tampered with.

**Creating a signed tag:**

```bash
git tag -s v2.3.0 -m "Release v2.3.0"
git push origin v2.3.0
```

**Verifying a signed tag:**

```bash
git tag -v v2.3.0
```

This will show the GPG signature details and the signer's key. Maintainer public keys are available in the [GitHub profile](https://github.com/vanillapdf) and via public keyservers.

**Release artifact verification:**

Each release also includes a Sigstore/cosign bundle (`.bundle` file) for every artifact, enabling keyless verification without GPG:

```bash
# Install cosign: https://docs.sigstore.dev/cosign/system_config/installation/
cosign verify-blob \
  --bundle vanillapdf-v2.3.0.tar.gz.bundle \
  vanillapdf-v2.3.0.tar.gz
```

---

## 🙏 Thank You!

Every bug report, idea, and pull request makes Vanilla.PDF better. We appreciate your time and contributions!