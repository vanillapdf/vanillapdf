# 🧩 Vanilla.PDF Release Guide (Stable & Prerelease)

This guide outlines how to manage stable, alpha, beta, and RC releases for Vanilla.PDF using Git, tags, and GitHub workflows.

---

## ✅ Branching Strategy

### `main`

* Development happens here.
* Nightly builds (`*-nightly-*`) are triggered from this branch.
* Prerelease tags (`alpha`, `beta`, `rc`) can be made directly from here.
* 🔒 `release/x.y` branches are **not merged back** into `main`; instead, new development continues directly from `main`.
* Development happens here.
* Nightly builds (`*-nightly-*`) are triggered from this branch.
* Prerelease tags (`alpha`, `beta`, `rc`) can be made directly from here.

### `release/x.y` (optional)

* Created only when stabilizing a major/minor release.
* Allows `main` to continue diverging while testing RC builds.

```bash
git checkout -b release/2.2 main
```

---

## 🔁 Cherry-pick Strategy

To maintain a clean history and avoid merging release branches back into `main`, use this strategy for applying fixes:

| Scenario                             | Commit on...  | Then cherry-pick to...                 |
| ------------------------------------ | ------------- | -------------------------------------- |
| New feature or general bugfix        | `main`        | Optional (not in release)              |
| Critical fix for an upcoming release | `release/x.y` | Optional (to `main` if still relevant) |

> ✅ Best practice: commit to `main` first, then cherry-pick into `release/x.y` if it applies.

### 🔒 Cherry-pick Permissions

* If the `release/x.y` branch is protected (e.g., requires PRs), you **must** create a pull request with the cherry-picked commit.
* If it's not protected, you may push cherry-picked commits directly.

```bashbash
# Example:
git checkout main
# Make your fix and commit

git checkout release/2.2
git cherry-pick <commit-sha>
```

---

## 🏷️ Tagging for Prereleases

Create annotated tags to publish prerelease builds:

```bash
git tag -a v2.2.0-alpha.1 -m "Alpha release of Vanilla.PDF 2.2.0"
git push origin v2.2.0-alpha.1

# Later:
git tag -a v2.2.0-beta.1 -m "Beta release of Vanilla.PDF 2.2.0"
git push origin v2.2.0-beta.1

# And then:
git tag -a v2.2.0-rc.1 -m "Release candidate for Vanilla.PDF 2.2.0"
git push origin v2.2.0-rc.1
```

> These tags trigger GitHub Actions to publish prerelease NuGet packages.

---

## 🚀 Tagging for Stable Releases

Tag from `release/x.y` (or `main` if you skipped a release branch).
If the last prerelease (e.g., `v2.2.0-rc.1`) is already the final state, you can tag the exact same commit as the stable release:

```bash
git tag -a v2.2.0 -m "Final stable release of Vanilla.PDF 2.2.0"
git push origin v2.2.0
```

---

## 📦 Package Version Handling

### In `Directory.Packages.props`

Use stable version by default:

```xml
<PackageVersion Include="vanillapdf" Version="2.1.0" />
```

> ⚠️ Overriding this version dynamically in CI is currently not supported.

---

## 🧪 Workflow Summary

| Workflow                                           | Trigger                             | Version Source             | Publishes? |
| -------------------------------------------------- | ----------------------------------- | -------------------------- | ---------- |
| `publish-nuget-packages.yml`, `github-release.yml` | Tag: `v*` without prerelease suffix | `Directory.Packages.props` | ✅ Yes      |
| `publish-nuget-packages.yml`, `github-release.yml` | Tag: `v*` with `-alpha/-beta/-rc`   | Tag version                | ✅ Yes      |

---

## 🔧 Notes

* All prereleases must be pushed as annotated tags (`git tag -a` or with `-m`).
* Prerelease detection is handled in workflows using tag parsing:

```bash
TAG="${GITHUB_REF#refs/tags/}"
echo "tag=$TAG" >> "$GITHUB_OUTPUT"
VERSION="${TAG#v}"
EXTRA=""
if [[ "$VERSION" =~ ^[0-9]+\.[0-9]+\.[0-9]+(.*)$ ]]; then
  EXTRA="${BASH_REMATCH[1]}"
fi
if [[ -n "$EXTRA" ]]; then
  echo "prerelease=true" >> "$GITHUB_OUTPUT"
else
  echo "prerelease=false" >> "$GITHUB_OUTPUT"
fi
```

* Stable workflows **must not** match prerelease tags.

---

*Last updated: 2025-06-17*
