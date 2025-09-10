#include "vanillapdf/api/c_pdf_data_api.h"
#include "vanillapdf/semantics/c_document.h"
#include "vanillapdf/semantics/c_catalog.h"
#include "vanillapdf/semantics/c_page_tree.h"
#include "vanillapdf/semantics/c_page_object.h"
#include "vanillapdf/semantics/c_fields.h"
#include "vanillapdf/semantics/c_interactive_forms.h"
#include "vanillapdf/syntax/c_dictionary_object.h"
#include "vanillapdf/syntax/c_array_object.h"
#include "vanillapdf/syntax/c_name_object.h"
#include "vanillapdf/syntax/c_string_object.h"
#include "vanillapdf/syntax/c_integer_object.h"
#include "vanillapdf/syntax/c_real_object.h"
#include "vanillapdf/utils/c_result_codes.h"
#include <vector>
#include <memory>
#include <string>
#include <cstring>

// Internal implementation structures
namespace {

struct SignatureInfoImpl {
    std::string field_name;
    std::string signer_name;
    DateHandle* signing_time;
    std::string reason;
    std::string location;
    std::string contact_info;
    SignatureStatus verification_status;
    std::string verification_message;
    std::string certificate_chain;
    size_type page_number;
    boolean_type document_modified;
    DigitalSignatureHandle* digital_signature;
    
    SignatureInfoImpl() : signing_time(nullptr), verification_status(SignatureStatus_Unknown), 
                          page_number(0), document_modified(VANILLAPDF_FALSE), 
                          digital_signature(nullptr) {}
    
    ~SignatureInfoImpl() {
        if (signing_time) Date_Release(signing_time);
        if (digital_signature) DigitalSignature_Release(digital_signature);
    }
};

struct SignatureListImpl {
    std::vector<SignatureInfoHandle*> signatures;
    
    ~SignatureListImpl() {
        for (auto sig : signatures) {
            SignatureInfo_Release(sig);
        }
    }
};

struct BookmarkInfoImpl {
    std::string title;
    size_type level;
    boolean_type is_open;
    BookmarkDestinationHandle* destination;
    BookmarkListHandle* children;
    real_type red, green, blue;
    boolean_type is_italic;
    boolean_type is_bold;
    OutlineItemHandle* outline_item;
    
    BookmarkInfoImpl() : level(0), is_open(VANILLAPDF_FALSE), destination(nullptr), 
                         children(nullptr), red(0), green(0), blue(0),
                         is_italic(VANILLAPDF_FALSE), is_bold(VANILLAPDF_FALSE),
                         outline_item(nullptr) {}
    
    ~BookmarkInfoImpl() {
        if (destination) BookmarkDestination_Release(destination);
        if (children) BookmarkList_Release(children);
        if (outline_item) OutlineItem_Release(outline_item);
    }
};

struct BookmarkListImpl {
    std::vector<BookmarkInfoHandle*> bookmarks;
    
    ~BookmarkListImpl() {
        for (auto bookmark : bookmarks) {
            BookmarkInfo_Release(bookmark);
        }
    }
};

struct BookmarkDestinationImpl {
    size_type page_number;
    DestinationType type;
    real_type x, y, zoom;
    real_type left, bottom, right, top;
    
    BookmarkDestinationImpl() : page_number(0), type(DestinationType_Undefined),
                                x(0), y(0), zoom(1.0),
                                left(0), bottom(0), right(0), top(0) {}
};

struct CommentInfoImpl {
    AnnotationType type;
    std::string content;
    std::string subject;
    std::string author;
    DateHandle* creation_date;
    DateHandle* modification_date;
    size_type page_number;
    real_type left, bottom, right, top;
    real_type red, green, blue;
    real_type opacity;
    boolean_type is_reply;
    CommentInfoHandle* parent_comment;
    CommentListHandle* replies;
    AnnotationHandle* annotation;
    
    CommentInfoImpl() : type(AnnotationType_Undefined), creation_date(nullptr),
                        modification_date(nullptr), page_number(0),
                        left(0), bottom(0), right(0), top(0),
                        red(0), green(0), blue(0), opacity(1.0),
                        is_reply(VANILLAPDF_FALSE), parent_comment(nullptr),
                        replies(nullptr), annotation(nullptr) {}
    
    ~CommentInfoImpl() {
        if (creation_date) Date_Release(creation_date);
        if (modification_date) Date_Release(modification_date);
        if (parent_comment) CommentInfo_Release(parent_comment);
        if (replies) CommentList_Release(replies);
        if (annotation) Annotation_Release(annotation);
    }
};

struct CommentListImpl {
    std::vector<CommentInfoHandle*> comments;
    
