# Technical Documentation

This directory contains technical documentation for project management, technical debt tracking, and architectural decisions.

## Structure

- **`debt/`** - Technical debt items that need to be addressed
- **`features/`** - Planned features and enhancements
- **`decisions/`** - Architecture Decision Records (ADRs)

## Technical Debt Tracking

Technical debt items are documented with:
- **Problem description** and root cause analysis
- **Current workarounds** and their locations in codebase
- **Proper solutions** with implementation options
- **Priority and timeline** for resolution

Each debt item should have a corresponding GitHub issue for tracking and discussion.

## Contributing

When adding technical debt:
1. Create detailed documentation in `debt/[issue-name].md`
2. Create GitHub issue with appropriate labels
3. Reference the GitHub issue in any temporary code workarounds
4. Update this index when items are resolved