# Contributing to Vanilla.PDF

Thank you for considering contributing to **Vanilla.PDF**! Your involvement helps improve the project for everyone. Whether you're fixing bugs, improving documentation, or suggesting new features—we welcome your input.

Vanilla.PDF is released under the [Apache License 2.0](LICENSE.txt). Contributions are accepted into this repository on those terms and are distributed as part of the project under that license. The license is declared once at project level rather than in per-file headers — see [Copyright Notices and Credits](#️-copyright-notices-and-credits) below.

There is one requirement for contributing code: sign the [Contributor License Agreement](CLA.md). It is requested automatically on your first pull request and takes one comment.

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
- 📜 **Sign the CLA** (see below) — requested automatically on your first pull request; one signature per version of the agreement
- 🚫 **Submit only your own work**: do not include third-party code in a pull request (see below)
- 🤖 **Disclose AI assistance**: if generative AI tools helped produce your change, say so in the pull request or add an `Assisted-by:` trailer to the affected commits (see Section 6 of the [CLA](CLA.md))
- 🧪 **Ensure tests pass**: Run the test suite before submitting
- 📝 **Use clear commit messages**: conventional commit format is preferred but not enforced

---

## 📜 Contributor License Agreement (CLA)

Before your first pull request can be merged, you must sign our [Contributor License Agreement](CLA.md). You sign once per version of the agreement: if the text is ever amended you will be asked to sign the new version, and you may be asked to sign again if your employment situation changes. Otherwise you are not asked again.

**Why a CLA?** Vanilla.PDF is released under Apache 2.0. The CLA gives the project the rights to license contributions on other terms as well — including commercially — should the project need that in the future. It does not transfer your copyright: you keep full ownership of your contributions and may use them however you like elsewhere. Code already released under Apache 2.0 remains available under Apache 2.0; the CLA does not and cannot change that.

**How to sign:** When you open your first pull request, an automated check will post instructions. You sign by replying with a comment on the pull request:

> I have read the CLA Document and I hereby sign the CLA

The check then turns green automatically (comment `recheck` if it needs a nudge). The check identifies you by the email address on your commits, so make sure it is linked to your GitHub account — otherwise the bot cannot match your signature to your commits and reports you as an unknown committer. Your signature is recorded privately — what is recorded, why, and for how long is set out in Section 15 of the [CLA](CLA.md).

**What your signature covers.** A signature applies to every contribution you submit while that version of the agreement is in effect, and also to anything you contributed before signing. It is not per pull request.

**Tell us if your situation changes.** If you change employer, or take on a client or contract that may claim rights in the code you write, obtain that party's permission before your next contribution and let us know — see Section 9 of the [CLA](CLA.md). We may ask you to sign again.

**One agreement, not a per-commit ritual.** Some projects additionally require a `Signed-off-by:` trailer on every commit under the Developer Certificate of Origin. Vanilla.PDF does not: the CLA covers the same ground and more, and every signature is recorded with the exact text it was given against. You are welcome to sign off your commits out of habit — `git commit -s` does no harm — but nothing here requires it.

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

## 🔐 Release Verification

Every release artifact carries a GitHub build provenance attestation, which records the repository, workflow, and commit it was built from. Verification is keyless — there is no public key to fetch, trust, or keep current — using the [GitHub CLI](https://cli.github.com/):

```bash
gh attestation verify <downloaded-artifact> \
  --repo vanillapdf/vanillapdf
```

Any published release artifact works in place of `<downloaded-artifact>` — package, archive, or binary.

A successful result means the artifact was produced by this repository's release workflow from the commit named in the attestation, and has not been altered since. That is a stronger statement than a signature alone: it binds the artifact to the build that produced it, not merely to whoever held a key.

Releases are not GPG-signed. Signing tags from an automated release pipeline would require a long-lived private key held in CI secrets, and a signature made by such a key attests to nothing more than that the release workflow ran — which the provenance attestation already establishes, without a key to protect, rotate, or revoke.

If your organization has a policy requiring signed tags, open an issue and tell us about it — it is a decision we are willing to revisit, but one that needs a proper key-custody and revocation story rather than an assertion in a document.

---

## 🙏 Thank You!

Every bug report, idea, and pull request makes Vanilla.PDF better. We appreciate your time and contributions!
