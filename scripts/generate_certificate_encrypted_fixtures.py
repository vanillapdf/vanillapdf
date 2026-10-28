#!/usr/bin/env python3
"""Generate the certificate encrypted test fixtures in fixtures/certificate-encrypted.

Each fixture is a one page document encrypted with the public-key security
handler (/Filter /Adobe.PubSec): the file key derives from a seed, which is
wrapped into a PKCS#7 envelope for every recipient. The fixtures cover the
envelope ciphers older and current producers write (RC2, 3DES, AES) and the
content encryption of both subfilters (adbe.pkcs7.s4: RC4, adbe.pkcs7.s5: crypt
filters with RC4, AESV2 or AESV3).

    python3 scripts/generate_certificate_encrypted_fixtures.py \\
        --recipient TestUser3.cert.pem --other-recipient other.cert.pem

The recipient is the certificate of the testdata corpus TestUser3.pfx
(issuer C=SK, serial 4), which the tools tests decrypt the fixtures with. The
other recipient is any RSA certificate, used only for the envelope that does
not belong to the decrypting key. Requires the OpenSSL 3 command line with the
legacy provider, which writes the RC2 envelopes. Python standard library only.
"""

import argparse
import hashlib
import os
import subprocess
import tempfile
import zlib
from pathlib import Path

PERMISSIONS = (0xFFFFF0C1).to_bytes(4, "little", signed=False)
PRODUCER = b"vanillapdf certificate encrypted fixture generator"


class Variant:
    def __init__(self, name, subfilter, content, envelope_cipher, other_recipient=False):
        self.name = name
        self.subfilter = subfilter              # "s4" or "s5"
        self.content = content                  # "rc4-40", "rc4-128", "aesv2" or "aesv3"
        self.envelope_cipher = envelope_cipher  # openssl cms cipher option
        self.other_recipient = other_recipient


VARIANTS = [
    Variant("s4-rc4-40-rc2-40", "s4", "rc4-40", "-rc2-40"),
    Variant("s4-rc4-128-3des", "s4", "rc4-128", "-des3"),
    Variant("s5-aesv2-rc2-128", "s5", "aesv2", "-rc2-128"),
    Variant("s5-aesv3-rc2-128", "s5", "aesv3", "-rc2-128"),
    Variant("s5-aesv3-aes256", "s5", "aesv3", "-aes256"),
    Variant("s5-aesv2-two-recipients", "s5", "aesv2", "-aes128", other_recipient=True),
]


def rc4(key, data):
    state = list(range(256))
    j = 0
    for i in range(256):
        j = (j + state[i] + key[i % len(key)]) & 0xFF
        state[i], state[j] = state[j], state[i]

    output = bytearray()
    i = j = 0
    for byte in data:
        i = (i + 1) & 0xFF
        j = (j + state[i]) & 0xFF
        state[i], state[j] = state[j], state[i]
        output.append(byte ^ state[(state[i] + state[j]) & 0xFF])
    return bytes(output)


def aes_cbc(key, data):
    """AES-CBC with a random IV prepended and PKCS#7 padding, as PDF stores it."""
    iv = os.urandom(16)
    padding = 16 - len(data) % 16
    padded = data + bytes([padding]) * padding
    cipher = "-aes-128-cbc" if len(key) == 16 else "-aes-256-cbc"
    result = subprocess.run(["openssl", "enc", cipher, "-nopad", "-K", key.hex(), "-iv", iv.hex()],
                            input=padded, capture_output=True, check=True)
    return iv + result.stdout


def object_key(file_key, number, aes):
    salt = b"sAlT" if aes else b""
    digest = hashlib.md5(file_key + number.to_bytes(3, "little") + b"\x00\x00" + salt).digest()
    return digest[:min(len(file_key) + 5, 16)]


def encrypt(variant, file_key, number, data):
    if variant.content in ("rc4-40", "rc4-128"):
        return rc4(object_key(file_key, number, False), data)
    if variant.content == "aesv2":
        return aes_cbc(object_key(file_key, number, True), data)
    return aes_cbc(file_key, data)


def envelope(work, recipient, cipher, payload):
    source = work / "seed.bin"
    target = work / "envelope.der"
    source.write_bytes(payload)
    subprocess.run(["openssl", "cms", "-encrypt", "-binary", "-outform", "DER", cipher,
                    "-provider", "legacy", "-provider", "default",
                    "-in", str(source), "-out", str(target), str(recipient)], check=True)
    return target.read_bytes()


def key_length(variant):
    return {"rc4-40": 5, "rc4-128": 16, "aesv2": 16, "aesv3": 32}[variant.content]


