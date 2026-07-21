#!/usr/bin/env python3
"""
External PDF conformance validation using qpdf.

Re-saves each test PDF through vanillapdf.tools and validates the output
with ``qpdf --check``.

Which files to test is an opt-in allowlist (conformance_check.cfg), keyed by
corpus-relative path. Passwords for encrypted files come from the corpus
manifest.json (single source of truth), not the allowlist.

Usage:
    python scripts/conformance_check.py \
        --tools-binary build/.../vanillapdf.tools[.exe] \
        --source-dir <extracted corpus dir> \
        --config scripts/conformance_check.cfg
        # --manifest defaults to <source-dir>/../manifest.json
"""

import argparse
import io
import json
import os
import shutil
import subprocess
import sys
import tempfile
import unicodedata


def normalize_string(s):
    """Match the normalisation used by run_test.py."""
    normalized = unicodedata.normalize("NFC", s)
    encoded = normalized.encode("utf8")
    return encoded.decode("latin-1")


def load_config(config_path):
    with io.open(config_path, encoding="utf8") as fh:
        return json.load(fh)


def password_for(rel_path, manifest_files):
    """Return a password for corpus file *rel_path* from the manifest, or None."""
    entry = manifest_files.get("corpus/" + rel_path)
    if entry is None:
        return None
    for key in ("owner_password", "user_password"):
        if key in entry:
            return normalize_string(entry[key])
    return None


def _collect_output(result):
    """Safely concatenate stdout and stderr, handling None."""
    parts = []
    if result.stdout:
        parts.append(result.stdout)
    if result.stderr:
        parts.append(result.stderr)
    return "\n".join(parts)


def run_resave(tools_binary, src, dst, password=None, timeout=120):
    """Run ``vanillapdf.tools resave`` and return (returncode, output)."""
    cmd = [tools_binary, "resave", "-s", src, "-d", dst]
    if password is not None:
        cmd += ["-p", password]
    result = subprocess.run(
        cmd, capture_output=True, encoding="utf-8", errors="replace",
        timeout=timeout,
    )
    return result.returncode, _collect_output(result)


def run_qpdf_check(qpdf_binary, pdf_path, password=None, timeout=60):
    """Run ``qpdf --check`` and return (returncode, combined output)."""
    cmd = [qpdf_binary, "--check", pdf_path]
    if password is not None:
        cmd += ["--password=" + password]
    result = subprocess.run(
        cmd, capture_output=True, encoding="utf-8", errors="replace",
        timeout=timeout,
    )
    return result.returncode, _collect_output(result)


def run_qpdf_decrypt(qpdf_binary, src, dst, password, timeout=60):
    """Run ``qpdf --decrypt`` and return (returncode, combined output)."""
    cmd = [qpdf_binary, "--decrypt", "--password=" + password, src, dst]
    result = subprocess.run(
        cmd, capture_output=True, encoding="utf-8", errors="replace",
        timeout=timeout,
    )
    return result.returncode, _collect_output(result)


