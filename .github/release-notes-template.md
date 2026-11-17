<!--
Release notes template for Vanilla.PDF. See "Release Notes" in RELEASE-GUIDE.md.

How to use it:
1. release.yml creates the draft release with GitHub's generated list of pull
   requests ("What's Changed") and a "Full Changelog" link.
2. Copy this template, fill in the sections, and move the generated list into
   the <details> block at the end.
3. Delete every section that has no entries, and delete this comment.
4. Paste the result into the draft release body before approving the
   production gate.

Rules:
- One line per change, starting with a verb in the imperative mood, like the
  pull request titles ("Add ...", "Fix ...", "Make ..."). Drop the
  conventional-commit prefix (feat:, fix:, ci:).
- End each line with the pull request numbers in parentheses. Related pull
  requests share one line: (#460, #462).
- Write for library users: what they can now do or what behaves differently,
  not how the code changed. Leave out changes nobody outside the project sees
  (internal refactoring, CI-only fixes) unless they explain a user-visible
  change.
- Put API and symbol names in backticks.
- Keep the section order and the emoji below, so releases look alike.
- No title heading: GitHub shows the release name right above the notes.
-->
<!-- Keep the line that matches the tag; delete the others. Stable releases have none. -->
> **Alpha.** This is a pre-release of X.Y.Z, published for early testing. The API added in this release may still change before X.Y.Z is final. Please report problems in [issues](https://github.com/vanillapdf/vanillapdf/issues).

> **Beta.** This is a pre-release of X.Y.Z. The feature set is complete; the API may still change in response to feedback. Please report problems in [issues](https://github.com/vanillapdf/vanillapdf/issues).

> **Release candidate.** This is a pre-release of X.Y.Z for validation ahead of the stable release. The API is considered final for X.Y.Z barring critical issues found during the RC.

<!-- Two or three sentences: what this release is about, for someone deciding whether to upgrade. -->
This release ...

## ⚠️ Breaking changes

<!-- Anything that can break code, builds or deployments that worked with the previous release:
     removed or renamed API, changed behavior, raised minimum platform versions, changed package
     layout. Say what to do instead. -->
- 

## ✨ Highlights

<!-- The three to five changes worth reading about even if nothing else is. Bold lead-in, then a short description. -->
- **Feature name** — what it lets users do (#000)

## 📝 Interactive forms

- 

## 🔏 Digital signatures

- 

## 🔒 Encryption

- 

## 📄 Pages & content

- 

## 🐛 Bug fixes & robustness

- 

## 🧵 Thread safety

- 

## 🛠️ Build, CI & platforms

<!-- Platform support changes (minimum OS, toolchains the binaries are built with) come first. -->
- 

## 📚 Documentation

- 

## 📦 Dependencies

<!-- One line: the vcpkg baseline and anything users must install or upgrade. -->
- Update vcpkg to YYYY.MM.DD (from YYYY.MM.DD) and routine GitHub Actions group bumps

## ⚠️ Known limitations

<!-- Problems known at release time, with the workaround. -->
- 

<details>
<summary>All NN pull requests</summary>

<!-- The generated "What's Changed" list from the draft, unchanged. -->

</details>

<!-- From the draft. Compare against the previous stable release (vX.Y.Z...vX.Y+1.0-alpha.1),
     or against the previous pre-release of the same version for later pre-releases. -->
**Full Changelog**: https://github.com/vanillapdf/vanillapdf/compare/vPREVIOUS...vX.Y.Z
