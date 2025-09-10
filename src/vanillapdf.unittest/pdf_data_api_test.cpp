#include "unittest.h"
#include "vanillapdf/api/c_pdf_data_api.h"
#include "vanillapdf/semantics/c_document.h"
#include "vanillapdf/semantics/c_catalog.h"
#include "vanillapdf/semantics/c_page_tree.h"
#include "vanillapdf/semantics/c_page_object.h"
#include "vanillapdf/semantics/c_interactive_forms.h"
#include "vanillapdf/semantics/c_fields.h"
#include "vanillapdf/syntax/c_dictionary_object.h"
#include "vanillapdf/syntax/c_array_object.h"
#include "vanillapdf/syntax/c_name_object.h"
#include "vanillapdf/syntax/c_string_object.h"
#include "vanillapdf/syntax/c_integer_object.h"
#include "vanillapdf/syntax/c_real_object.h"
#include "vanillapdf/utils/c_result_codes.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

class PDFDataAPITest : public ::testing::Test {
protected:
    DocumentHandle* document = nullptr;
    
    void SetUp() override {
        // Create a test document
        // In a real test, this would load a test PDF file
        Document_Create(&document);
    }
    
    void TearDown() override {
        if (document) {
            Document_Release(document);
            document = nullptr;
        }
    }
    
    // Helper to create a test PDF with signatures
    void CreateTestPDFWithSignatures() {
        // This would create a test PDF with signature fields
        // For now, it's a placeholder
    }
    
    // Helper to create a test PDF with bookmarks
    void CreateTestPDFWithBookmarks() {
        // This would create a test PDF with bookmarks/outlines
        // For now, it's a placeholder
    }
    
    // Helper to create a test PDF with comments
    void CreateTestPDFWithComments() {
        // This would create a test PDF with various annotations
        // For now, it's a placeholder
    }
};

// ===========================
// Signature API Tests
// ===========================

TEST_F(PDFDataAPITest, GetSignatures_EmptyDocument) {
    SignatureListHandle* signatures = nullptr;
    error_type result = Document_GetSignatures(document, &signatures);
    
    EXPECT_EQ(result, error_type_success);
    EXPECT_NE(signatures, nullptr);
    
    size_type count = 0;
    SignatureList_GetCount(signatures, &count);
    EXPECT_EQ(count, 0);
    
    SignatureList_Release(signatures);
}

