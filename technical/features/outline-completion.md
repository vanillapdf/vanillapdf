# Outline Feature Completion

## Overview

This document outlines the roadmap for completing the PDF outline functionality in vanillapdf. The outline system provides hierarchical navigation for PDF documents, but currently lacks support for destinations, actions, and structure elements.

## Current Status

### ✅ Phase 1: Destinations Integration (COMPLETED)

**Status:** Implemented on 2025-09-27

**What was implemented:**
- Added `OutlineItem::Destination()` C++ method in `src/vanillapdf/semantics/objects/outline.h:57`
- Implemented `OutlineItem_GetDestination()` C API function in `include/vanillapdf/semantics/c_outline.h:206`
- Added C++ implementation in `src/vanillapdf/semantics/objects/outline.cpp:136`
- Added C API implementation in `src/vanillapdf/implementation/semantics/c_outline.cpp:219`

**Technical Details:**
- Leverages existing destinations system (`DestinationBase::Create()`)
- Uses standard `OutputDestinationPtr` return pattern
- Returns `VANILLAPDF_ERROR_OBJECT_MISSING` when no destination is present
- Follows existing code patterns for consistency

**Files Modified:**
- `src/vanillapdf/semantics/objects/outline.h` - Added method declaration
- `src/vanillapdf/semantics/objects/outline.cpp` - Added implementation + destinations include
- `include/vanillapdf/semantics/c_outline.h` - Added C API declaration
- `src/vanillapdf/implementation/semantics/c_outline.cpp` - Added C API implementation

## Remaining Work

### 🚧 Phase 2: Actions System (Medium Priority)

**Status:** Not Started

**Scope:** Implement PDF action dictionaries for interactive outline items

**Required Components:**
1. **Action Base Classes** (New)
   - `ActionBase` - Base class for all action types
   - `GoToAction` - Navigate to specific page/destination
   - `URIAction` - Open external URI
   - `GoToRAction` - Navigate to remote document
   - `LaunchAction` - Launch external application
   - `NamedAction` - Execute named action (NextPage, PrevPage, etc.)

2. **C++ API** (New file: `src/vanillapdf/semantics/objects/actions.h`)
   ```cpp
   class ActionBase : public HighLevelObject<syntax::DictionaryObjectPtr> {
   public:
       enum class Type {
           Undefined = 0,
           GoTo,
           URI,
           GoToR,
           Launch,
           Named
       };
       virtual Type GetActionType() const noexcept = 0;
   };

   class GoToAction : public ActionBase {
   public:
       DestinationPtr GetDestination() const;
   };
   ```

3. **C API** (New file: `include/vanillapdf/semantics/c_actions.h`)
   ```c
   typedef enum {
       ActionType_Undefined = 0,
       ActionType_GoTo,
       ActionType_URI,
       // ...
   } ActionType;

   VANILLAPDF_API error_type CALLING_CONVENTION Action_GetActionType(ActionHandle* handle, ActionType* result);
   ```

4. **Integration with Outlines**
   - Uncomment `OutlineItem_GetActions()` in `c_outline.h:208`
   - Implement `OutlineItem::Action()` method
   - Handle PDF name constant `A` (Action)

**Estimated Effort:** 3-4 days

### 🔮 Phase 3: StructureElement Support (Complex)

**Status:** Not Started

**Scope:** Add PDF structure element support for accessibility

**Required Research:**
- PDF structure element hierarchy requirements
- Tagged PDF specifications (PDF 32000-1:2008 Section 14.7)
- Relationship with outline items

**Required Components:**
1. **Structure Element Classes** (New)
   - `StructureElementBase` - Base structure element
   - `StructureElementDict` - Dictionary-based structure element
   - `StructureTree` - Document structure tree

2. **Integration Points**
   - Uncomment `OutlineItem_GetStructureElement()` in `c_outline.h:209`
   - Implement `OutlineItem::StructureElement()` method
   - Handle PDF name constant `SE` (StructureElement)

**Dependencies:** Requires understanding of PDF accessibility features

**Estimated Effort:** 1-2 weeks (includes research)

## Implementation Guidelines

### Code Patterns to Follow

1. **C++ Class Design:**
   ```cpp
   class NewClass : public HighLevelObject<syntax::DictionaryObjectPtr> {
   public:
       explicit NewClass(syntax::DictionaryObjectPtr root);
       // Methods that may not exist return bool + output parameter
       bool OptionalProperty(OutputTypePtr& result) const;
       // Required properties return direct pointers
       syntax::StringObjectPtr RequiredProperty() const;
   };
   ```

2. **C API Pattern:**
   ```c
   VANILLAPDF_API error_type CALLING_CONVENTION Class_Method(ClassHandle* handle, ResultHandle** result)
   {
       Class* obj = reinterpret_cast<Class*>(handle);
       RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
       RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

       try {
           OutputTypePtr direct;
           auto contains = obj->Method(direct);
           if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
           auto ptr = direct.AddRefGet();
           *result = reinterpret_cast<ResultHandle*>(ptr);
           return VANILLAPDF_ERROR_SUCCESS;
       } CATCH_VANILLAPDF_EXCEPTIONS
   }
   ```

### Dependencies and Name Constants

**Existing PDF Name Constants** (in `syntax/utils/name_constants.h`):
- `Dest` - For destinations (already used)
- `A` - For actions (ready to use)
- Need to verify: `SE` for structure elements

## Success Criteria

### Phase 2 (Actions) Complete When:
- [ ] All action types implemented with C++ and C APIs
- [ ] `OutlineItem_GetAction()` function works correctly
- [ ] Actions can be retrieved from outline items
- [ ] GoTo actions properly integrate with destinations
- [ ] URI actions provide accessible URI strings
- [ ] Tests pass for action functionality

### Phase 3 (StructureElement) Complete When:
- [ ] Structure element hierarchy implemented
- [ ] `OutlineItem_GetStructureElement()` function works
- [ ] Structure elements accessible via C and C++ APIs
- [ ] Integration with accessibility features
- [ ] Documentation for tagged PDF support

## Timeline Estimate

- **Phase 2 (Actions):** 1-2 weeks
- **Phase 3 (StructureElement):** 2-3 weeks (including research)
- **Total:** 1-2 months for complete outline feature set

---

**Status:** Phase 1 Complete, Phase 2 and 3 Planned
**Last Updated:** 2025-09-27