def file_key(variant, seed, recipients):
    material = seed + b"".join(recipients)
    if variant.content == "aesv3":
        return hashlib.sha256(material).digest()
    return hashlib.sha1(material).digest()[:key_length(variant)]


def hex_string(value):
    return b"<" + value.hex().encode() + b">"


def encrypt_dictionary(variant, recipients):
    recipient_array = b"[" + b"".join(hex_string(recipient) for recipient in recipients) + b"]"

    if variant.subfilter == "s4":
        version = b"/V 1/R 2" if variant.content == "rc4-40" else b"/V 2/R 3/Length 128"
        return (b"<</Filter/Adobe.PubSec/SubFilter/adbe.pkcs7.s4" + version +
                b"/Recipients" + recipient_array + b">>")

    if variant.content == "aesv3":
        version, method, length = b"/V 5/R 5", b"/AESV3", b"256"
    else:
        version, method, length = b"/V 4/R 4", b"/AESV2", b"128"

    return (b"<</Filter/Adobe.PubSec/SubFilter/adbe.pkcs7.s5" + version +
            b"/CF<</DefaultCryptFilter<</CFM" + method + b"/Length " + length +
            b"/Recipients" + recipient_array + b">>>>" +
            b"/StmF/DefaultCryptFilter/StrF/DefaultCryptFilter>>")


def generate(variant, recipient, other_recipient, output_dir):
    # A fixed seed per fixture keeps the file key stable across regenerations;
    # the envelopes still differ, as OpenSSL picks a random content key
    seed = hashlib.sha256(variant.name.encode()).digest()[:20]

    with tempfile.TemporaryDirectory() as directory:
        work = Path(directory)
        recipients = []
        if variant.other_recipient:
            recipients.append(envelope(work, other_recipient, variant.envelope_cipher, seed + PERMISSIONS))
        recipients.append(envelope(work, recipient, variant.envelope_cipher, seed + PERMISSIONS))

    key = file_key(variant, seed, recipients)

    content = zlib.compress(b"BT /F1 24 Tf 72 720 Td (Hello, " + variant.name.encode() + b") Tj ET\n")
    encrypted_content = encrypt(variant, key, 4, content)
    encrypted_producer = encrypt(variant, key, 6, PRODUCER)

    objects = {
        1: b"<</Type/Catalog/Pages 2 0 R>>",
        2: b"<</Type/Pages/Kids[3 0 R]/Count 1>>",
        3: b"<</Type/Page/Parent 2 0 R/MediaBox[0 0 612 792]/Resources<</Font<</F1 5 0 R>>>>/Contents 4 0 R>>",
        4: (b"<</Length " + str(len(encrypted_content)).encode() + b"/Filter/FlateDecode>>stream\n" +
            encrypted_content + b"\nendstream"),
        5: b"<</Type/Font/Subtype/Type1/BaseFont/Helvetica>>",
        6: b"<</Producer" + hex_string(encrypted_producer) + b">>",
        7: encrypt_dictionary(variant, recipients),
    }

    output = bytearray(b"%PDF-1.7\n%\xe2\xe3\xcf\xd3\n")
    offsets = {}
    for number, body in objects.items():
        offsets[number] = len(output)
        output += str(number).encode() + b" 0 obj\n" + body + b"\nendobj\n"

    identifier = hex_string(hashlib.md5(variant.name.encode()).digest())
    xref_offset = len(output)
    output += b"xref\n0 " + str(len(objects) + 1).encode() + b"\n0000000000 65535 f \n"
    for number in sorted(objects):
        output += b"%010d 00000 n \n" % offsets[number]
    output += (b"trailer\n<</Size " + str(len(objects) + 1).encode() +
               b"/Root 1 0 R/Info 6 0 R/Encrypt 7 0 R/ID[" + identifier + identifier + b"]>>\n" +
               b"startxref\n" + str(xref_offset).encode() + b"\n%%EOF\n")

    path = output_dir / f"{variant.name}.pdf"
    path.write_bytes(bytes(output))
    print(f"{path} ({len(output)} bytes)")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--recipient", required=True, type=Path, help="PEM certificate of the decrypting key (TestUser3)")
    parser.add_argument("--other-recipient", required=True, type=Path, help="PEM certificate of an unrelated RSA recipient")
    parser.add_argument("--output-dir", type=Path,
                        default=Path(__file__).resolve().parent.parent / "fixtures" / "certificate-encrypted")
    arguments = parser.parse_args()

    arguments.output_dir.mkdir(parents=True, exist_ok=True)
    for variant in VARIANTS:
        generate(variant, arguments.recipient, arguments.other_recipient, arguments.output_dir)


if __name__ == "__main__":
    main()