    ~CommentListImpl() {
        for (auto comment : comments) {
            CommentInfo_Release(comment);
        }
    }
};

// Helper functions
StringObjectHandle* CreateStringObject(const std::string& str) {
    if (str.empty()) return nullptr;
    
    // This is a simplified version - in real implementation, we'd need proper string object creation
    // For now, returning nullptr as placeholder
    return nullptr;
}

error_type ExtractSignatureInfo(SignatureFieldHandle* field, SignatureInfoImpl* info) {
    if (!field || !info) return error_type_invalid_arguments;
    
    // Get the digital signature
    error_type err = SignatureField_GetSignature(field, &info->digital_signature);
    if (err != error_type_success || !info->digital_signature) {
        return err;
    }
    
    // Extract signature details
    StringObjectHandle* str = nullptr;
    
    // Get signer name
    err = DigitalSignature_GetName(info->digital_signature, &str);
    if (err == error_type_success && str) {
        // Extract string value - simplified
        info->signer_name = "Signer Name"; // Placeholder
        StringObject_Release(str);
    }
    
    // Get reason
    err = DigitalSignature_GetReason(info->digital_signature, &str);
    if (err == error_type_success && str) {
        info->reason = "Signing Reason"; // Placeholder
        StringObject_Release(str);
    }
    
    // Get location
    err = DigitalSignature_GetLocation(info->digital_signature, &str);
    if (err == error_type_success && str) {
        info->location = "Signing Location"; // Placeholder
        StringObject_Release(str);
    }
    
    // Get contact info
    err = DigitalSignature_GetContactInfo(info->digital_signature, &str);
    if (err == error_type_success && str) {
        info->contact_info = "Contact Info"; // Placeholder
        StringObject_Release(str);
    }
    
    // Get signing time
    err = DigitalSignature_GetSigningTime(info->digital_signature, &info->signing_time);
    
    // Set verification status - simplified for now
    info->verification_status = SignatureStatus_NotVerified;
    info->verification_message = "Signature verification not yet implemented";
    info->document_modified = VANILLAPDF_FALSE;
    
    return error_type_success;
}

error_type ExtractBookmarkInfo(OutlineItemHandle* item, size_type level, BookmarkInfoImpl* info) {
    if (!item || !info) return error_type_invalid_arguments;
    
    info->level = level;
    info->outline_item = item;
    
    // Get title
    StringObjectHandle* title = nullptr;
    error_type err = OutlineItem_GetTitle(item, &title);
    if (err == error_type_success && title) {
        info->title = "Bookmark Title"; // Placeholder
        StringObject_Release(title);
    }
    
    // Get color
    OutlineItemColorHandle* color = nullptr;
    err = OutlineItem_GetColor(item, &color);
    if (err == error_type_success && color) {
        IntegerObjectHandle* val = nullptr;
        if (OutlineItemColor_GetRed(color, &val) == error_type_success && val) {
            // Convert to real value - simplified
            info->red = 0.0;
            IntegerObject_Release(val);
        }
        if (OutlineItemColor_GetGreen(color, &val) == error_type_success && val) {
            info->green = 0.0;
            IntegerObject_Release(val);
        }
        if (OutlineItemColor_GetBlue(color, &val) == error_type_success && val) {
            info->blue = 0.0;
            IntegerObject_Release(val);
        }
        OutlineItemColor_Release(color);
    }
    
    // Get flags
    OutlineItemFlagsHandle* flags = nullptr;
    err = OutlineItem_GetFlags(item, &flags);
    if (err == error_type_success && flags) {
        OutlineItemFlags_IsItalic(flags, &info->is_italic);
        OutlineItemFlags_IsBold(flags, &info->is_bold);
        OutlineItemFlags_Release(flags);
    }
    
    // Check if open
    IntegerObjectHandle* count = nullptr;
    err = OutlineItem_GetCount(item, &count);
    if (err == error_type_success && count) {
        integer_type count_val = 0;
        IntegerObject_GetValue(count, &count_val);
        info->is_open = (count_val > 0) ? VANILLAPDF_TRUE : VANILLAPDF_FALSE;
        IntegerObject_Release(count);
    }
    
    return error_type_success;
}

void CollectBookmarksRecursive(OutlineItemHandle* item, size_type level, std::vector<BookmarkInfoHandle*>& bookmarks) {
    if (!item) return;
    
    OutlineItemHandle* current = item;
    while (current) {
        auto info = new BookmarkInfoImpl();
        if (ExtractBookmarkInfo(current, level, info) == error_type_success) {
            bookmarks.push_back(reinterpret_cast<BookmarkInfoHandle*>(info));
            
            // Process children
            OutlineItemHandle* first_child = nullptr;
            if (OutlineItem_GetFirst(current, &first_child) == error_type_success && first_child) {
                CollectBookmarksRecursive(first_child, level + 1, bookmarks);
            }
        } else {
            delete info;
        }
        
        // Move to next sibling
        OutlineItemHandle* next = nullptr;
        if (OutlineItem_GetNext(current, &next) != error_type_success || !next) {
            break;
        }
        current = next;
    }
}

} // anonymous namespace

