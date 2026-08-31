# Coding Style

Detailed coding style preferences for C++ code in this project. This file is continuously updated as new patterns are established.

## Variable Initialization

- Prefer `= value` over brace initialization `{value}` for scalar, atomic, and member variables
  - Good: `std::atomic<int64_t> counter = 0;` / `int m_count = 0;`
  - Avoid: `std::atomic<int64_t> counter{0};` / `int m_count{0};`

## Naming

- Prefer full, descriptive identifiers over abbreviations for locals, members, and parameters. The codebase spells names out in full (`key_length`, `error_message_length`, `range_length`, `after_signature_contents_length`, `m_key_length`).
  - Good: `auto password_length = std::min<types::size_type>(password.size(), 127);`
  - Avoid: `auto pw_length = std::min<types::size_type>(password.size(), 127);`

## Move Semantics

- Do not use `std::move` unless it is mandatory (e.g., moving a non-copyable type like `unique_ptr`). The codebase uses copy semantics for smart pointers (Deferred<T>) and will adopt `std::move` incrementally. Sprinkling `std::move` ahead of that effort breaks style consistency.
- Prefer proper named constructors over aggregate brace-initialization for structs: write `Frame(node)` with an explicit constructor, not `{node, 0}`.
  - Good: `stack.push_back(Frame(root));`
  - Avoid: `stack.push_back({root, 0});`

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

## Consistency with Existing Patterns

- Before proposing a new method signature, search the codebase for similar methods and match their pattern (e.g., `TryFind()` with `OutputPointer<T>&` out param + `bool` return, not `FindOptional()` returning nullable)
- Before defining new constants, search `constants.h` and the codebase for existing ones (e.g., FNV-1a constants)
- When modifying a data structure (e.g., changing container type), think through ALL implications on hash, equality, comparison, and API signatures upfront — ensure they are consistent with each other before implementing
- Prefer keeping existing infrastructure (e.g., `std::hash` specializations) over introducing local alternatives (e.g., per-use hash structs)

## Dictionary Mutation

- Use `Insert` with `overwrite = true` instead of the Remove + Insert dance in setters
  - Good: `_obj->Insert(constant::Name::MK, value, true);`
  - Avoid: `if (_obj->Contains(constant::Name::MK)) { _obj->Remove(constant::Name::MK); } _obj->Insert(constant::Name::MK, value);`
- For create-if-missing sub-dictionaries, invert the logic: insert when absent, then a single `FindAs` covers both cases
  - Good:
    ```cpp
    if (!_obj->Contains(constant::Name::AP)) {
        syntax::DictionaryObjectPtr new_appearance_dictionary;
        _obj->Insert(constant::Name::AP, new_appearance_dictionary);
    }
    auto appearance_dictionary = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::AP);
    ```
  - Avoid: declaring an empty `DictionaryObjectPtr` and assigning it from `FindAs` in one branch while inserting it in the other

## Error Handling

- Use `LOG_ERROR_AND_THROW(ExceptionType, "message: {}", value)` instead of a plain `throw` where possible — it logs through spdlog and throws with the formatted message (available via `precompiled.h`)
  - Good: `LOG_ERROR_AND_THROW(ParseException, "Unknown XObject subtype: {}", subtype->ToString());`
  - Avoid: `throw ParseException("Unknown XObject subtype: " + subtype->ToString());`

## Type Hierarchies

- Base classes use the `*Base` suffix (`XObjectBase`, `AnnotationBase`, `FontBase`) — never a `Base*` prefix
- The hierarchy pattern: a `Type` enum with `Undefined = 0` inside the base class, a pure virtual `GetXxxType()` implemented inline by subclasses, a static `Create` factory dispatching on the subtype name, and C API `Xxx_ToBaseYyy` / `Xxx_FromBaseYyy` conversions via `SafeObjectConvert`
- Factory `Create` methods return `Deferred` (e.g. `XObjectPtr`), not `std::unique_ptr` — the `unique_ptr` + `release()` factory pattern (`AnnotationBase::Create`, `FontBase::Create`) is obsolete; do not copy it into new code

## C API Conventions

- `Xxx_FindYyy` returns `VANILLAPDF_ERROR_OBJECT_MISSING` when the entry is absent — no boolean out param at the C level, even when the C++ side uses the `TryFind` + `bool` convention (`InteractiveForm_FindField` precedent)
- `Xxx_RemoveYyy` takes a `boolean_type* result` out param mirroring the C++ `bool` return — absence is reported through the flag, not an error code
- Semantic objects that get attached by **indirect reference** (form XObjects via `/AP`, widgets via `/Annots` and the field tree, pages, fields) are created with `Xxx_CreateFromDocument(DocumentHandle*, ...)` — the C++ `Create(DocumentPtr)` allocates an xref entry up front (`FormXObject::Create`, `WidgetAnnotation::Create`, `PageObject::Create` precedent), so a later attaching setter can never produce a dangling reference. Do not add new detached `CreateFromRect`-style constructors — the existing ones on the markup annotations predate this rule and their deprecation is tracked in #537

## Memory Allocation

- Use `pdf_new` instead of `std::make_unique` or raw `new` for heap allocations — it enables debug memory tracking on MSVC (`_CRTDBG_MAP_ALLOC`)
  - Good: `auto obj = pdf_new IntegerObject(42);`
  - Avoid: `auto obj = std::make_unique<IntegerObject>(42);`

## Operator Conventions

- `operator[]` MUST return by reference — this is standard C++ convention and required for `DeferredContainer<T>` compatibility
- For thread-safe by-value access, provide a separate `GetValue()` method that acquires a lock guard and returns a copy
  - `operator[]` → fast, no lock, returns `T&`
  - `GetValue()` → thread-safe, holds lock, returns `T` by value

## Cross-Platform Compiler Behavior

- Clang with `-Werror,-Wswitch` requires every enum value to be handled in `switch` statements (including `Undefined`); MSVC does not warn. Always add a `default:` case when switching on enums.

## Header Dependencies

- NEVER include public C API headers (`include/vanillapdf/`) from internal C++ headers (`src/vanillapdf/`) — this is an inverted dependency
- Internal C++ code should only include other internal headers
- The C API implementation layer (`src/vanillapdf/implementation/`) bridges between C headers and C++ internals
