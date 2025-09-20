# VanillaPDF Bookmarks API Documentation

## Overview

The VanillaPDF Bookmarks API provides a comprehensive interface for accessing and navigating PDF bookmarks (also known as outlines). This API is designed to be used by the vanillapdf-desktop application to display bookmarks in the UI and enable document navigation.

## Key Features

- **Hierarchical bookmark traversal** with level information
- **Destination support** for jumping to specific locations in the PDF
- **Style and color information** for bookmark display
- **Two access patterns**: Iterator-based and flat list
- **Consistent with existing VanillaPDF API patterns**

## API Components

### 1. Core Outline API (Existing, Enhanced)

Located in: `include/vanillapdf/semantics/c_outline.h`

#### Key Enhancement:
```c
// Get the destination for a bookmark/outline item
VANILLAPDF_API error_type OutlineItem_GetDestination(
    OutlineItemHandle* handle, 
    DestinationHandle** result
);
```

### 2. High-Level Bookmarks API (New)

Located in: `include/vanillapdf/semantics/c_bookmarks.h`

#### BookmarksIterator
Provides efficient traversal of the bookmark tree:

```c
// Create iterator from document outline
BookmarksIterator_Create(OutlineHandle* outline, BookmarksIteratorHandle** result);

// Check if more bookmarks available
BookmarksIterator_HasNext(BookmarksIteratorHandle* handle, boolean_type* result);

// Get next bookmark
BookmarksIterator_GetNext(BookmarksIteratorHandle* handle, BookmarkItemHandle** result);

// Get total count of bookmarks
BookmarksIterator_GetCount(BookmarksIteratorHandle* handle, size_type* result);

// Reset iterator to beginning
BookmarksIterator_Reset(BookmarksIteratorHandle* handle);
```

#### BookmarkItem
High-level wrapper for easy access to bookmark properties:

```c
// Get bookmark title/text
BookmarkItem_GetTitle(BookmarkItemHandle* handle, StringObjectHandle** result);

// Get bookmark destination
BookmarkItem_GetDestination(BookmarkItemHandle* handle, DestinationHandle** result);

// Get hierarchical level (0 = top-level)
BookmarkItem_GetLevel(BookmarkItemHandle* handle, size_type* result);

// Check if bookmark has children
BookmarkItem_HasChildren(BookmarkItemHandle* handle, boolean_type* result);

// Check if bookmark is expanded by default
BookmarkItem_IsOpen(BookmarkItemHandle* handle, boolean_type* result);

// Get style information
BookmarkItem_GetColor(BookmarkItemHandle* handle, OutlineItemColorHandle** result);
BookmarkItem_GetFlags(BookmarkItemHandle* handle, OutlineItemFlagsHandle** result);
```

#### Convenience Functions
```c
// Get all bookmarks as a flat array
Bookmarks_GetAllItems(OutlineHandle* outline, BookmarkItemHandle*** items, size_type* count);

// Free the array
Bookmarks_FreeItemsArray(BookmarkItemHandle** items, size_type count);
```

### 3. Enhanced Destinations API

Located in: `include/vanillapdf/semantics/c_destinations.h`

#### New Functions:
```c
// Get destination type
Destination_GetType(DestinationHandle* handle, DestinationType* result);

// Get destination parameters
Destination_GetLeft(DestinationHandle* handle, RealObjectHandle** result);
Destination_GetTop(DestinationHandle* handle, RealObjectHandle** result);
Destination_GetRight(DestinationHandle* handle, RealObjectHandle** result);
Destination_GetBottom(DestinationHandle* handle, RealObjectHandle** result);
Destination_GetZoom(DestinationHandle* handle, RealObjectHandle** result);
```

## Usage Example

### Basic Bookmark Iteration

