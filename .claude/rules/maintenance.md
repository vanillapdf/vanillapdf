# Maintenance Scripts

## Updating vcpkg Dependencies

Use the automated Python script to update vcpkg to the latest version:

```bash
# Check what would be updated (dry run)
python scripts/update_vcpkg.py --dry-run

# Update vcpkg and create PR
python scripts/update_vcpkg.py

# Update without creating PR
python scripts/update_vcpkg.py --no-pr

# Force update even if already on latest
python scripts/update_vcpkg.py --force
```

**Requirements:**
- Python 3.6+
- git command line
- gh CLI (for PR creation)

The script will:
1. Check current vs latest vcpkg versions
2. Create update branch if needed
3. Update vcpkg submodule and vcpkg.json baseline
4. Commit changes with descriptive message
5. Push branch and optionally create PR

## Updating Conan Data

`conan/conandata.yml` is **not tracked in git** (gitignored) — `conan create conan/`
always builds from the local checkout via `export_sources()`, so this file is
never read in that flow, and a committed copy would only ever go stale. It
exists solely for locally exercising the download-based `source()` path
(mirroring how Conan Center Index consumes the recipe). Generate it on demand:

```bash
# Auto-detect latest tag and update
python scripts/update_conandata.py

# Specific version
python scripts/update_conandata.py --version 2.3.0

# Dry run
python scripts/update_conandata.py --dry-run
```

**Requirements:**
- Python 3.6+
- git command line

The script will:
1. Download the release archive from GitHub
2. Calculate the SHA256 hash
3. Add or update the version entry in `conan/conandata.yml` (local file only)
