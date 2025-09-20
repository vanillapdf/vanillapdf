/**
 * Example demonstrating how to use the VanillaPDF Bookmarks API
 * 
 * This example shows how to:
 * 1. Open a PDF document
 * 2. Get the document's outline (bookmarks)
 * 3. Iterate through all bookmarks
 * 4. Access bookmark properties including destinations
 */

#include <stdio.h>
#include <stdlib.h>
#include "vanillapdf/c_vanillapdf_api.h"
#include "vanillapdf/semantics/c_catalog.h"
#include "vanillapdf/semantics/c_outline.h"
#include "vanillapdf/semantics/c_bookmarks.h"
#include "vanillapdf/semantics/c_destinations.h"
#include "vanillapdf/syntax/c_string_object.h"
#include "vanillapdf/syntax/c_integer_object.h"
#include "vanillapdf/syntax/c_real_object.h"

void print_indent(size_t level) {
    for (size_t i = 0; i < level; i++) {
        printf("  ");
    }
}

void print_destination_info(DestinationHandle* destination) {
    if (!destination) {
        printf(" (no destination)");
        return;
    }

    DestinationType type;
    if (Destination_GetType(destination, &type) == VANILLAPDF_ERROR_SUCCESS) {
        ObjectHandle* page_obj;
        if (Destination_GetPageNumber(destination, &page_obj) == VANILLAPDF_ERROR_SUCCESS && page_obj) {
            // Check if it's an integer (page index) or reference
            IntegerObjectHandle* page_index;
            if (IntegerObject_FromObject(page_obj, &page_index) == VANILLAPDF_ERROR_SUCCESS && page_index) {
                integer_type page_num;
                IntegerObject_GetIntegerValue(page_index, &page_num);
                printf(" -> Page %lld", (long long)page_num);
                IntegerObject_Release(page_index);
            }
            Object_Release(page_obj);
        }

        // Print destination type and parameters
        switch (type) {
            case DestinationType_XYZ: {
                printf(" [XYZ");
                RealObjectHandle* left, *top, *zoom;
                if (Destination_GetLeft(destination, &left) == VANILLAPDF_ERROR_SUCCESS && left) {
                    real_type val;
                    RealObject_GetRealValue(left, &val);
                    printf(" left:%.2f", val);
                    RealObject_Release(left);
                }
                if (Destination_GetTop(destination, &top) == VANILLAPDF_ERROR_SUCCESS && top) {
                    real_type val;
                    RealObject_GetRealValue(top, &val);
                    printf(" top:%.2f", val);
                    RealObject_Release(top);
                }
                if (Destination_GetZoom(destination, &zoom) == VANILLAPDF_ERROR_SUCCESS && zoom) {
                    real_type val;
                    RealObject_GetRealValue(zoom, &val);
                    printf(" zoom:%.2f", val);
                    RealObject_Release(zoom);
                }
                printf("]");
                break;
            }
            case DestinationType_Fit:
                printf(" [Fit]");
                break;
            case DestinationType_FitHorizontal:
                printf(" [FitH]");
                break;
            case DestinationType_FitVertical:
                printf(" [FitV]");
                break;
            case DestinationType_FitRectangle:
                printf(" [FitR]");
                break;
            default:
                printf(" [Type:%d]", type);
                break;
        }
    }
}

