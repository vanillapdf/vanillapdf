# 🧩 Vanilla.PDF Release Guide (Stable & Prerelease)

This guide outlines how to manage stable, alpha, beta, and RC releases for Vanilla.PDF using Git, tags, and GitHub workflows.

---

## ✅ Branching Strategy

### `main`

* Development happens here.
* Nightly builds (`*-nightly-*`) are triggered from this branch.
* Prerelease tags (`alpha`, `beta`, `rc`) can be made directly from here.
* 🔒 `release/x.y` branches are **not merged back** into `main`; instead, new development continues directly from `main`.

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

```bash
# Example:
git checkout main
# Make your fix and commit

git checkout release/2.2
git cherry-pick <commit-sha>
```

---

## 🏷️ Creating a Release (Prerelease or Stable)

Tags are **not** created manually with `git tag`/`git push`. The `release.yml`
workflow owns tag creation end-to-end, for both prereleases (`alpha`, `beta`,
`rc`) and stable releases:

1. Trigger `release.yml` via `workflow_dispatch` (Actions tab, or
   `gh workflow run release.yml -f tag=v2.3.0-rc.1 -f dry_run=true`) with the
   desired tag name. Run it once with `dry_run: true` first — this builds and
   validates every package (NuGet, deb, rpm, dmg, docs, and, for stable tags
   only, the vcpkg/Conan submission dry-runs) without publishing or tagging
   anything.
2. Re-run with `dry_run: false`. This builds the packages for real and creates
   the GitHub release as a **draft**, still without creating the tag.
3. The `production` environment gate pauses the workflow. Replace the
   generated draft release body with curated notes (see
   [Release Notes](#-release-notes) below) in the GitHub UI, then approve the
   gate.
4. Approving publishes the draft, which is what actually **creates the tag**
   at the draft's target commit. This ordering avoids binding the release to
   a pre-existing tag before the draft is finalized.

> Whether a tag counts as a prerelease is derived automatically from its
> suffix (`-alpha.N`, `-beta.N`, `-rc.N`) — see the tag-parsing logic in
> `release.yml`'s `prepare` job.

---

## 📝 Release Notes

The draft release starts with GitHub's generated list of every merged pull
request. That list is the raw material, not the release notes: write curated
notes from [`.github/release-notes-template.md`](.github/release-notes-template.md)
before approving the gate. The structure is the same for every release, so
users find the same information in the same place:

1. **Pre-release notice**, for pre-releases only: a blockquote saying what
   kind of pre-release it is (alpha, beta, release candidate) and how stable
   the API is. There is no title heading; GitHub shows the release name
   right above the notes.
2. **Summary**: two or three sentences on what the release is about.
3. **⚠️ Breaking changes**, only when there are any, each with what to do
   instead. Raised minimum platform versions count.
4. **✨ Highlights**: the three to five changes worth reading about.
5. **Topic sections** in a fixed order: 📝 Interactive forms,
   🔏 Digital signatures, 🔒 Encryption, 📄 Pages & content,
   🐛 Bug fixes & robustness, 🧵 Thread safety, 🛠️ Build, CI & platforms,
   📚 Documentation, 📦 Dependencies. Empty sections are left out.
6. **⚠️ Known limitations** with workarounds, when there are any.
7. **The generated pull request list**, unchanged, collapsed in a
   `<details>` block.
8. **Full Changelog** link, as generated.

One line per change, written for library users in the imperative mood of the
pull request titles, without the conventional-commit prefix, and ending with
the pull request numbers in parentheses. Routine dependency bumps collapse
into a single line under 📦 Dependencies.

Every pre-release gets the full structure, not only the stable release: the
notes of a pre-release are what early adopters read to decide whether to test
it.

---

## 📦 Package Version Handling

`cmake/version.cmake` (`VANILLAPDF_VERSION_MAJOR/MINOR/PATCH`) is the single
source of truth for the release version. The `verify` job in `release.yml`
fails the run if the tag doesn't match these values.

`vcpkg.json` is generated from `cmake/version.cmake` at configure time
(`cmake/vcpkg_manifest.cmake`) — it does not need to be hand-edited before
tagging. `ports/vanillapdf/vcpkg.json` (the upstream vcpkg port) is updated
**after** the release via `scripts/update_port.py`, since it needs the
SHA512 of the published release tarball, which only exists once the tag has
been pushed.

---

## 🧪 Workflow Summary

| Workflow                                           | Trigger                             | Version Source             | Publishes? |
| -------------------------------------------------- | ----------------------------------- | -------------------------- | ---------- |
| `release.yml` (calls packaging, pages, release) | `workflow_dispatch` (tag input) | `cmake/version.cmake` | ✅ Yes (unless `dry_run: true`) |

### Version source

`cmake/version.cmake` is the only file that needs to match the tag before
release — the `verify` job in `release.yml` checks this and fails the run on
a mismatch. `vcpkg.json` is generated from it automatically; the upstream
`ports/vanillapdf/vcpkg.json` is synced post-release (see
[Package Version Handling](#-package-version-handling) above).

The `github-release.yml` workflow is invoked by `release.yml` with the tag and
prerelease flag already provided, so version checks are centralized in
`release.yml`.

The `github-pages.yml` workflow runs on pushes and pull requests to ensure
documentation builds cleanly. Actual deployment happens only when `release.yml`
invokes it with deployment enabled.

---

## 🔧 Notes

* Tags are created by `release.yml` (via publishing the draft release), not pushed manually.
* `release.yml` has no `push: tags:` trigger — it only runs on `pull_request`
  (dry-run only, synthetic tag from `cmake/version.cmake`) and
  `workflow_dispatch` (real releases, tag supplied as input). Prerelease
  detection is derived from that tag in the `prepare` job:

```bash
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

* Jobs that must only run for the newest stable release (docs deploy,
  vcpkg/Homebrew/Conan submission) additionally check `is_latest`, computed
  by comparing this tag against existing stable tags — see `prepare` in
  `release.yml`.

---

*Last updated: 2026-11-17*
