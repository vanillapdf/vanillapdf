#!/usr/bin/env python3
"""PDF encryption interoperability test (qpdf).

Encrypts a PDF with the vanillapdf command-line tool using the requested
algorithm and key length, then verifies that an independent implementation
(qpdf) reports the algorithm we actually asked for, can decrypt it with the user
and owner passwords, and rejects a wrong password.

This guards against key-derivation bugs that a self round-trip cannot catch: a
scheme that is wrong but internally consistent decrypts fine against itself
while being rejected by every third-party reader. It applies to every standard
security handler:

    RC4  40  -> V=1/V=2, R=2/R=3         (qpdf reports RC4)
    RC4 128  -> V=2,     R=3             (qpdf reports RC4)
    AES 128  -> V=4,     R=4             (qpdf reports AESv2)
    AES 256  -> V=5,     R=6             (qpdf reports AESv3, ISO 32000-2 Algorithm 2.B)

The algorithm check is deliberately independent of decryption. Requesting AES
and receiving RC4 still decrypts and still round-trips against ourselves -- the
reported encryption method is the only thing that reveals it.

Usage:
    verify_encryption_interop_qpdf.py <tools_exe> <source_pdf> <output_dir> \
        <algorithm> <key_length>

    algorithm   : RC4 | AES
    key_length  : 40 | 128 | 256
"""

import os
import re
import subprocess
import sys

OWNER_PASSWORD = "owner-secret"
USER_PASSWORD = "user-secret"
WRONG_PASSWORD = "definitely-not-the-password"

# Encryption method name that "qpdf --show-encryption" must report for each
# requested algorithm/key length combination. Compared case-insensitively.
EXPECTED_ENCRYPTION_METHOD = {
    ("RC4", "40"): "rc4",
    ("RC4", "128"): "rc4",
    ("AES", "128"): "aesv2",
    ("AES", "256"): "aesv3",
}

# "stream encryption method: AESv2", "string encryption method: RC4", ...
ENCRYPTION_METHOD_PATTERN = re.compile(
    r"^\s*(stream|string|file) encryption method:\s*(\S+)\s*$",
    re.IGNORECASE | re.MULTILINE,
)


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

    def verify_algorithm():
        """Check that qpdf reports the algorithm we requested.

        Returns an error string, or None when the file matches expectations.
        """

        expected = EXPECTED_ENCRYPTION_METHOD.get((algorithm.upper(), key_length))
        if expected is None:
            return "no expected encryption method recorded for {} {}".format(
                algorithm, key_length
            )

        show = subprocess.run(
            ["qpdf", "--show-encryption", "--password=" + OWNER_PASSWORD, destination],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            universal_newlines=True,
        )

        output = show.stdout or ""
        if show.returncode != 0:
            return "qpdf --show-encryption exited with {}:\n{}".format(
                show.returncode, output
            )

        methods = {
            scope.lower(): method for scope, method in
            ENCRYPTION_METHOD_PATTERN.findall(output)
        }

        # Fail loudly rather than silently passing if qpdf ever changes how it
        # words this output -- a missing match must never read as success.
        if not methods:
            return (
                "could not find any 'encryption method' line in qpdf output; "
                "the output format may have changed:\n{}".format(output)
            )

        for scope in ("stream", "string"):
            actual = methods.get(scope)
            if actual is None:
                return "qpdf did not report a {} encryption method:\n{}".format(
                    scope, output
                )

            if actual.lower() != expected:
                return "{} encryption method is {}, expected {}".format(
                    scope, actual, expected
                )

        print("qpdf: encryption methods {}".format(methods))
        return None

    algorithm_error = verify_algorithm()
    if algorithm_error is not None:
        print("FAILED: requested {} {} but {}".format(
            algorithm, key_length, algorithm_error
        ))
        return 1
    print("qpdf: algorithm matches the requested {} {}".format(algorithm, key_length))

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
