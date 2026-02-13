# Tooling Lessons

- The Edit tool requires a prior Read call on the same file in the same conversation. Grep results do not count as reading. Always Read before Edit.
- Never remove git worktrees or take other destructive actions (branch deletion, force push, file deletion) without explicit user request. When blocked by a worktree on `main`, create a new branch from the current checkout instead.
- After merging branches, always grep for duplicate class/function definitions (e.g., `class Foo` appearing twice in the same header). Git auto-merge silently includes both copies when two branches independently add the same symbol, causing C++ redefinition errors.
