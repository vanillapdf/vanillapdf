#!/usr/bin/env python3
"""PDF encryption interoperability test (qpdf).

Encrypts a PDF with the vanillapdf command-line tool using the requested
algorithm and key length, then verifies that an independent implementation
(qpdf) can decrypt it with the user and owner passwords and rejects a wrong
password.

This guards against key-derivation bugs that a self round-trip cannot catch: a
scheme that is wrong but internally consistent decrypts fine against itself
while being rejected by every third-party reader. It applies to every standard
security handler:

    RC4  40  -> V=1/V=2, R=2/R=3
    RC4 128  -> V=2,     R=3
    AES 128  -> V=4,     R=4  (AESV2)
    AES 256  -> V=5,     R=6  (AESV3, ISO 32000-2 Algorithm 2.B)

Usage:
    verify_encryption_interop_qpdf.py <tools_exe> <source_pdf> <output_dir> \
        <algorithm> <key_length>

    algorithm   : RC4 | AES
    key_length  : 40 | 128 | 256
"""

import os
import subprocess
import sys

OWNER_PASSWORD = "owner-secret"
USER_PASSWORD = "user-secret"
WRONG_PASSWORD = "definitely-not-the-password"


def main():
    if len(sys.argv) < 6:
        print(__doc__)
        return 2

    tools_exe, source_pdf, output_dir, algorithm, key_length = sys.argv[1:6]

    os.makedirs(output_dir, exist_ok=True)
    destination = os.path.join(
        output_dir, "interop_{}_{}.pdf".format(algorithm.lower(), key_length)
    )

    command = [
        tools_exe, "encrypt",
        "-s", source_pdf,
        "-d", destination,
        "-op", OWNER_PASSWORD,
        "-up", USER_PASSWORD,
        "-ka", algorithm,
        "-kl", key_length,
    ]

    # Do not print the command: it carries the -op/-up passwords.
    print("Encrypting {} as {} {}".format(source_pdf, algorithm, key_length))
    result = subprocess.run(command)
    if result.returncode != 0:
        print("FAILED: vanillapdf encryption exited with", result.returncode)
        return 1

    decrypted = os.path.join(output_dir, "qpdf_" + os.path.basename(destination))

    def qpdf_decrypt(password):
        # qpdf exits 0 on success, non-zero on error (e.g. wrong password).
        return subprocess.run(
            ["qpdf", "--decrypt", "--password=" + password, destination, decrypted]
        ).returncode

    if qpdf_decrypt(USER_PASSWORD) != 0:
        print("FAILED: qpdf could not decrypt with the user password")
        return 1
    print("qpdf: user password decrypted successfully")

    if qpdf_decrypt(OWNER_PASSWORD) != 0:
        print("FAILED: qpdf could not decrypt with the owner password")
        return 1
    print("qpdf: owner password decrypted successfully")

    if qpdf_decrypt(WRONG_PASSWORD) == 0:
        print("FAILED: qpdf accepted a wrong password")
        return 1
    print("qpdf: wrong password correctly rejected")

    print("{} {} interop test PASSED".format(algorithm, key_length))
    return 0


if __name__ == "__main__":
    sys.exit(main())
