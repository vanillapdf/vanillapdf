---
description: Update the project coding style guide when a style preference is expressed or discovered
auto-invoke: true
---

# Update Coding Style

When any of the following happen during conversation:
- The user expresses a coding style preference ("I don't like X", "use Y instead", "this looks weird")
- The user corrects a style choice in code you wrote
- You notice a consistent pattern in the codebase that isn't yet documented

Then update `.claude/rules/coding-style.md` with the new preference, using the same format as existing entries (section heading, bullet points with Good/Avoid examples).

When invoked manually with `/update-coding-style`, perform a full scan:
1. Read `.claude/rules/coding-style.md` to see what is already documented
2. Sample recent commits and files under `src/vanillapdf/` to identify recurring patterns in: naming, formatting, control flow, error handling, comments, includes
3. Add any new consistently-used patterns not yet documented
4. Remove or update any rules that contradict current practice
5. Summarize what was added or changed
