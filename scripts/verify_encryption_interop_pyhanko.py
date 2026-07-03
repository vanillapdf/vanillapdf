#!/usr/bin/env python3
"""PDF encryption interoperability test (pyHanko).

Encrypts a PDF with the vanillapdf command-line tool using the requested
algorithm and key length, then verifies that an independent implementation
(pyHanko) can authenticate both the user and owner passwords, reject a wrong
password, and decrypt the document content.

This guards against key-derivation bugs that a self round-trip cannot catch: a
scheme that is wrong but internally consistent decrypts fine against itself
while being rejected by every third-party reader. It applies to every standard
security handler:

    RC4  40  -> V=1/V=2, R=2/R=3
    RC4 128  -> V=2,     R=3
    AES 128  -> V=4,     R=4  (AESV2)
    AES 256  -> V=5,     R=6  (AESV3, ISO 32000-2 Algorithm 2.B)

Usage:
    verify_encryption_interop_pyhanko.py <tools_exe> <source_pdf> <output_dir> \
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

    from pyhanko.pdf_utils.reader import PdfFileReader
    from pyhanko.pdf_utils.crypt import AuthStatus

    def open_and_authenticate(password):
        with open(destination, "rb") as handle:
            reader = PdfFileReader(handle, strict=False)
            if not reader.encrypted:
                raise AssertionError("output document is not encrypted")

            auth = reader.decrypt(password)

            # Only touch encrypted content when authentication succeeded; reading
            # objects without a valid key raises rather than returning a status.
            if auth.status != AuthStatus.FAILED:
                # Prove the file encryption key works, not just that the password
                # hash matched, by decrypting real content.
                page_count = int(reader.root["/Pages"]["/Count"])
                if page_count < 1:
                    raise AssertionError("decrypted document reports no pages")

                if "/Metadata" in reader.root:
                    metadata = reader.root["/Metadata"].data
                    if not metadata:
                        raise AssertionError("metadata stream decrypted to empty data")

            return auth.status

    user_status = open_and_authenticate(USER_PASSWORD)
    if user_status != AuthStatus.USER:
        print("FAILED: pyHanko user authentication returned", user_status)
        return 1
    print("pyHanko: user password authenticated as USER")

    owner_status = open_and_authenticate(OWNER_PASSWORD)
    if owner_status != AuthStatus.OWNER:
        print("FAILED: pyHanko owner authentication returned", owner_status)
        return 1
    print("pyHanko: owner password authenticated as OWNER")

    wrong_status = open_and_authenticate(WRONG_PASSWORD)
    if wrong_status != AuthStatus.FAILED:
        print("FAILED: pyHanko accepted a wrong password as", wrong_status)
        return 1
    print("pyHanko: wrong password correctly rejected")

    print("{} {} interop test PASSED".format(algorithm, key_length))
    return 0


if __name__ == "__main__":
    sys.exit(main())
