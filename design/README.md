# Design Documents

This folder contains detailed design and implementation documentation for vanillapdf features.

## Purpose

- Capture architectural decisions and rationale
- Document implementation plans for major features
- Provide technical specifications beyond what fits in GitHub issues

## Documents

| Document | Description | Status |
|----------|-------------|--------|
| [signature-verification.md](signature-verification.md) | PKCS#7 signature verification infrastructure | Complete |
| [crl-ocsp-revocation.md](crl-ocsp-revocation.md) | CRL/OCSP certificate revocation checking | Planned |

## Contributing

When adding a new design document:

1. Use kebab-case for filenames (e.g., `feature-name.md`)
2. Include a clear overview section at the top
3. Document design decisions and alternatives considered
4. Update this README with a link to the new document
