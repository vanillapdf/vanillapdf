# Security Policy

## Overview

This document outlines the security support policy and responsible vulnerability disclosure process for the **Vanilla.PDF** project.

## Supported Versions

The following Vanilla.PDF versions currently receive **security fixes and critical patches**:

| Version | Supported |
| ------- | --------- |
| 2.2.x   | ✅        |
| 2.1.x   | ✅        |
| < 2.1   | ❌        |

## Reporting a Vulnerability

If you discover a security vulnerability in Vanilla.PDF, please report it **privately and responsibly**:

- **GitHub (preferred):** Submit a draft security advisory at [github.com/vanillapdf/vanillapdf/security/advisories](https://github.com/vanillapdf/vanillapdf/security/advisories)
- **Email (alternative):** Contact us directly at `security@vanillapdf.com`

> ⚠️ **Do NOT open a public GitHub issue for security vulnerabilities.** Public issues are visible to everyone and may put users at risk before a fix is available.

### What to Include in Your Report

To help us triage and reproduce the issue quickly, please provide:

- Affected version(s) of Vanilla.PDF
- A clear description of the vulnerability and its potential impact
- Steps to reproduce, including any relevant PDF files, code snippets, or proof-of-concept
- The environment (OS, compiler, build configuration) where the issue was observed
- Your assessment of severity, if possible

## Severity Classification

We use the following severity levels to prioritize responses:

| Severity | Description |
| -------- | ----------- |
| **Critical** | Remote code execution, memory corruption exploitable without user interaction |
| **High** | Significant data exposure, privilege escalation, or exploitable crash |
| **Medium** | Limited impact vulnerabilities requiring specific conditions to trigger |
| **Low** | Minor issues with negligible security impact |

## Response Timeline

- **Acknowledgement:** Within 72 hours of receiving the report
- **Initial assessment:** Within 7 days
- **Fix or mitigation:** Within 7–14 days for High/Critical; up to 30 days for Medium/Low, depending on complexity

## CVE Assignment

For confirmed vulnerabilities, we will request a CVE identifier through the appropriate authority. The CVE number will be included in the public advisory once the fix is released.

## Disclosure Policy

- We will notify you when the fix is ready and coordinate a disclosure date
- We aim to publish a GitHub security advisory and patch release simultaneously
- We ask that you refrain from public disclosure until the fix has been released
- If we are unable to resolve the issue within a reasonable timeframe, we will work with you to agree on an appropriate disclosure date

## Reporter Credit

We are grateful to security researchers who help keep Vanilla.PDF safe. With your permission, we will acknowledge your contribution in the published security advisory.

---

_Last updated: February 2026_