def main():
    parser = argparse.ArgumentParser(
        description="Validate vanillapdf round-trip output with qpdf"
    )
    parser.add_argument(
        "--tools-binary",
        required=True,
        help="Path to the vanillapdf.tools executable",
    )
    parser.add_argument(
        "--source-dir",
        required=True,
        help="Repository root (file paths in config are relative to this)",
    )
    parser.add_argument(
        "--config",
        required=True,
        help="Path to the conformance opt-in list (conformance_check.cfg)",
    )
    parser.add_argument(
        "--manifest",
        default=None,
        help="Path to the corpus manifest.json (default: alongside the corpus)",
    )
    parser.add_argument(
        "--qpdf",
        default="qpdf",
        help="Path to the qpdf binary (default: qpdf)",
    )
    parser.add_argument(
        "--output-dir",
        default=None,
        help="Directory for resaved PDFs (default: auto-created temp dir)",
    )
    parser.add_argument(
        "--keep-output",
        action="store_true",
        help="Do not delete the output directory on exit",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Print details for every file, not just failures",
    )
    args = parser.parse_args()

    # Verify prerequisites
    if not os.path.isfile(args.tools_binary):
        print(f"ERROR: tools binary not found: {args.tools_binary}")
        return 1
    if not os.path.isdir(args.source_dir):
        print(f"ERROR: source directory not found: {args.source_dir}")
        return 1
    if not os.path.isfile(args.config):
        print(f"ERROR: config file not found: {args.config}")
        return 1

    # Verify qpdf is available
    try:
        subprocess.run(
            [args.qpdf, "--version"],
            capture_output=True,
            text=True,
            timeout=10,
        )
    except FileNotFoundError:
        print(f"ERROR: qpdf not found at: {args.qpdf}")
        return 1

    config = load_config(args.config)
    file_list = config.get("Files", [])

    if not file_list:
        print("No files listed in config")
        return 1

    # Passwords come from the corpus manifest (single source of truth), not the
    # opt-in list. Default to the manifest co-located with the corpus.
    manifest_path = args.manifest
    if manifest_path is None:
        manifest_path = os.path.join(
            os.path.dirname(os.path.normpath(args.source_dir)), "manifest.json")
    if not os.path.isfile(manifest_path):
        print(f"ERROR: manifest not found: {manifest_path}")
        return 1
    manifest_files = load_config(manifest_path).get("files", {})

    # Prepare output directory
    tmp_created = False
    output_dir = args.output_dir
    if output_dir is None:
        output_dir = tempfile.mkdtemp(prefix="vanillapdf_conformance_")
        tmp_created = True
    else:
        os.makedirs(output_dir, exist_ok=True)

    total = 0
    passed = 0
    failed = 0
    missing = 0
    resave_failures = []
    qpdf_failures = []

    for rel_path in file_list:
        pdf_path = os.path.join(args.source_dir, rel_path)
        filename = os.path.basename(pdf_path)
        total += 1

        if not os.path.isfile(pdf_path):
            missing += 1
            print(f"  MISS  {rel_path} (file not found)")
            continue

        password = password_for(rel_path, manifest_files)

        # Build output path preserving relative structure
        out_path = os.path.join(output_dir, rel_path)
        os.makedirs(os.path.dirname(out_path), exist_ok=True)

        # Step 1: resave
        rc, output = run_resave(
            args.tools_binary, pdf_path, out_path, password=password
        )
        if rc != 0:
            failed += 1
            resave_failures.append((rel_path, output.strip()))
            print(f"  FAIL  {rel_path} (resave returned {rc})")
            if args.verbose and output.strip():
                for line in output.strip().splitlines():
                    print(f"        {line}")
            continue

        # Step 2: qpdf --check
        rc, output = run_qpdf_check(args.qpdf, out_path, password=password)
        if rc != 0:
            # qpdf exit code 3 = warnings only (not errors)
            if rc == 3:
                passed += 1
                if args.verbose:
                    print(f"  WARN  {rel_path} (qpdf warnings)")
            else:
                failed += 1
                qpdf_failures.append((rel_path, output.strip()))
                print(f"  FAIL  {rel_path} (qpdf --check returned {rc})")
                if args.verbose and output.strip():
                    for line in output.strip().splitlines():
                        print(f"        {line}")
                continue
        else:
            passed += 1

        # Step 3: for encrypted files, verify qpdf can decrypt them
        if password is not None:
            decrypted_path = out_path + ".decrypted.pdf"
            rc, output = run_qpdf_decrypt(
                args.qpdf, out_path, decrypted_path, password=password
            )
            # qpdf exit code 3 = warnings only (not errors)
            # Known: stream /Length mismatch in encrypted files
            # https://github.com/vanillapdf/vanillapdf/issues/364
            if rc not in (0, 3):
                # Undo the pass from step 2
                passed -= 1
                failed += 1
                qpdf_failures.append((rel_path + " (decrypt)", output.strip()))
                print(f"  FAIL  {rel_path} (qpdf --decrypt returned {rc})")
                if args.verbose and output.strip():
                    for line in output.strip().splitlines():
                        print(f"        {line}")
                continue

        if args.verbose:
            print(f"  PASS  {rel_path}")

    # Summary
    print()
    print("=" * 60)
    print(f"Conformance check summary")
    print(f"  Total:   {total}")
    print(f"  Passed:  {passed}")
    print(f"  Failed:  {failed}")
    if missing > 0:
        print(f"  Missing: {missing}")
    print("=" * 60)

    if resave_failures:
        print()
        print(f"Resave failures ({len(resave_failures)}):")
        for name, msg in resave_failures:
            print(f"  - {name}")
            if msg:
                for line in msg.splitlines()[:3]:
                    print(f"    {line}")

    if qpdf_failures:
        print()
        print(f"qpdf failures ({len(qpdf_failures)}):")
        for name, msg in qpdf_failures:
            print(f"  - {name}")
            if msg:
                for line in msg.splitlines()[:3]:
                    print(f"    {line}")

    # Cleanup
    if tmp_created and not args.keep_output:
        shutil.rmtree(output_dir, ignore_errors=True)

    return 1 if (failed > 0 or missing > 0) else 0


if __name__ == "__main__":
    sys.exit(main())
