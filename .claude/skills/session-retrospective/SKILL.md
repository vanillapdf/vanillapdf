---
description: Capture lessons learned from the current session to improve future accuracy and speed
auto-invoke: true
---

# Session Retrospective

## When to auto-invoke

Trigger this skill when any of the following happen during conversation:
- The user corrects a mistake ("that's wrong", "I don't like this", "this is incorrect")
- A tool call fails and requires a different approach
- Something takes multiple attempts that should have taken one
- The user shares non-obvious codebase knowledge or preferences
- The conversation is wrapping up (user says "thanks", "that's all", commits final work)

## What to capture

For each lesson, record:
- **What happened**: Brief description of the mistake or discovery
- **Why it matters**: Impact on speed or correctness
- **Rule**: The actionable takeaway

## Where to write

### Actionable rules → `.claude/rules/lessons/<topic>.md`

Topic-based files under `.claude/rules/lessons/`. Each file stays small and focused. Current files:
- `build-and-testing.md` — Build commands, test execution shortcuts
- `exceptions.md` — Exception type usage and include patterns
- `tooling.md` — Claude Code tool quirks and workarounds

Create new topic files as needed (e.g., `git.md`, `api.md`, `codebase.md`). Format: bullet points, one rule per line, concise.

Only add a rule if it would prevent a real mistake from recurring. Do not add obvious or generic advice.

### Detailed notes → memory files

Write verbose context, session-specific details, and exploratory notes to files under the auto memory directory. Use topic-based files (e.g., `debugging.md`, `codebase-knowledge.md`). Link from `MEMORY.md` if adding a new file.

## When invoked manually

When the user runs `/session-retrospective`, review the full conversation and:
1. Identify all corrections, failed attempts, and rediscovered knowledge
2. Update the appropriate files under `.claude/rules/lessons/` and memory files
3. Summarize what was added or changed