TEST_F(PDFDataAPITest, GetSignatures_InvalidArguments) {
    SignatureListHandle* signatures = nullptr;
    
    // Test with null document
    error_type result = Document_GetSignatures(nullptr, &signatures);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test with null result pointer
    result = Document_GetSignatures(document, nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
}

TEST_F(PDFDataAPITest, SignatureInfo_GettersSetters) {
    // Create a mock signature info
    // In a real test, this would be obtained from a document
    SignatureInfoHandle* sig_info = nullptr;
    
    // Test field name getter
    StringObjectHandle* field_name = nullptr;
    error_type result = SignatureInfo_GetFieldName(sig_info, &field_name);
    // Note: This will fail with invalid arguments since sig_info is null
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test with null result pointer
    result = SignatureInfo_GetFieldName(sig_info, nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
}

TEST_F(PDFDataAPITest, SignatureVerification) {
    CreateTestPDFWithSignatures();
    
    boolean_type all_valid = VANILLAPDF_FALSE;
    error_type result = Document_VerifySignatures(document, &all_valid);
    
    EXPECT_EQ(result, error_type_success);
    // In a real test with actual signatures, we'd check the validity
}

TEST_F(PDFDataAPITest, SignatureList_Operations) {
    SignatureListHandle* signatures = nullptr;
    error_type result = Document_GetSignatures(document, &signatures);
    
    ASSERT_EQ(result, error_type_success);
    ASSERT_NE(signatures, nullptr);
    
    size_type count = 0;
    SignatureList_GetCount(signatures, &count);
    
    // Try to get signature at invalid index
    if (count == 0) {
        SignatureInfoHandle* sig = nullptr;
        result = SignatureList_GetAt(signatures, 0, &sig);
        EXPECT_EQ(result, error_type_index_out_of_range);
    }
    
    SignatureList_Release(signatures);
}

TEST_F(PDFDataAPITest, SignatureStatus_Values) {
    // Test that all signature status values are distinct
    EXPECT_NE(SignatureStatus_Unknown, SignatureStatus_Valid);
    EXPECT_NE(SignatureStatus_Valid, SignatureStatus_Invalid);
    EXPECT_NE(SignatureStatus_Invalid, SignatureStatus_NotVerified);
    EXPECT_NE(SignatureStatus_NotVerified, SignatureStatus_CertificateExpired);
    EXPECT_NE(SignatureStatus_CertificateExpired, SignatureStatus_CertificateRevoked);
    EXPECT_NE(SignatureStatus_CertificateRevoked, SignatureStatus_CertificateUntrusted);
    EXPECT_NE(SignatureStatus_CertificateUntrusted, SignatureStatus_DocumentModified);
}

// ===========================
// Bookmark API Tests
// ===========================

TEST_F(PDFDataAPITest, GetBookmarks_EmptyDocument) {
    BookmarkListHandle* bookmarks = nullptr;
    error_type result = Document_GetBookmarks(document, &bookmarks);
    
    EXPECT_EQ(result, error_type_success);
    EXPECT_NE(bookmarks, nullptr);
    
    size_type count = 0;
    BookmarkList_GetCount(bookmarks, &count);
    EXPECT_EQ(count, 0);
    
    BookmarkList_Release(bookmarks);
}

TEST_F(PDFDataAPITest, GetBookmarks_InvalidArguments) {
    BookmarkListHandle* bookmarks = nullptr;
    
    // Test with null document
    error_type result = Document_GetBookmarks(nullptr, &bookmarks);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test with null result pointer
    result = Document_GetBookmarks(document, nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
}

TEST_F(PDFDataAPITest, BookmarkInfo_Properties) {
    // In a real test, we'd get this from an actual document
    BookmarkInfoHandle* bookmark = nullptr;
    
    // Test title getter
    StringObjectHandle* title = nullptr;
    error_type result = BookmarkInfo_GetTitle(bookmark, &title);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test level getter
    size_type level = 0;
    result = BookmarkInfo_GetLevel(bookmark, &level);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test color getter
    real_type red = 0, green = 0, blue = 0;
    result = BookmarkInfo_GetColor(bookmark, &red, &green, &blue);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test style flags
    boolean_type is_italic = VANILLAPDF_FALSE;
    result = BookmarkInfo_IsItalic(bookmark, &is_italic);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    boolean_type is_bold = VANILLAPDF_FALSE;
    result = BookmarkInfo_IsBold(bookmark, &is_bold);
    EXPECT_EQ(result, error_type_invalid_arguments);
}

TEST_F(PDFDataAPITest, BookmarkDestination_Properties) {
    BookmarkDestinationHandle* destination = nullptr;
    
    // Test page number getter
    size_type page_num = 0;
    error_type result = BookmarkDestination_GetPageNumber(destination, &page_num);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test type getter
    DestinationType type = DestinationType_Undefined;
    result = BookmarkDestination_GetType(destination, &type);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test position getter
    real_type x = 0, y = 0, zoom = 0;
    result = BookmarkDestination_GetPosition(destination, &x, &y, &zoom);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test rectangle getter
    real_type left = 0, bottom = 0, right = 0, top = 0;
    result = BookmarkDestination_GetRectangle(destination, &left, &bottom, &right, &top);
    EXPECT_EQ(result, error_type_invalid_arguments);
}

TEST_F(PDFDataAPITest, BookmarkList_Operations) {
    BookmarkListHandle* bookmarks = nullptr;
    error_type result = Document_GetBookmarks(document, &bookmarks);
    
    ASSERT_EQ(result, error_type_success);
    ASSERT_NE(bookmarks, nullptr);
    
    size_type count = 0;
    BookmarkList_GetCount(bookmarks, &count);
    
    // Try to get bookmark at invalid index
    if (count == 0) {
        BookmarkInfoHandle* bookmark = nullptr;
        result = BookmarkList_GetAt(bookmarks, 0, &bookmark);
        EXPECT_EQ(result, error_type_index_out_of_range);
    }
    
    BookmarkList_Release(bookmarks);
}

TEST_F(PDFDataAPITest, DestinationType_Values) {
    // Test that all destination type values are distinct
    EXPECT_NE(DestinationType_Undefined, DestinationType_XYZ);
    EXPECT_NE(DestinationType_XYZ, DestinationType_Fit);
    EXPECT_NE(DestinationType_Fit, DestinationType_FitHorizontal);
    EXPECT_NE(DestinationType_FitHorizontal, DestinationType_FitVertical);
    EXPECT_NE(DestinationType_FitVertical, DestinationType_FitRectangle);
    EXPECT_NE(DestinationType_FitRectangle, DestinationType_FitBoundingBox);
    EXPECT_NE(DestinationType_FitBoundingBox, DestinationType_FitBoundingBoxHorizontal);
    EXPECT_NE(DestinationType_FitBoundingBoxHorizontal, DestinationType_FitBoundingBoxVertical);
}

// ===========================
// Comment API Tests
// ===========================

TEST_F(PDFDataAPITest, GetComments_EmptyDocument) {
    CommentListHandle* comments = nullptr;
    error_type result = Document_GetComments(document, &comments);
    
    EXPECT_EQ(result, error_type_success);
    EXPECT_NE(comments, nullptr);
    
    size_type count = 0;
    CommentList_GetCount(comments, &count);
    EXPECT_EQ(count, 0);
    
    CommentList_Release(comments);
}

TEST_F(PDFDataAPITest, GetPageComments_EmptyDocument) {
    CommentListHandle* comments = nullptr;
    error_type result = Document_GetPageComments(document, 0, &comments);
    
    // This might fail if there are no pages
    if (result == error_type_success) {
        EXPECT_NE(comments, nullptr);
        
        size_type count = 0;
        CommentList_GetCount(comments, &count);
        EXPECT_EQ(count, 0);
        
        CommentList_Release(comments);
    }
}

TEST_F(PDFDataAPITest, GetComments_InvalidArguments) {
    CommentListHandle* comments = nullptr;
    
    // Test with null document
    error_type result = Document_GetComments(nullptr, &comments);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test with null result pointer
    result = Document_GetComments(document, nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test page comments with null document
    result = Document_GetPageComments(nullptr, 0, &comments);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test page comments with null result pointer
    result = Document_GetPageComments(document, 0, nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
}

TEST_F(PDFDataAPITest, CommentInfo_Properties) {
    CommentInfoHandle* comment = nullptr;
    
    // Test type getter
    AnnotationType type = AnnotationType_Undefined;
    error_type result = CommentInfo_GetType(comment, &type);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test content getter
    StringObjectHandle* content = nullptr;
    result = CommentInfo_GetContent(comment, &content);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test author getter
    StringObjectHandle* author = nullptr;
    result = CommentInfo_GetAuthor(comment, &author);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test dates
    DateHandle* date = nullptr;
    result = CommentInfo_GetCreationDate(comment, &date);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    result = CommentInfo_GetModificationDate(comment, &date);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test page number
    size_type page_num = 0;
    result = CommentInfo_GetPageNumber(comment, &page_num);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test rectangle
    real_type left = 0, bottom = 0, right = 0, top = 0;
    result = CommentInfo_GetRectangle(comment, &left, &bottom, &right, &top);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test color
    real_type red = 0, green = 0, blue = 0;
    result = CommentInfo_GetColor(comment, &red, &green, &blue);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test opacity
    real_type opacity = 0;
    result = CommentInfo_GetOpacity(comment, &opacity);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test reply status
    boolean_type is_reply = VANILLAPDF_FALSE;
    result = CommentInfo_IsReply(comment, &is_reply);
    EXPECT_EQ(result, error_type_invalid_arguments);
}

TEST_F(PDFDataAPITest, CommentList_Operations) {
    CommentListHandle* comments = nullptr;
    error_type result = Document_GetComments(document, &comments);
    
    ASSERT_EQ(result, error_type_success);
    ASSERT_NE(comments, nullptr);
    
    size_type count = 0;
    CommentList_GetCount(comments, &count);
    
    // Try to get comment at invalid index
    if (count == 0) {
        CommentInfoHandle* comment = nullptr;
        result = CommentList_GetAt(comments, 0, &comment);
        EXPECT_EQ(result, error_type_index_out_of_range);
    }
    
    CommentList_Release(comments);
}

TEST_F(PDFDataAPITest, CommentReplies) {
    CommentInfoHandle* comment = nullptr;
    
    // Test parent comment getter
    CommentInfoHandle* parent = nullptr;
    error_type result = CommentInfo_GetParentComment(comment, &parent);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    // Test replies getter
    CommentListHandle* replies = nullptr;
    result = CommentInfo_GetReplies(comment, &replies);
    EXPECT_EQ(result, error_type_invalid_arguments);
}

// ===========================
// Integration Tests
// ===========================

TEST_F(PDFDataAPITest, CompleteWorkflow_Signatures) {
    CreateTestPDFWithSignatures();
    
    // Get all signatures
    SignatureListHandle* signatures = nullptr;
    error_type result = Document_GetSignatures(document, &signatures);
    ASSERT_EQ(result, error_type_success);
    ASSERT_NE(signatures, nullptr);
    
    size_type count = 0;
    SignatureList_GetCount(signatures, &count);
    
    // Process each signature
    for (size_type i = 0; i < count; ++i) {
        SignatureInfoHandle* sig = nullptr;
        result = SignatureList_GetAt(signatures, i, &sig);
        EXPECT_EQ(result, error_type_success);
        
        if (sig) {
            // Check verification status
            SignatureStatus status;
            SignatureInfo_GetVerificationStatus(sig, &status);
            
            // Get page number
            size_type page_num = 0;
            SignatureInfo_GetPageNumber(sig, &page_num);
            
            // Check if document was modified
            boolean_type modified = VANILLAPDF_FALSE;
            SignatureInfo_IsDocumentModified(sig, &modified);
        }
    }
    
    SignatureList_Release(signatures);
}

TEST_F(PDFDataAPITest, CompleteWorkflow_Bookmarks) {
    CreateTestPDFWithBookmarks();
    
    // Get all bookmarks
    BookmarkListHandle* bookmarks = nullptr;
    error_type result = Document_GetBookmarks(document, &bookmarks);
    ASSERT_EQ(result, error_type_success);
    ASSERT_NE(bookmarks, nullptr);
    
    size_type count = 0;
    BookmarkList_GetCount(bookmarks, &count);
    
    // Process each bookmark
    for (size_type i = 0; i < count; ++i) {
        BookmarkInfoHandle* bookmark = nullptr;
        result = BookmarkList_GetAt(bookmarks, i, &bookmark);
        EXPECT_EQ(result, error_type_success);
        
        if (bookmark) {
            // Get level
            size_type level = 0;
            BookmarkInfo_GetLevel(bookmark, &level);
            
            // Check if open
            boolean_type is_open = VANILLAPDF_FALSE;
            BookmarkInfo_IsOpen(bookmark, &is_open);
            
            // Get destination
            BookmarkDestinationHandle* dest = nullptr;
            BookmarkInfo_GetDestination(bookmark, &dest);
            
            if (dest) {
                size_type page_num = 0;
                BookmarkDestination_GetPageNumber(dest, &page_num);
                
                DestinationType type;
                BookmarkDestination_GetType(dest, &type);
            }
            
            // Get children
            BookmarkListHandle* children = nullptr;
            BookmarkInfo_GetChildren(bookmark, &children);
            
            if (children) {
                size_type child_count = 0;
                BookmarkList_GetCount(children, &child_count);
            }
        }
    }
    
    BookmarkList_Release(bookmarks);
}

TEST_F(PDFDataAPITest, CompleteWorkflow_Comments) {
    CreateTestPDFWithComments();
    
    // Get all comments
    CommentListHandle* comments = nullptr;
    error_type result = Document_GetComments(document, &comments);
    ASSERT_EQ(result, error_type_success);
    ASSERT_NE(comments, nullptr);
    
    size_type count = 0;
    CommentList_GetCount(comments, &count);
    
    // Process each comment
    for (size_type i = 0; i < count; ++i) {
        CommentInfoHandle* comment = nullptr;
        result = CommentList_GetAt(comments, i, &comment);
        EXPECT_EQ(result, error_type_success);
        
        if (comment) {
            // Get type
            AnnotationType type;
            CommentInfo_GetType(comment, &type);
            
            // Get page number
            size_type page_num = 0;
            CommentInfo_GetPageNumber(comment, &page_num);
            
            // Check if it's a reply
            boolean_type is_reply = VANILLAPDF_FALSE;
            CommentInfo_IsReply(comment, &is_reply);
            
            if (is_reply) {
                CommentInfoHandle* parent = nullptr;
                CommentInfo_GetParentComment(comment, &parent);
            }
            
            // Get replies
            CommentListHandle* replies = nullptr;
            CommentInfo_GetReplies(comment, &replies);
            
            if (replies) {
                size_type reply_count = 0;
                CommentList_GetCount(replies, &reply_count);
            }
        }
    }
    
    CommentList_Release(comments);
}

// ===========================
// Memory Management Tests
// ===========================

TEST_F(PDFDataAPITest, ReleaseHandles_NullSafe) {
    // Test that release functions handle null pointers gracefully
    error_type result;
    
    result = SignatureInfo_Release(nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    result = SignatureList_Release(nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    result = BookmarkInfo_Release(nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    result = BookmarkList_Release(nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    result = BookmarkDestination_Release(nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    result = CommentInfo_Release(nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
    
    result = CommentList_Release(nullptr);
    EXPECT_EQ(result, error_type_invalid_arguments);
}

// ===========================
// Performance Tests
// ===========================

TEST_F(PDFDataAPITest, Performance_LargeDocumentSignatures) {
    // This test would measure performance with a large document
    // containing many signatures
    
    // For now, just ensure the API can handle multiple calls
    for (int i = 0; i < 10; ++i) {
        SignatureListHandle* signatures = nullptr;
        error_type result = Document_GetSignatures(document, &signatures);
        EXPECT_EQ(result, error_type_success);
        if (signatures) {
            SignatureList_Release(signatures);
        }
    }
}

TEST_F(PDFDataAPITest, Performance_LargeDocumentBookmarks) {
    // This test would measure performance with a large document
    // containing many bookmarks
    
    // For now, just ensure the API can handle multiple calls
    for (int i = 0; i < 10; ++i) {
        BookmarkListHandle* bookmarks = nullptr;
        error_type result = Document_GetBookmarks(document, &bookmarks);
        EXPECT_EQ(result, error_type_success);
        if (bookmarks) {
            BookmarkList_Release(bookmarks);
        }
    }
}

TEST_F(PDFDataAPITest, Performance_LargeDocumentComments) {
    // This test would measure performance with a large document
    // containing many comments
    
    // For now, just ensure the API can handle multiple calls
    for (int i = 0; i < 10; ++i) {
        CommentListHandle* comments = nullptr;
        error_type result = Document_GetComments(document, &comments);
        EXPECT_EQ(result, error_type_success);
        if (comments) {
            CommentList_Release(comments);
        }
    }
}