void process_bookmarks_with_iterator(DocumentHandle* document) {
    printf("\n=== Using Bookmarks Iterator API ===\n\n");

    // Get the catalog
    CatalogHandle* catalog;
    if (Document_GetCatalog(document, &catalog) != VANILLAPDF_ERROR_SUCCESS) {
        printf("Failed to get document catalog\n");
        return;
    }

    // Get the outline
    OutlineHandle* outline;
    error_type result = Catalog_GetOutlines(catalog, &outline);
    if (result != VANILLAPDF_ERROR_SUCCESS || !outline) {
        printf("No bookmarks found in this document\n");
        Catalog_Release(catalog);
        return;
    }

    // Create bookmarks iterator
    BookmarksIteratorHandle* iterator;
    if (BookmarksIterator_Create(outline, &iterator) != VANILLAPDF_ERROR_SUCCESS) {
        printf("Failed to create bookmarks iterator\n");
        Outline_Release(outline);
        Catalog_Release(catalog);
        return;
    }

    // Get total count
    size_type total_count;
    if (BookmarksIterator_GetCount(iterator, &total_count) == VANILLAPDF_ERROR_SUCCESS) {
        printf("Total bookmarks: %zu\n\n", total_count);
    }

    // Iterate through all bookmarks
    boolean_type has_next;
    BookmarksIterator_HasNext(iterator, &has_next);
    
    while (has_next) {
        BookmarkItemHandle* item;
        if (BookmarksIterator_GetNext(iterator, &item) == VANILLAPDF_ERROR_SUCCESS && item) {
            // Get level for indentation
            size_type level;
            BookmarkItem_GetLevel(item, &level);
            print_indent(level);

            // Get and print title
            StringObjectHandle* title;
            if (BookmarkItem_GetTitle(item, &title) == VANILLAPDF_ERROR_SUCCESS && title) {
                string_type title_str;
                size_type title_len;
                if (StringObject_GetValue(title, &title_str, &title_len) == VANILLAPDF_ERROR_SUCCESS) {
                    printf("%.*s", (int)title_len, title_str);
                }
                StringObject_Release(title);
            }

            // Get and print destination
            DestinationHandle* destination;
            if (BookmarkItem_GetDestination(item, &destination) == VANILLAPDF_ERROR_SUCCESS) {
                print_destination_info(destination);
                if (destination) {
                    Destination_Release(destination);
                }
            }

            // Check for children
            boolean_type has_children;
            if (BookmarkItem_HasChildren(item, &has_children) == VANILLAPDF_ERROR_SUCCESS && has_children) {
                printf(" [+]");
            }

            // Check if open
            boolean_type is_open;
            if (BookmarkItem_IsOpen(item, &is_open) == VANILLAPDF_ERROR_SUCCESS && is_open) {
                printf(" (expanded)");
            }

            // Check for style
            OutlineItemFlagsHandle* flags;
            if (BookmarkItem_GetFlags(item, &flags) == VANILLAPDF_ERROR_SUCCESS && flags) {
                boolean_type is_bold, is_italic;
                OutlineItemFlags_IsBold(flags, &is_bold);
                OutlineItemFlags_IsItalic(flags, &is_italic);
                if (is_bold) printf(" [bold]");
                if (is_italic) printf(" [italic]");
                OutlineItemFlags_Release(flags);
            }

            printf("\n");
            BookmarkItem_Release(item);
        }

        BookmarksIterator_HasNext(iterator, &has_next);
    }

    // Clean up
    BookmarksIterator_Release(iterator);
    Outline_Release(outline);
    Catalog_Release(catalog);
}

void process_bookmarks_flat_list(DocumentHandle* document) {
    printf("\n=== Using Flat List API ===\n\n");

    // Get the catalog
    CatalogHandle* catalog;
    if (Document_GetCatalog(document, &catalog) != VANILLAPDF_ERROR_SUCCESS) {
        printf("Failed to get document catalog\n");
        return;
    }

    // Get the outline
    OutlineHandle* outline;
    error_type result = Catalog_GetOutlines(catalog, &outline);
    if (result != VANILLAPDF_ERROR_SUCCESS || !outline) {
        printf("No bookmarks found in this document\n");
        Catalog_Release(catalog);
        return;
    }

    // Get all bookmarks as a flat list
    BookmarkItemHandle** items;
    size_type count;
    if (Bookmarks_GetAllItems(outline, &items, &count) == VANILLAPDF_ERROR_SUCCESS) {
        printf("Retrieved %zu bookmarks\n\n", count);

        for (size_type i = 0; i < count; i++) {
            BookmarkItemHandle* item = items[i];
            
            // Get level for indentation
            size_type level;
            BookmarkItem_GetLevel(item, &level);
            print_indent(level);

            // Get and print title
            StringObjectHandle* title;
            if (BookmarkItem_GetTitle(item, &title) == VANILLAPDF_ERROR_SUCCESS && title) {
                string_type title_str;
                size_type title_len;
                if (StringObject_GetValue(title, &title_str, &title_len) == VANILLAPDF_ERROR_SUCCESS) {
                    printf("%zu. %.*s", i + 1, (int)title_len, title_str);
                }
                StringObject_Release(title);
            }

            // Get and print destination
            DestinationHandle* destination;
            if (BookmarkItem_GetDestination(item, &destination) == VANILLAPDF_ERROR_SUCCESS) {
                print_destination_info(destination);
                if (destination) {
                    Destination_Release(destination);
                }
            }

            printf("\n");
        }

        // Free the items array
        Bookmarks_FreeItemsArray(items, count);
    }

    // Clean up
    Outline_Release(outline);
    Catalog_Release(catalog);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <pdf_file>\n", argv[0]);
        return 1;
    }

    // Initialize the library
    error_type init_result = VANILLAPDF_Init();
    if (init_result != VANILLAPDF_ERROR_SUCCESS) {
        printf("Failed to initialize VanillaPDF library\n");
        return 1;
    }

    // Open the document
    DocumentHandle* document;
    error_type open_result = Document_Open(argv[1], &document);
    if (open_result != VANILLAPDF_ERROR_SUCCESS) {
        printf("Failed to open PDF file: %s\n", argv[1]);
        VANILLAPDF_Finish();
        return 1;
    }

    printf("Successfully opened: %s\n", argv[1]);

    // Process bookmarks using iterator
    process_bookmarks_with_iterator(document);

    // Process bookmarks using flat list
    process_bookmarks_flat_list(document);

    // Clean up
    Document_Release(document);
    VANILLAPDF_Finish();

    return 0;
}