// SignatureInfo API Implementation
extern "C" {

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetFieldName(SignatureInfoHandle* handle, StringObjectHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = CreateStringObject(impl->field_name);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetSignerName(SignatureInfoHandle* handle, StringObjectHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = CreateStringObject(impl->signer_name);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetSigningTime(SignatureInfoHandle* handle, DateHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = impl->signing_time;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetReason(SignatureInfoHandle* handle, StringObjectHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = CreateStringObject(impl->reason);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetLocation(SignatureInfoHandle* handle, StringObjectHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = CreateStringObject(impl->location);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetContactInfo(SignatureInfoHandle* handle, StringObjectHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = CreateStringObject(impl->contact_info);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetVerificationStatus(SignatureInfoHandle* handle, SignatureStatus* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = impl->verification_status;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetVerificationMessage(SignatureInfoHandle* handle, StringObjectHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = CreateStringObject(impl->verification_message);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetCertificateChain(SignatureInfoHandle* handle, StringObjectHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = CreateStringObject(impl->certificate_chain);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetPageNumber(SignatureInfoHandle* handle, size_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = impl->page_number;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_IsDocumentModified(SignatureInfoHandle* handle, boolean_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = impl->document_modified;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetDigitalSignature(SignatureInfoHandle* handle, DigitalSignatureHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    *result = impl->digital_signature;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_Release(SignatureInfoHandle* handle) {
    if (!handle) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureInfoImpl*>(handle);
    delete impl;
    return error_type_success;
}

// SignatureList API Implementation
VANILLAPDF_API error_type CALLING_CONVENTION SignatureList_GetCount(SignatureListHandle* handle, size_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureListImpl*>(handle);
    *result = impl->signatures.size();
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureList_GetAt(SignatureListHandle* handle, size_type index, SignatureInfoHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureListImpl*>(handle);
    if (index >= impl->signatures.size()) return error_type_index_out_of_range;
    *result = impl->signatures[index];
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureList_Release(SignatureListHandle* handle) {
    if (!handle) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<SignatureListImpl*>(handle);
    delete impl;
    return error_type_success;
}

// BookmarkInfo API Implementation
VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetTitle(BookmarkInfoHandle* handle, StringObjectHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkInfoImpl*>(handle);
    *result = CreateStringObject(impl->title);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetLevel(BookmarkInfoHandle* handle, size_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkInfoImpl*>(handle);
    *result = impl->level;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_IsOpen(BookmarkInfoHandle* handle, boolean_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkInfoImpl*>(handle);
    *result = impl->is_open;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetDestination(BookmarkInfoHandle* handle, BookmarkDestinationHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkInfoImpl*>(handle);
    *result = impl->destination;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetChildren(BookmarkInfoHandle* handle, BookmarkListHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkInfoImpl*>(handle);
    *result = impl->children;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetColor(BookmarkInfoHandle* handle, 
                                                                   real_type* red, 
                                                                   real_type* green, 
                                                                   real_type* blue) {
    if (!handle || !red || !green || !blue) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkInfoImpl*>(handle);
    *red = impl->red;
    *green = impl->green;
    *blue = impl->blue;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_IsItalic(BookmarkInfoHandle* handle, boolean_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkInfoImpl*>(handle);
    *result = impl->is_italic;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_IsBold(BookmarkInfoHandle* handle, boolean_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkInfoImpl*>(handle);
    *result = impl->is_bold;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetOutlineItem(BookmarkInfoHandle* handle, OutlineItemHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkInfoImpl*>(handle);
    *result = impl->outline_item;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_Release(BookmarkInfoHandle* handle) {
    if (!handle) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkInfoImpl*>(handle);
    delete impl;
    return error_type_success;
}

// BookmarkDestination API Implementation
VANILLAPDF_API error_type CALLING_CONVENTION BookmarkDestination_GetPageNumber(BookmarkDestinationHandle* handle, size_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkDestinationImpl*>(handle);
    *result = impl->page_number;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkDestination_GetType(BookmarkDestinationHandle* handle, DestinationType* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkDestinationImpl*>(handle);
    *result = impl->type;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkDestination_GetPosition(BookmarkDestinationHandle* handle,
                                                                             real_type* x,
                                                                             real_type* y,
                                                                             real_type* zoom) {
    if (!handle) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkDestinationImpl*>(handle);
    if (x) *x = impl->x;
    if (y) *y = impl->y;
    if (zoom) *zoom = impl->zoom;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkDestination_GetRectangle(BookmarkDestinationHandle* handle,
                                                                              real_type* left,
                                                                              real_type* bottom,
                                                                              real_type* right,
                                                                              real_type* top) {
    if (!handle) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkDestinationImpl*>(handle);
    if (left) *left = impl->left;
    if (bottom) *bottom = impl->bottom;
    if (right) *right = impl->right;
    if (top) *top = impl->top;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkDestination_Release(BookmarkDestinationHandle* handle) {
    if (!handle) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkDestinationImpl*>(handle);
    delete impl;
    return error_type_success;
}

// BookmarkList API Implementation
VANILLAPDF_API error_type CALLING_CONVENTION BookmarkList_GetCount(BookmarkListHandle* handle, size_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkListImpl*>(handle);
    *result = impl->bookmarks.size();
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkList_GetAt(BookmarkListHandle* handle, size_type index, BookmarkInfoHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkListImpl*>(handle);
    if (index >= impl->bookmarks.size()) return error_type_index_out_of_range;
    *result = impl->bookmarks[index];
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkList_Release(BookmarkListHandle* handle) {
    if (!handle) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<BookmarkListImpl*>(handle);
    delete impl;
    return error_type_success;
}

// CommentInfo API Implementation
VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetType(CommentInfoHandle* handle, AnnotationType* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = impl->type;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetContent(CommentInfoHandle* handle, StringObjectHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = CreateStringObject(impl->content);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetSubject(CommentInfoHandle* handle, StringObjectHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = CreateStringObject(impl->subject);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetAuthor(CommentInfoHandle* handle, StringObjectHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = CreateStringObject(impl->author);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetCreationDate(CommentInfoHandle* handle, DateHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = impl->creation_date;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetModificationDate(CommentInfoHandle* handle, DateHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = impl->modification_date;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetPageNumber(CommentInfoHandle* handle, size_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = impl->page_number;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetRectangle(CommentInfoHandle* handle,
                                                                      real_type* left,
                                                                      real_type* bottom,
                                                                      real_type* right,
                                                                      real_type* top) {
    if (!handle) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    if (left) *left = impl->left;
    if (bottom) *bottom = impl->bottom;
    if (right) *right = impl->right;
    if (top) *top = impl->top;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetColor(CommentInfoHandle* handle,
                                                                  real_type* red,
                                                                  real_type* green,
                                                                  real_type* blue) {
    if (!handle) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    if (red) *red = impl->red;
    if (green) *green = impl->green;
    if (blue) *blue = impl->blue;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetOpacity(CommentInfoHandle* handle, real_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = impl->opacity;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_IsReply(CommentInfoHandle* handle, boolean_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = impl->is_reply;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetParentComment(CommentInfoHandle* handle, CommentInfoHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = impl->parent_comment;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetReplies(CommentInfoHandle* handle, CommentListHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = impl->replies;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetAnnotation(CommentInfoHandle* handle, AnnotationHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    *result = impl->annotation;
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_Release(CommentInfoHandle* handle) {
    if (!handle) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentInfoImpl*>(handle);
    delete impl;
    return error_type_success;
}

// CommentList API Implementation
VANILLAPDF_API error_type CALLING_CONVENTION CommentList_GetCount(CommentListHandle* handle, size_type* result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentListImpl*>(handle);
    *result = impl->comments.size();
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentList_GetAt(CommentListHandle* handle, size_type index, CommentInfoHandle** result) {
    if (!handle || !result) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentListImpl*>(handle);
    if (index >= impl->comments.size()) return error_type_index_out_of_range;
    *result = impl->comments[index];
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION CommentList_Release(CommentListHandle* handle) {
    if (!handle) return error_type_invalid_arguments;
    auto* impl = reinterpret_cast<CommentListImpl*>(handle);
    delete impl;
    return error_type_success;
}

// Document-level API Functions
VANILLAPDF_API error_type CALLING_CONVENTION Document_GetSignatures(DocumentHandle* document, SignatureListHandle** result) {
    if (!document || !result) return error_type_invalid_arguments;
    
    auto* list_impl = new SignatureListImpl();
    
    // Get the catalog
    CatalogHandle* catalog = nullptr;
    error_type err = Document_GetCatalog(document, &catalog);
    if (err != error_type_success || !catalog) {
        delete list_impl;
        return err;
    }
    
    // Get interactive form
    InteractiveFormHandle* form = nullptr;
    err = Catalog_GetInteractiveForm(catalog, &form);
    if (err == error_type_success && form) {
        // Get fields
        FieldCollectionHandle* fields = nullptr;
        err = InteractiveForm_GetFields(form, &fields);
        if (err == error_type_success && fields) {
            size_type count = 0;
            FieldCollection_GetSize(fields, &count);
            
            for (size_type i = 0; i < count; ++i) {
                FieldHandle* field = nullptr;
                if (FieldCollection_At(fields, i, &field) == error_type_success && field) {
                    FieldType type;
                    if (Field_GetFieldType(field, &type) == error_type_success && 
                        type == FieldType_Signature) {
                        
                        SignatureFieldHandle* sig_field = nullptr;
                        if (SignatureField_FromField(field, &sig_field) == error_type_success && sig_field) {
                            auto* sig_info = new SignatureInfoImpl();
                            sig_info->page_number = i; // Simplified - would need proper page lookup
                            
                            if (ExtractSignatureInfo(sig_field, sig_info) == error_type_success) {
                                list_impl->signatures.push_back(reinterpret_cast<SignatureInfoHandle*>(sig_info));
                            } else {
                                delete sig_info;
                            }
                            
                            SignatureField_Release(sig_field);
                        }
                    }
                    Field_Release(field);
                }
            }
            FieldCollection_Release(fields);
        }
        InteractiveForm_Release(form);
    }
    
    Catalog_Release(catalog);
    *result = reinterpret_cast<SignatureListHandle*>(list_impl);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_VerifySignatures(DocumentHandle* document, boolean_type* all_valid) {
    if (!document || !all_valid) return error_type_invalid_arguments;
    
    SignatureListHandle* signatures = nullptr;
    error_type err = Document_GetSignatures(document, &signatures);
    if (err != error_type_success) {
        return err;
    }
    
    size_type count = 0;
    SignatureList_GetCount(signatures, &count);
    
    *all_valid = VANILLAPDF_TRUE;
    for (size_type i = 0; i < count; ++i) {
        SignatureInfoHandle* sig = nullptr;
        if (SignatureList_GetAt(signatures, i, &sig) == error_type_success && sig) {
            SignatureStatus status;
            SignatureInfo_GetVerificationStatus(sig, &status);
            if (status != SignatureStatus_Valid) {
                *all_valid = VANILLAPDF_FALSE;
                break;
            }
        }
    }
    
    SignatureList_Release(signatures);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_GetBookmarks(DocumentHandle* document, BookmarkListHandle** result) {
    if (!document || !result) return error_type_invalid_arguments;
    
    auto* list_impl = new BookmarkListImpl();
    
    // Get the catalog
    CatalogHandle* catalog = nullptr;
    error_type err = Document_GetCatalog(document, &catalog);
    if (err != error_type_success || !catalog) {
        delete list_impl;
        return err;
    }
    
    // Get outlines
    OutlineHandle* outline = nullptr;
    err = Catalog_GetOutlines(catalog, &outline);
    if (err == error_type_success && outline) {
        OutlineItemHandle* first = nullptr;
        err = Outline_GetFirst(outline, &first);
        if (err == error_type_success && first) {
            CollectBookmarksRecursive(first, 0, list_impl->bookmarks);
        }
        Outline_Release(outline);
    }
    
    Catalog_Release(catalog);
    *result = reinterpret_cast<BookmarkListHandle*>(list_impl);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_GetComments(DocumentHandle* document, CommentListHandle** result) {
    if (!document || !result) return error_type_invalid_arguments;
    
    auto* list_impl = new CommentListImpl();
    
    // Get page tree
    PageTreeHandle* page_tree = nullptr;
    error_type err = Document_GetPages(document, &page_tree);
    if (err != error_type_success || !page_tree) {
        delete list_impl;
        return err;
    }
    
    size_type page_count = 0;
    PageTree_GetCount(page_tree, &page_count);
    
    for (size_type page_idx = 0; page_idx < page_count; ++page_idx) {
        PageObjectHandle* page = nullptr;
        if (PageTree_GetPage(page_tree, page_idx, &page) == error_type_success && page) {
            PageAnnotationsHandle* annotations = nullptr;
            if (PageObject_GetAnnotations(page, &annotations) == error_type_success && annotations) {
                size_type annot_count = 0;
                PageAnnotations_GetSize(annotations, &annot_count);
                
                for (size_type annot_idx = 0; annot_idx < annot_count; ++annot_idx) {
                    AnnotationHandle* annotation = nullptr;
                    if (PageAnnotations_At(annotations, annot_idx, &annotation) == error_type_success && annotation) {
                        AnnotationType type;
                        if (Annotation_GetAnnotationType(annotation, &type) == error_type_success) {
                            // Filter for comment-like annotations
                            if (type == AnnotationType_Text || type == AnnotationType_FreeText ||
                                type == AnnotationType_Highlight || type == AnnotationType_Underline ||
                                type == AnnotationType_StrikeOut || type == AnnotationType_Squiggly ||
                                type == AnnotationType_Caret || type == AnnotationType_Ink ||
                                type == AnnotationType_Popup) {
                                
                                auto* comment_info = new CommentInfoImpl();
                                comment_info->type = type;
                                comment_info->page_number = page_idx;
                                comment_info->annotation = annotation;
                                
                                // Extract more details from annotation dictionary
                                // This would require additional API calls to get annotation properties
                                
                                list_impl->comments.push_back(reinterpret_cast<CommentInfoHandle*>(comment_info));
                            } else {
                                Annotation_Release(annotation);
                            }
                        } else {
                            Annotation_Release(annotation);
                        }
                    }
                }
                PageAnnotations_Release(annotations);
            }
            PageObject_Release(page);
        }
    }
    
    PageTree_Release(page_tree);
    *result = reinterpret_cast<CommentListHandle*>(list_impl);
    return error_type_success;
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_GetPageComments(DocumentHandle* document, size_type page_number, CommentListHandle** result) {
    if (!document || !result) return error_type_invalid_arguments;
    
    auto* list_impl = new CommentListImpl();
    
    // Get page tree
    PageTreeHandle* page_tree = nullptr;
    error_type err = Document_GetPages(document, &page_tree);
    if (err != error_type_success || !page_tree) {
        delete list_impl;
        return err;
    }
    
    PageObjectHandle* page = nullptr;
    if (PageTree_GetPage(page_tree, page_number, &page) == error_type_success && page) {
        PageAnnotationsHandle* annotations = nullptr;
        if (PageObject_GetAnnotations(page, &annotations) == error_type_success && annotations) {
            size_type annot_count = 0;
            PageAnnotations_GetSize(annotations, &annot_count);
            
            for (size_type annot_idx = 0; annot_idx < annot_count; ++annot_idx) {
                AnnotationHandle* annotation = nullptr;
                if (PageAnnotations_At(annotations, annot_idx, &annotation) == error_type_success && annotation) {
                    AnnotationType type;
                    if (Annotation_GetAnnotationType(annotation, &type) == error_type_success) {
                        // Filter for comment-like annotations
                        if (type == AnnotationType_Text || type == AnnotationType_FreeText ||
                            type == AnnotationType_Highlight || type == AnnotationType_Underline ||
                            type == AnnotationType_StrikeOut || type == AnnotationType_Squiggly ||
                            type == AnnotationType_Caret || type == AnnotationType_Ink ||
                            type == AnnotationType_Popup) {
                            
                            auto* comment_info = new CommentInfoImpl();
                            comment_info->type = type;
                            comment_info->page_number = page_number;
                            comment_info->annotation = annotation;
                            
                            list_impl->comments.push_back(reinterpret_cast<CommentInfoHandle*>(comment_info));
                        } else {
                            Annotation_Release(annotation);
                        }
                    } else {
                        Annotation_Release(annotation);
                    }
                }
            }
            PageAnnotations_Release(annotations);
        }
        PageObject_Release(page);
    }
    
    PageTree_Release(page_tree);
    *result = reinterpret_cast<CommentListHandle*>(list_impl);
    return error_type_success;
}

} // extern "C"