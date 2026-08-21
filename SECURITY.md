# Security Policy

## Overview

This document outlines the security support policy and responsible vulnerability disclosure process for the **Vanilla.PDF** project.

## Supported Versions

Security fixes are provided for the **two most recent minor releases**. Older versions are not patched; please upgrade before reporting an issue against them.

| Version | Supported |
| ------- | --------- |
| 2.3.x   | ✅        |
| 2.2.x   | ✅        |
| < 2.2   | ❌        |

## Reporting a Vulnerability

If you discover a security vulnerability in Vanilla.PDF, please report it **privately and responsibly**:

- **GitHub (preferred):** Submit a draft security advisory at [github.com/vanillapdf/vanillapdf/security/advisories](https://github.com/vanillapdf/vanillapdf/security/advisories)
- **Email (alternative):** Contact us directly at `security@vanillapdf.com`. If you wish to encrypt your report, maintainer public keys are available on the [GitHub profile](https://github.com/vanillapdf) and via public keyservers.

> ⚠️ **Do NOT open a public GitHub issue for security vulnerabilities.** Public issues are visible to everyone and may put users at risk before a fix is available.

### What to Include in Your Report

To help us triage and reproduce the issue quickly, please provide:

- Affected version(s) of Vanilla.PDF
- A clear description of the vulnerability and its potential impact
- Steps to reproduce, including any relevant PDF files, code snippets, or proof-of-concept
- The environment (OS, compiler, build configuration) where the issue was observed
- Your assessment of severity, if possible

**Reports from fuzzing** are welcome, but a bare crash is not a report. Please attach the minimized input file, state the sanitizer and build configuration used, and confirm the crash reproduces on a current supported release. Reports consisting only of an automated tool's output, without a reproducer or any analysis of impact, may be closed without detailed investigation.

## Scope

**In scope** — the Vanilla.PDF library itself: memory-safety and undefined-behaviour issues in the parser, cross-reference handling, filters and decoders; flaws in encryption and permission handling; and any issue allowing code execution, data disclosure, or a denial of service that a caller cannot reasonably prevent.

**Out of scope**

- Vulnerabilities in third-party dependencies (zlib, libjpeg-turbo, OpenJPEG, OpenSSL, and others). Report these to the upstream project. If an upstream issue is exploitable *through* Vanilla.PDF in a way the upstream fix does not address, we want to hear about it.
- Resource exhaustion from deliberately adversarial input where the caller controls the limits — for example, very deep object nesting or extremely large images. Callers are expected to impose their own limits on untrusted documents.
- Issues that require an attacker to already control the host, the build environment, or the calling application.
- Missing hardening flags or defence-in-depth suggestions with no demonstrated impact. These are welcome, but as regular issues rather than advisories.

## Severity Classification

We use the following severity levels to prioritize responses:

| Severity | Description |
| -------- | ----------- |
| **Critical** | Remote code execution, memory corruption exploitable without user interaction |
| **High** | Significant data exposure, privilege escalation, or exploitable crash |
| **Medium** | Limited impact vulnerabilities requiring specific conditions to trigger |
| **Low** | Minor issues with negligible security impact |

## Response Timeline

Vanilla.PDF is maintained by a small team. The following are the targets we work to, not contractual guarantees:

- **Acknowledgement:** within 72 hours of receiving the report
- **Initial assessment:** within 7 days
- **Fix or mitigation:** 7–14 days for High and Critical issues; up to 30 days for Medium and Low, depending on complexity

Where a fix will take longer than these targets — because the issue is structural, or reproduction is difficult — we will say so and keep you informed rather than going quiet.

## CVE Assignment

For confirmed vulnerabilities we request a CVE identifier through GitHub, which acts as a CVE Numbering Authority for repositories it hosts. The identifier is included in the published advisory once the fix is released.

## Disclosure Policy

- We will notify you when the fix is ready and coordinate a disclosure date
- We aim to publish a GitHub security advisory and patch release simultaneously
- We ask that you refrain from public disclosure until the fix has been released
- If we are unable to resolve the issue within a reasonable timeframe, we will work with you to agree on an appropriate disclosure date

## Safe Harbour

If you make a good-faith effort to comply with this policy while researching and reporting a vulnerability, we will not pursue or support legal action against you in connection with that research. Good faith means: testing only against your own installations and your own documents, avoiding privacy violations and disruption to others, not exfiltrating or retaining data that is not yours, and giving us a reasonable opportunity to fix the issue before disclosing it.

Vanilla.PDF does not operate a bug bounty programme and does not offer monetary rewards for vulnerability reports.

## Reporter Credit

We are grateful to security researchers who help keep Vanilla.PDF safe. With your permission, we will acknowledge your contribution in the published security advisory, and the commit containing the fix carries a `Reported-by:` trailer crediting you.

If you submit a fix yourself rather than only a report, it is a contribution like any other: it requires a `Signed-off-by:` line and a signed Contributor License Agreement. See [CONTRIBUTING.md](CONTRIBUTING.md). Please still report the issue privately first — do not open a public pull request describing an unfixed vulnerability.

---

_Last updated: August 2026_
