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

import collections
import os
import re
import subprocess
import sys

OWNER_PASSWORD = "owner-secret"
USER_PASSWORD = "user-secret"
WRONG_PASSWORD = "definitely-not-the-password"

ExpectedEncryption = collections.namedtuple("ExpectedEncryption", "method revision")


def expected_encryption(algorithm, key_length):
    """What "qpdf --show-encryption" must report for a requested algorithm and key length.

    Mirrors the selection in EncryptionUtils::CreateEncryptionDictionary rather than
    enumerating key lengths, so every valid RC4 length is covered. The method is compared
    case-insensitively.

    Returns None for combinations the standard security handler cannot express.
    """

    if algorithm == "AES":
        if key_length == 128:
            return ExpectedEncryption(method="aesv2", revision=4)

        if key_length == 256:
            return ExpectedEncryption(method="aesv3", revision=6)

        return None

    if algorithm == "RC4":
        if 40 <= key_length <= 128 and key_length % 8 == 0:
            # Revision 2 only carries 40-bit keys; anything longer moves to revision 3.
            return ExpectedEncryption(method="rc4", revision=(2 if key_length == 40 else 3))

        return None

    return None


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
        "--owner-password", OWNER_PASSWORD,
        "--user-password", USER_PASSWORD,
        "--algorithm", algorithm,
        "--key-length", key_length,
    ]

    # Do not print the command: it carries the owner/user passwords.
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

        # "stream encryption method: AESv2", "string encryption method: RC4", ...
        # qpdf only emits these lines when crypt filters are in play (/V 4 and
        # above). For RC4 (/V 1 and /V 2) it prints none, and RC4 is implied by
        # the revision.
        method_pattern = re.compile(
            r"^\s*(stream|string|file) encryption method:\s*(\S+)\s*$",
            re.IGNORECASE | re.MULTILINE,
        )

        revision_pattern = re.compile(r"^\s*R\s*=\s*(\d+)\s*$", re.MULTILINE)

        # qpdf echoes the user password back when handed the owner password.
        # Keep it out of the CI log even though these are throwaway credentials.
        user_password_pattern = re.compile(
            r"^(\s*User password\s*=\s*).*$", re.IGNORECASE | re.MULTILINE
        )

        expected = expected_encryption(algorithm.upper(), int(key_length))
        if expected is None:
            return "{} {} is not a combination the standard security handler can express".format(
                algorithm, key_length
            )

        show = subprocess.run(
            ["qpdf", "--show-encryption", "--password=" + OWNER_PASSWORD, destination],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            universal_newlines=True,
        )

        output = user_password_pattern.sub(r"\1<redacted>", show.stdout or "")
        if show.returncode != 0:
            return "qpdf --show-encryption exited with {}:\n{}".format(
                show.returncode, output
            )

        revision_match = revision_pattern.search(output)

        # Fail loudly rather than silently passing if qpdf ever changes how it
        # words this output -- an unparseable report must never read as success.
        if revision_match is None:
            return (
                "could not find the 'R = ' line in qpdf output; the output "
                "format may have changed:\n{}".format(output)
            )

        revision = int(revision_match.group(1))

        methods = {
            scope.lower(): method.lower() for scope, method in
            method_pattern.findall(output)
        }

        # Absent method lines mean /V 1 or /V 2, which can only ever be RC4. An
        # AES expectation therefore fails here exactly as it should.
        stream_method = methods.get("stream", "rc4")
        string_method = methods.get("string", "rc4")

        print("qpdf: R={} stream={} string={}".format(
            revision, stream_method, string_method
        ))

        if revision != expected.revision:
            return "security handler revision is {}, expected {}".format(
                revision, expected.revision
            )

        # Streams and strings are checked separately: a writer can get the /StmF
        # and /StrF crypt filters out of step with each other.
        if stream_method != expected.method:
            return "stream encryption method is {}, expected {}".format(
                stream_method, expected.method
            )

        if string_method != expected.method:
            return "string encryption method is {}, expected {}".format(
                string_method, expected.method
            )

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
