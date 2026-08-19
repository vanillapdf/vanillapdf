#!/usr/bin/python

# Runs a single test fixture through vanillapdf.test.
#
# Expectations (passwords, skip flags, merge/signing fixtures) come from the
# vanillapdf-testdata manifest.json, keyed by the fixture's repo-relative path.
# Every path in the manifest resolves against the extracted testdata root, so
# the fixture, the merge file, the signing certificate and any decryption
# certificate all live inside the fetched corpus.

import argparse
import io
import json
import os
import posixpath
import subprocess
import sys

USER_PASSWORD_KEY = "user_password"
OWNER_PASSWORD_KEY = "owner_password"
CERTIFICATE_KEY = "certificate"
MERGE_KEY = "merge_file"
SIGNING_KEY = "signing_certificate"

LICENSE_OPTION = "-l"
PASSWORD_OPTION = "-p"
CERTIFICATE_OPTION = "-k"
MERGE_OPTION = "-m"
QUIET_OPTION = "-q"
SIGNING_CERTIFICATE_OPTION = "-sc"

# Maps a manifest "skip" value to the vanillapdf.test command line option
SKIP_OPTIONS = {
    "process": "-sp",
    "save": "-ss",
    "edit": "-se",
    "incremental_save": "-si",
}


def parse_arguments():
    parser = argparse.ArgumentParser(description="Run a single fixture through vanillapdf.test")
    parser.add_argument("--exe", required=True, help="path to the vanillapdf.test executable")
    parser.add_argument("--testdata-root", required=True, help="root the manifest paths resolve against")
    parser.add_argument("--key", required=True, help="manifest key (repo-relative path) of this fixture")
    parser.add_argument("--manifest", required=True, help="path to manifest.json")
    parser.add_argument("--license", required=True, help="path to the license file")
    parser.add_argument("--launcher", nargs="+", help="optional launcher command that executes the test binary (e.g. the adb wrapper when cross-compiling for Android); may span multiple words")
    return parser.parse_args()


def build_base_parameters(args, config, entry):
    # posixpath keeps the joined separators forward slashes on every host:
    # these paths are consumed by the test binary, which may run on a remote
    # device (Android via the adb launcher) where only POSIX paths are valid.
    # Common parameters for every invocation of this fixture
    test_file = posixpath.join(args.testdata_root, args.key)
    parameters = [args.exe, test_file, LICENSE_OPTION, args.license, QUIET_OPTION]

    if (args.launcher):
        parameters = args.launcher + parameters

    if (MERGE_KEY in config):
        parameters.append(MERGE_OPTION)
        parameters.append(posixpath.join(args.testdata_root, config[MERGE_KEY]))

    for skip in entry.get("skip", []):
        parameters.append(SKIP_OPTIONS[skip])

    if (SIGNING_KEY in config):
        parameters.append(SIGNING_CERTIFICATE_OPTION)
        parameters.append(posixpath.join(args.testdata_root, config[SIGNING_KEY]))

    return parameters


def run(parameters):
    # Route stdout to devnull: successful runs can emit large volumes of output
    # and dominate the runtime. stderr is left intact so failures stay visible.
    return subprocess.call(parameters, stdout=subprocess.DEVNULL)


def main():
    args = parse_arguments()

    with io.open(args.manifest, encoding='utf8') as manifest_file:
        manifest = json.load(manifest_file)

    config = manifest.get("config", {})
    entry = manifest["files"][args.key]

    base_parameters = build_base_parameters(args, config, entry)

    # Encrypted fixtures carry their credentials in the manifest entry
    if (USER_PASSWORD_KEY in entry or OWNER_PASSWORD_KEY in entry):
        rv = -1

        if (USER_PASSWORD_KEY in entry):
            user_password = entry[USER_PASSWORD_KEY]
            rv = run(base_parameters + [PASSWORD_OPTION, user_password])
            if (rv != 0):
                return rv

        if (OWNER_PASSWORD_KEY in entry):
            owner_password = entry[OWNER_PASSWORD_KEY]
            rv = run(base_parameters + [PASSWORD_OPTION, owner_password])
            if (rv != 0):
                return rv

        return rv

    # Authentication using a certificate (path resolves inside the testdata root)
    if (CERTIFICATE_KEY in entry):
        certificate_path = posixpath.join(args.testdata_root, entry[CERTIFICATE_KEY])
        return run(base_parameters + [CERTIFICATE_OPTION, certificate_path])

    # Unencrypted fixture: default behavior
    return run(base_parameters)


if __name__ == "__main__":
    sys.exit(main())
