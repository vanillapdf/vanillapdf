#!/usr/bin/env python3
"""PDF encryption interoperability test (pyHanko).

Encrypts a PDF with the vanillapdf command-line tool using the requested
algorithm and key length, then verifies that an independent implementation
(pyHanko) reports the algorithm we actually asked for, can authenticate both
the user and owner passwords, reject a wrong password, and decrypt the document
content.

This guards against key-derivation bugs that a self round-trip cannot catch: a
scheme that is wrong but internally consistent decrypts fine against itself
while being rejected by every third-party reader. It applies to every standard
security handler:

    RC4  40  -> V=1/V=2, R=2/R=3         (/V2,    5-byte key)
    RC4 128  -> V=2,     R=3             (/V2,   16-byte key)
    AES 128  -> V=4,     R=4             (/AESV2, 16-byte key)
    AES 256  -> V=5,     R=6             (/AESV3, 32-byte key, ISO 32000-2 Algorithm 2.B)

The algorithm check is deliberately independent of authentication. Requesting
AES and receiving RC4 still authenticates, still decrypts, and still round-trips
against ourselves -- the crypt filter method is the only thing that reveals it.

Usage:
    verify_encryption_interop_pyhanko.py <tools_exe> <source_pdf> <output_dir> \
        <algorithm> <key_length>

    algorithm   : RC4 | AES
    key_length  : 40 | 128 | 256
"""

import collections
import os
import subprocess
import sys

OWNER_PASSWORD = "owner-secret"
USER_PASSWORD = "user-secret"
WRONG_PASSWORD = "definitely-not-the-password"

ExpectedEncryption = collections.namedtuple(
    "ExpectedEncryption", "method key_length_bytes revision"
)


def expected_encryption(algorithm, key_length):
    """What pyHanko must report for a requested algorithm and key length.

    Mirrors the selection in EncryptionUtils::CreateEncryptionDictionary rather than
    enumerating key lengths, so every valid RC4 length is covered. The crypt filter method
    is the authoritative signal: /V2 is RC4, /AESV2 is AES-128, /AESV3 is AES-256.

    Returns None for combinations the standard security handler cannot express.
    """

    if algorithm == "AES":
        if key_length == 128:
            return ExpectedEncryption(method="/AESV2", key_length_bytes=16, revision=4)

        if key_length == 256:
            return ExpectedEncryption(method="/AESV3", key_length_bytes=32, revision=6)

        return None

    if algorithm == "RC4":
        if 40 <= key_length <= 128 and key_length % 8 == 0:
            # Revision 2 only carries 40-bit keys; anything longer moves to revision 3.
            return ExpectedEncryption(
                method="/V2",
                key_length_bytes=key_length // 8,
                revision=(2 if key_length == 40 else 3),
            )

        return None

    return None


# Only RC4-40 and RC4-128 are exercised against pyHanko, even though the library writes any
# length from 40 to 128. Algorithm 1 makes the object key min(file key + 5, 16) bytes, so
# RC4-56 needs a 96-bit and RC4-80 a 120-bit RC4 key. Both are valid per ISO 32000-1, but the
# "cryptography" backend pyHanko uses accepts only 40, 56, 64, 80, 128, 160, 192 and 256 bit
# keys and raises on the rest. qpdf covers those lengths instead; do not add them here.


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

    from pyhanko.pdf_utils.reader import PdfFileReader
    from pyhanko.pdf_utils.crypt import AuthStatus

    def verify_algorithm():
        """Check that the file really uses the algorithm we requested.

        Returns an error string, or None when the file matches expectations.
        """

        expected = expected_encryption(algorithm.upper(), int(key_length))
        if expected is None:
            return "{} {} is not a combination the standard security handler can express".format(
                algorithm, key_length
            )

        with open(destination, "rb") as handle:
            reader = PdfFileReader(handle, strict=False)
            if not reader.encrypted:
                return "output document is not encrypted"

            reader.decrypt(OWNER_PASSWORD)
            handler = reader.security_handler

            crypt_filter_config = handler.crypt_filter_config
            stream_filter = crypt_filter_config.get_for_stream()
            string_filter = crypt_filter_config.get_for_string()

            # SecurityHandlerVersion and StandardSecuritySettingsRevision are
            # plain Enums, not IntEnums -- int() on them raises.
            print(
                "pyHanko: V={} R={} keylen={} stream={} string={}".format(
                    handler.version.value,
                    handler.revision.value,
                    handler.keylen,
                    stream_filter.method,
                    string_filter.method,
                )
            )

            # Streams and strings are checked separately: a writer can get the
            # /StmF and /StrF crypt filters out of step with each other.
            if stream_filter.method != expected.method:
                return "stream crypt filter is {}, expected {}".format(
                    stream_filter.method, expected.method
                )

            if string_filter.method != expected.method:
                return "string crypt filter is {}, expected {}".format(
                    string_filter.method, expected.method
                )

            if handler.keylen != expected.key_length_bytes:
                return "file encryption key is {} bytes, expected {}".format(
                    handler.keylen, expected.key_length_bytes
                )

            if handler.revision.value != expected.revision:
                return "security handler revision is {}, expected {}".format(
                    handler.revision.value, expected.revision
                )

        return None

    algorithm_error = verify_algorithm()
    if algorithm_error is not None:
        print("FAILED: requested {} {} but {}".format(
            algorithm, key_length, algorithm_error
        ))
        return 1
    print("pyHanko: algorithm matches the requested {} {}".format(algorithm, key_length))

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