```c
// 1. Get document catalog
CatalogHandle* catalog;
Document_GetCatalog(document, &catalog);

// 2. Get outline (bookmarks root)
OutlineHandle* outline;
Catalog_GetOutlines(catalog, &outline);

// 3. Create iterator
BookmarksIteratorHandle* iterator;
BookmarksIterator_Create(outline, &iterator);

// 4. Iterate through bookmarks
boolean_type has_next;
BookmarksIterator_HasNext(iterator, &has_next);

while (has_next) {
    BookmarkItemHandle* item;
    BookmarksIterator_GetNext(iterator, &item);
    
    // Get title
    StringObjectHandle* title;
    BookmarkItem_GetTitle(item, &title);
    
    // Get destination for navigation
    DestinationHandle* destination;
    BookmarkItem_GetDestination(item, &destination);
    
    if (destination) {
        // Get page number
        ObjectHandle* page;
        Destination_GetPageNumber(destination, &page);
        
        // Get destination type
        DestinationType type;
        Destination_GetType(destination, &type);
        
        // Handle navigation based on type...
        Destination_Release(destination);
    }
    
    StringObject_Release(title);
    BookmarkItem_Release(item);
    
    BookmarksIterator_HasNext(iterator, &has_next);
}

// 5. Cleanup
BookmarksIterator_Release(iterator);
Outline_Release(outline);
Catalog_Release(catalog);
```

### Using Flat List API

```c
// Get all bookmarks at once
BookmarkItemHandle** items;
size_type count;
Bookmarks_GetAllItems(outline, &items, &count);

// Process each bookmark
for (size_type i = 0; i < count; i++) {
    BookmarkItemHandle* item = items[i];
    // Process bookmark...
}

// Free the array
Bookmarks_FreeItemsArray(items, count);
```

## Integration with vanillapdf-desktop

The vanillapdf-desktop application can use this API to:

1. **Build bookmark tree UI**: Use the iterator to traverse bookmarks hierarchically, using `GetLevel()` for indentation
2. **Navigate on click**: Use `GetDestination()` to get the target location and jump to it
3. **Display bookmark properties**: Show bold/italic styles, colors, and expanded state
4. **Search bookmarks**: Iterate through all bookmarks to find matches
5. **Export bookmark list**: Generate a table of contents from the bookmark structure

## Key Benefits

1. **Consistent API Design**: Follows the same patterns as other VanillaPDF APIs
2. **Memory Safe**: Proper reference counting and cleanup functions
3. **Flexible Access**: Both iterator and array-based access patterns
4. **Complete Information**: Access to all bookmark properties including destinations
5. **Performance**: Efficient traversal without loading entire tree into memory

## Files Modified/Added

### New Files:
- `include/vanillapdf/semantics/c_bookmarks.h` - High-level bookmarks API
- `src/vanillapdf/semantics/objects/bookmarks.h` - C++ implementation header
- `src/vanillapdf/semantics/objects/bookmarks.cpp` - C++ implementation
- `src/vanillapdf/implementation/semantics/c_bookmarks.cpp` - C API implementation
- `examples/bookmarks_example.c` - Usage example

### Modified Files:
- `include/vanillapdf/semantics/c_outline.h` - Added destination support
- `include/vanillapdf/semantics/c_destinations.h` - Added type and parameter getters
- `src/vanillapdf/semantics/objects/outline.h` - Added destination method
- `src/vanillapdf/semantics/objects/outline.cpp` - Implemented destination method
- `src/vanillapdf/implementation/semantics/c_outline.cpp` - Implemented destination API
- `src/vanillapdf/implementation/semantics/c_destinations.cpp` - Implemented new getters
- `include/vanillapdf/c_handles.h` - Added new handle types
- `src/vanillapdf/CMakeLists.txt` - Added new source files
- `include/files.cmake` - Added new header file

## Building and Testing

The API is fully integrated into the VanillaPDF build system. After building the library, you can test the bookmarks API using the provided example:

```bash
# Build the library
cmake --build build

# Test with a PDF file
./build/examples/bookmarks_example path/to/document.pdf
```

## Next Steps for vanillapdf-desktop

1. Integrate the bookmarks API into the desktop application
2. Create a tree widget to display the bookmark hierarchy
3. Implement click handlers to navigate using destinations
4. Add bookmark search functionality
5. Support bookmark editing (if needed in future)