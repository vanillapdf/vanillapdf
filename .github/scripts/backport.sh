#!/bin/bash
#
# Backport script for cherry-picking merged PRs to release branches.
# Called by the backport GitHub Actions workflow.
#
# Required environment variables:
#   GH_TOKEN      - GitHub token for API calls
#   PR_NUMBER     - Original PR number
#   PR_TITLE      - Original PR title
#   PR_BODY       - Original PR body
#   MERGE_COMMIT  - Merge commit SHA to cherry-pick
#   TARGETS       - JSON array of target branches (e.g., '["release/2.2"]')
#

set -e

# Validate required environment variables
validate_env() {
    local required_vars=("GH_TOKEN" "PR_NUMBER" "PR_TITLE" "MERGE_COMMIT" "TARGETS")
    for var in "${required_vars[@]}"; do
        if [[ -z "${!var}" ]]; then
            echo "::error::Required environment variable $var is not set"
            exit 1
        fi
    done
}

# Check if target branch exists on remote
check_target_branch() {
    local target="$1"
    if ! git ls-remote --exit-code --heads origin "$target" > /dev/null 2>&1; then
        echo "::error::Target branch '$target' does not exist"
        return 1
    fi
    return 0
}

# Create backport branch and cherry-pick
create_backport_branch() {
    local target="$1"
    local branch_name="$2"

    git fetch origin "$target"
    git checkout -b "$branch_name" "origin/$target"

    if git cherry-pick -x "$MERGE_COMMIT"; then
        return 0
    else
        return 1
    fi
}

# Push branch and create PR
create_backport_pr() {
    local target="$1"
    local branch_name="$2"

    git push origin "$branch_name"

    # Create PR body
    local pr_body="Backport of #${PR_NUMBER} to \`$target\`.

---

${PR_BODY:-No description provided.}"

    # Create the PR
    local pr_url
    pr_url=$(gh pr create \
        --base "$target" \
        --head "$branch_name" \
        --title "[Backport $target] $PR_TITLE" \
        --body "$pr_body")

    echo "$pr_url"
}

# Post success comment on original PR
post_success_comment() {
    local target="$1"
    local pr_url="$2"

    gh pr comment "$PR_NUMBER" --body "✅ Backport to \`$target\` created: $pr_url"
}

# Post failure comment with manual instructions (cherry-pick conflicts)
post_failure_comment() {
    local target="$1"
    local branch_name="$2"

    local failure_msg="❌ Backport to \`$target\` failed due to cherry-pick conflicts.

Please backport manually:

\`\`\`bash
git fetch origin $target
git checkout -b $branch_name origin/$target
git cherry-pick -x $MERGE_COMMIT
# Resolve conflicts, then:
git push origin $branch_name
gh pr create --base $target --head $branch_name
\`\`\`"

    gh pr comment "$PR_NUMBER" --body "$failure_msg"
}

# Post push/PR creation failure comment
post_push_failure_comment() {
    local target="$1"
    local branch_name="$2"

    local failure_msg="❌ Backport to \`$target\` failed during push or PR creation.

Please check the workflow logs and try manually:

\`\`\`bash
git fetch origin $target
git checkout -b $branch_name origin/$target
git cherry-pick -x $MERGE_COMMIT
git push origin $branch_name
gh pr create --base $target --head $branch_name
\`\`\`"

    gh pr comment "$PR_NUMBER" --body "$failure_msg"
}

# Clean up after processing a target
cleanup() {
    local branch_name="$1"

    # Return to the base branch and drop the temp branch. Both always exist here
    # (main is the default branch; the temp branch was created in
    # create_backport_branch), so a failure is a real error, not noise to hide.
    git checkout main
    git branch -D "$branch_name"
}

# Process a single backport target
# Returns 0 on success, 1 on failure
process_target() {
    local target="$1"
    local branch_name="backport-${PR_NUMBER}-to-${target//\//-}"
    local result=0

    echo "::group::Backporting to $target"

    # Check if target branch exists
    if ! check_target_branch "$target"; then
        gh pr comment "$PR_NUMBER" --body "❌ Backport to \`$target\` failed: branch does not exist."
        gh pr edit "$PR_NUMBER" --add-label "backport-failed $target"
        echo "::endgroup::"
        return 1
    fi

    # Create backport branch and cherry-pick
    if create_backport_branch "$target" "$branch_name"; then
        # Cherry-pick succeeded, try to create PR
        local pr_url
        if pr_url=$(create_backport_pr "$target" "$branch_name"); then
            echo "Created backport PR: $pr_url"
            post_success_comment "$target" "$pr_url"
        else
            echo "::error::Push or PR creation failed for $target"
            post_push_failure_comment "$target" "$branch_name"
            gh pr edit "$PR_NUMBER" --add-label "backport-failed $target"
            result=1
        fi
    else
        # Cherry-pick failed with conflicts, so one is in progress — abort it.
        git cherry-pick --abort
        echo "::error::Cherry-pick failed for $target (likely conflicts)"
        post_failure_comment "$target" "$branch_name"
        gh pr edit "$PR_NUMBER" --add-label "backport-failed $target"
        result=1
    fi

    # Clean up for next iteration
    cleanup "$branch_name"

    echo "::endgroup::"
    return $result
}

# Main entry point
main() {
    validate_env

    echo "Processing backport for PR #$PR_NUMBER"
    echo "Merge commit: $MERGE_COMMIT"
    echo "Targets: $TARGETS"

    local failed=0

    # Parse targets JSON array and process each
    for target in $(echo "$TARGETS" | jq -r '.[]'); do
        if ! process_target "$target"; then
            failed=1
        fi
    done

    echo "Backport processing complete"

    if [[ $failed -ne 0 ]]; then
        echo "::error::One or more backports failed"
        exit 1
    fi
}

main "$@"
