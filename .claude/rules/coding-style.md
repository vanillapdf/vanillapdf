# Coding Style

Detailed coding style preferences for C++ code in this project. This file is continuously updated as new patterns are established.

## Variable Initialization

- Prefer `= value` over brace initialization `{value}` for scalar, atomic, and member variables
  - Good: `std::atomic<int64_t> counter = 0;` / `int m_count = 0;`
  - Avoid: `std::atomic<int64_t> counter{0};` / `int m_count{0};`

## Increment and Decrement

- Avoid standalone `var--`, `var++`, `++var`, `--var` as a full statement on its own line; use compound assignment `var -= 1` or `var += 1` instead
  - Good: `s_active_object_count -= 1;` / `s_total_objects_created += 1;`
  - Avoid: `s_active_object_count--;` / `s_total_objects_created++;`
- Inline prefix `++var` / `--var` within expressions is fine (e.g., `auto current = ++s_active_object_count;`, `if (--m_ref_counter == 0)`)

## Control Flow

- When two consecutive single-line `if` statements can be combined with `||`, prefer the combined form
  - Good: `return (A) || (B);`
  - Avoid: `if (A) return true; return B;`

## Visual Cohesion

- Do not insert structurally different code into a group of similarly-looking lines without separating it with a blank line and a comment
- Uniform blocks (e.g., a series of assignments, a series of `if`/`return` checks) should stay visually cohesive
