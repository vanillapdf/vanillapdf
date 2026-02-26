#!/usr/bin/env python3
"""Validate YAML syntax of GitHub Actions workflow files.

Usage: validate_workflows.py <file1.yml> [file2.yml ...]
"""

import sys
import yaml


def main():
    files = sys.argv[1:]
    if not files:
        print("Usage: validate_workflows.py <file1.yml> [file2.yml ...]")
        sys.exit(1)

    errors = []
    for path in files:
        try:
            with open(path) as fh:
                yaml.safe_load(fh)
        except yaml.YAMLError as e:
            errors.append(f"{path}: {e}")

    if errors:
        print("YAML syntax errors found:")
        for error in errors:
            print(error)
        sys.exit(1)

    print(f"All {len(files)} workflow files are valid YAML")


if __name__ == "__main__":
    main()
