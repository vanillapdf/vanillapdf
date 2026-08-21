# Contributing to Vanilla.PDF

Thank you for considering contributing to **Vanilla.PDF**! Your involvement helps improve the project for everyone. Whether you're fixing bugs, improving documentation, or suggesting new features—we welcome your input.

Vanilla.PDF is released under the [Apache License 2.0](LICENSE.txt). Contributions are accepted into this repository on those terms and are distributed as part of the project under that license. Contributors additionally sign a [Contributor License Agreement](CLA.md) — see below.

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

**Security vulnerabilities are different.** If you believe you have found a security issue — a crash on malformed input, a memory-safety problem in the parser, or a flaw in encryption handling — please **do not open a public issue**. Follow the [security policy](SECURITY.md): submit a draft advisory through GitHub, or email [security@vanillapdf.com](mailto:security@vanillapdf.com).

---

## 💡 Feedback & Feature Requests

Missing a key feature? Have a great idea?

We’d love to hear it — send us a note at [info@vanillapdf.com](mailto:info@vanillapdf.com), or open a [GitHub issue](https://github.com/vanillapdf/vanillapdf/issues) so others can weigh in.

---

## 🔁 Submitting Pull Requests

We welcome contributions! Please follow these guidelines:

**🚨 MANDATORY: Branch and PR Workflow**
- **ALL changes must go through a branch and pull request** - this is required by repository permissions
- **NEVER commit directly to `main` or `release/*` branches** - they are protected
- Create a new branch for every change, no matter how small
- Follow naming convention: `feature/description` or `fix/description`

**Pull Request Guidelines:**
- 🧠 **Discuss major changes first**: For anything beyond a bug fix, contact the team before investing time. Be aware that changes to core subsystems (the parser, cross-reference handling, encryption) are often reimplemented by the maintainers rather than merged as-is — open an issue before spending a weekend on one; your report is credited with a `Reported-by:` trailer either way.
- 🌳 **Base your PR on the `main` branch** (or appropriate `release/*` branch for hotfixes)
- 🧹 **Match the existing coding style**
- ✍️ **Sign off your commits** with `git commit -s` (see the DCO section below)
- 📜 **Sign the CLA** (see below) — requested automatically on your first pull request; one signature per version of the agreement
- 🚫 **Submit only your own work**: do not include third-party code in a pull request (see below)
- 🤖 **Disclose AI assistance**: if generative AI tools helped produce your change, say so in the pull request or add an `Assisted-by:` trailer to the affected commits (see Section 6 of the [CLA](CLA.md))
- 🧪 **Ensure tests pass**: Run the test suite before submitting
- 📝 **Use clear commit messages**: conventional commit format is preferred but not enforced

---

## ✅ Developer Certificate of Origin (DCO)

Every commit must carry a `Signed-off-by:` line. That line is not a formality: it certifies the **Developer Certificate of Origin, version 1.1**, reproduced in full below.

Where the certificate refers to "the open source license indicated in the file," read it as the license declared for this repository — the Apache License 2.0, set out in [LICENSE.txt](LICENSE.txt). Vanilla.PDF declares its license once at project level rather than in per-file headers; see [Copyright Notices and Credits](#️-copyright-notices-and-credits) below.

```
Developer Certificate of Origin
Version 1.1

Copyright (C) 2004, 2006 The Linux Foundation and its contributors.

Everyone is permitted to copy and distribute verbatim copies of this
license document, but changing it is not allowed.


Developer's Certificate of Origin 1.1

By making a contribution to this project, I certify that:

(a) The contribution was created in whole or in part by me and I
    have the right to submit it under the open source license
    indicated in the file; or

(b) The contribution is based upon previous work that, to the best
    of my knowledge, is covered under an appropriate open source
    license and I have the right under that license to submit that
    work with modifications, whether created in whole or in part
    by me, under the same open source license (unless I am
    permitted to submit under a different license), as indicated
    in the file; or

(c) The contribution was provided directly to me by some other
    person who certified (a), (b) or (c) and I have not modified
    it.

(d) I understand and agree that this project and the contribution
    are public and that a record of the contribution (including all
    personal information I submit with it, including my sign-off) is
    maintained indefinitely and may be redistributed consistent with
    this project or the open source license(s) involved.
```

Add the sign-off automatically with the `-s` flag:

```bash
git commit -s -m "Fix PDF metadata encoding"
```

This appends a line like:

```
Signed-off-by: Your Name <you@example.com>
```

The name and email must be your real ones and must match your git configuration. If you forget the sign-off, amend the commit (`git commit --amend -s`) or rebase to add it across a range (`git rebase --signoff main`) and force-push the branch.

**Note the lower-case `-s`.** `git commit -S` (upper case) creates a GPG *signature*, which is a different thing. GPG signing of commits is **not required** in this repository — only maintainers' release tags are GPG-signed, as described at the end of this document.

For more information, visit [developercertificate.org](https://developercertificate.org)

---

## 📜 Contributor License Agreement (CLA)

Before your first pull request can be merged, you must sign our [Contributor License Agreement](CLA.md). You sign once per version of the agreement: if the text is ever amended you will be asked to sign the new version, otherwise you are never asked again.

**Why a CLA?** Vanilla.PDF is released under Apache 2.0. The CLA gives the project the rights to license contributions on other terms as well — including commercially — should the project need that in the future. It does not transfer your copyright: you keep full ownership of your contributions and may use them however you like elsewhere. Code already released under Apache 2.0 remains available under Apache 2.0; the CLA does not and cannot change that.

**How to sign:** When you open your first pull request, an automated check will post instructions. You sign by replying with a comment on the pull request:

> I have read the CLA Document and I hereby sign the CLA

The check then turns green automatically (comment `recheck` if it needs a nudge). Your signature is recorded privately — what is recorded, why, and for how long is set out in Section 15 of the [CLA](CLA.md).

**What your signature covers.** A signature applies to every contribution you submit while that version of the agreement is in effect, and also to anything you contributed before signing. It is not per pull request.

**Tell us if your situation changes.** If you change employer, or take on a client or contract that may claim rights in the code you write, obtain that party's permission before your next contribution and let us know — see Section 9 of the [CLA](CLA.md). We may ask you to sign again.

**CLA and commit sign-off serve different purposes.** The CLA is the legal grant covering your contributions. The `Signed-off-by:` trailer (see the DCO section above) is a per-commit assertion, attached to each individual change and recorded in the history itself. The CLA is what permits merging; the sign-off is required on every commit in this repository.

**Individual contributions only.** You sign the CLA as an individual, and you must have your employer's permission if they hold rights in your work. We do not currently offer a corporate contributor agreement — the full conditions are in Section 12 of the [CLA](CLA.md). If you cannot sign, please open an issue describing the change instead; if we implement it, the commit credits you with a `Reported-by:` trailer.

---

## 📦 Third-Party Code and Dependencies

**Do not include third-party code in a pull request.** This applies to whole files, individual functions, and snippets copied from other projects, blog posts, or answer sites. A contribution should be your own work, and the CLA you sign covers only your own work — you cannot grant rights in someone else's code.

If your change needs an external component, **open an issue first**. Adding a dependency is a maintainer decision: it affects the build on every supported platform, the packaging in vcpkg, Conan and Homebrew, and the project's licensing position for years afterwards.

Where a third-party component is accepted, it must be under a **permissive license** — MIT, BSD-2-Clause, BSD-3-Clause, Apache-2.0, ISC, or zlib. Copyleft licenses (GPL, LGPL, AGPL, MPL, EPL, and similar) are **not accepted in any form**, including for optional or test-only code.

Accepted components keep their original copyright and license notices untouched, and their license is recorded in [NOTICE.md](NOTICE.md).

If a contribution deliberately includes work that is not your own, it must be submitted separately and marked as such — see Section 8 of the [CLA](CLA.md).

---

## ©️ Copyright Notices and Credits

Copyright in Vanilla.PDF is tracked at the **project level**, not per file:

- **Do not add copyright or license headers to source files** — new or existing. Licensing is declared once in [LICENSE.txt](LICENSE.txt), and the project-level copyright notice lives in [NOTICE.md](NOTICE.md) — its "contributors" are the individuals recorded in git history. Third-party attributions live in NOTICE.md as well.
- **Do not add per-author copyright lines** (e.g. `// Copyright 2026 Jane Doe`) to files you create or modify. Under the CLA you retain copyright in your contribution automatically — a notice in the file adds no rights and, accumulated over time, makes the codebase look encumbered when it isn't.
- **Exception — third-party code:** never add, remove, or alter copyright or license notices in vendored or third-party code. Preserving those notices is a condition of virtually every license, including permissive ones; stripping them is a license violation, not a cleanup.
- **Credit is carried by git history.** Merged commits record your authorship, and where a change is squashed or reworked, additional authors are preserved with `Co-authored-by:` trailers. If a fix originates from your report or patch but is implemented separately, the commit carries a `Reported-by:` trailer crediting you. To appear in the repository's [contributors listing](https://github.com/vanillapdf/vanillapdf/graphs/contributors), make sure the email address in your commits is registered on your GitHub account.

Pull requests that add per-file copyright notices will be asked to remove them before merge.

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

Each release includes a GitHub build provenance attestation for every artifact, enabling keyless verification via the [GitHub CLI](https://cli.github.com/):

```bash
gh attestation verify vanillapdf.2.3.0.nupkg \
  --repo vanillapdf/vanillapdf
```

---

## 🙏 Thank You!

Every bug report, idea, and pull request makes Vanilla.PDF better. We appreciate your time and contributions!
