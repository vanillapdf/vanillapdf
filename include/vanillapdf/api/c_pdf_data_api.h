#ifndef _C_PDF_DATA_API_H
#define _C_PDF_DATA_API_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"
#include "vanillapdf/semantics/c_digital_signature.h"
#include "vanillapdf/semantics/c_outline.h"
#include "vanillapdf/semantics/c_annotations.h"
#include "vanillapdf/semantics/c_destinations.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_pdf_data_api.h
    * \brief High-level API for accessing PDF document data including signatures, bookmarks, and comments
    */

    /* ============================= */
    /* Signature API                 */
    /* ============================= */

    /**
    * \class SignatureInfoHandle
    * \extends IUnknownHandle
    * \ingroup group_api
    * \brief Contains comprehensive information about a digital signature
    */

    /**
    * \class SignatureListHandle
    * \extends IUnknownHandle
    * \ingroup group_api
    * \brief Collection of signatures in a document
    */

    /**
    * \brief Signature verification status
    * \ingroup group_api
    */
    typedef enum {
        SignatureStatus_Unknown = 0,
        SignatureStatus_Valid,
        SignatureStatus_Invalid,
        SignatureStatus_NotVerified,
        SignatureStatus_CertificateExpired,
        SignatureStatus_CertificateRevoked,
        SignatureStatus_CertificateUntrusted,
        SignatureStatus_DocumentModified
    } SignatureStatus;

    /**
    * \memberof SignatureInfoHandle
    * @{
    */

    /**
    * \brief Get the signature field name
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetFieldName(SignatureInfoHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the signer name
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetSignerName(SignatureInfoHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the signing time
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetSigningTime(SignatureInfoHandle* handle, DateHandle** result);

    /**
    * \brief Get the signing reason
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetReason(SignatureInfoHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the signing location
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetLocation(SignatureInfoHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get contact information
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetContactInfo(SignatureInfoHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the verification status of the signature
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetVerificationStatus(SignatureInfoHandle* handle, SignatureStatus* result);

    /**
    * \brief Get detailed verification message
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetVerificationMessage(SignatureInfoHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the certificate chain
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetCertificateChain(SignatureInfoHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the page number where signature appears (0-based)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetPageNumber(SignatureInfoHandle* handle, size_type* result);

    /**
    * \brief Check if document was modified after signing
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_IsDocumentModified(SignatureInfoHandle* handle, boolean_type* result);

    /**
    * \brief Get the underlying digital signature handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_GetDigitalSignature(SignatureInfoHandle* handle, DigitalSignatureHandle** result);

    /**
    * \copydoc IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureInfo_Release(SignatureInfoHandle* handle);

    /** @} */

    /**
    * \memberof SignatureListHandle
    * @{
    */

    /**
    * \brief Get the number of signatures in the list
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureList_GetCount(SignatureListHandle* handle, size_type* result);

    /**
    * \brief Get signature at specified index
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureList_GetAt(SignatureListHandle* handle, size_type index, SignatureInfoHandle** result);

    /**
    * \copydoc IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureList_Release(SignatureListHandle* handle);

    /** @} */

    /* ============================= */
    /* Bookmark API                  */
    /* ============================= */

    /**
    * \class BookmarkInfoHandle
    * \extends IUnknownHandle
    * \ingroup group_api
    * \brief Contains comprehensive information about a bookmark/outline item
    */

    /**
    * \class BookmarkListHandle
    * \extends IUnknownHandle
    * \ingroup group_api
    * \brief Collection of bookmarks in a document
    */

    /**
    * \class BookmarkDestinationHandle
    * \extends IUnknownHandle
    * \ingroup group_api
    * \brief Destination information for a bookmark
    */

    /**
    * \memberof BookmarkInfoHandle
    * @{
    */

    /**
    * \brief Get the bookmark title
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetTitle(BookmarkInfoHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the bookmark level (0 for top-level)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetLevel(BookmarkInfoHandle* handle, size_type* result);

    /**
    * \brief Check if bookmark is open/expanded
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_IsOpen(BookmarkInfoHandle* handle, boolean_type* result);

    /**
    * \brief Get the destination for this bookmark
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetDestination(BookmarkInfoHandle* handle, BookmarkDestinationHandle** result);

    /**
    * \brief Get child bookmarks
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetChildren(BookmarkInfoHandle* handle, BookmarkListHandle** result);

    /**
    * \brief Get the color of the bookmark (RGB)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetColor(BookmarkInfoHandle* handle, 
                                                                        real_type* red, 
                                                                        real_type* green, 
                                                                        real_type* blue);

    /**
    * \brief Check if bookmark text is italic
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_IsItalic(BookmarkInfoHandle* handle, boolean_type* result);

    /**
    * \brief Check if bookmark text is bold
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_IsBold(BookmarkInfoHandle* handle, boolean_type* result);

    /**
    * \brief Get the underlying outline item handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_GetOutlineItem(BookmarkInfoHandle* handle, OutlineItemHandle** result);

    /**
    * \copydoc IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkInfo_Release(BookmarkInfoHandle* handle);

    /** @} */

    /**
    * \memberof BookmarkDestinationHandle
    * @{
    */

    /**
    * \brief Get the target page number (0-based)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkDestination_GetPageNumber(BookmarkDestinationHandle* handle, size_type* result);

    /**
    * \brief Get the destination type
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkDestination_GetType(BookmarkDestinationHandle* handle, DestinationType* result);

    /**
    * \brief Get the position coordinates (if applicable)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkDestination_GetPosition(BookmarkDestinationHandle* handle,
                                                                                 real_type* x,
                                                                                 real_type* y,
                                                                                 real_type* zoom);

    /**
    * \brief Get the rectangle coordinates for FitRectangle type
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkDestination_GetRectangle(BookmarkDestinationHandle* handle,
                                                                                  real_type* left,
                                                                                  real_type* bottom,
                                                                                  real_type* right,
                                                                                  real_type* top);

    /**
    * \copydoc IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkDestination_Release(BookmarkDestinationHandle* handle);

    /** @} */

    /**
    * \memberof BookmarkListHandle
    * @{
    */

    /**
    * \brief Get the number of bookmarks in the list
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkList_GetCount(BookmarkListHandle* handle, size_type* result);

    /**
    * \brief Get bookmark at specified index
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkList_GetAt(BookmarkListHandle* handle, size_type index, BookmarkInfoHandle** result);

    /**
    * \copydoc IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkList_Release(BookmarkListHandle* handle);

    /** @} */

    /* ============================= */
    /* Comment/Annotation API        */
    /* ============================= */

    /**
    * \class CommentInfoHandle
    * \extends IUnknownHandle
    * \ingroup group_api
    * \brief Contains comprehensive information about a comment/annotation
    */

    /**
    * \class CommentListHandle
    * \extends IUnknownHandle
    * \ingroup group_api
    * \brief Collection of comments in a document
    */

    /**
    * \memberof CommentInfoHandle
    * @{
    */

    /**
    * \brief Get the comment type
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetType(CommentInfoHandle* handle, AnnotationType* result);

    /**
    * \brief Get the comment content/text
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetContent(CommentInfoHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the comment subject
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetSubject(CommentInfoHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the comment author
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetAuthor(CommentInfoHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the creation date
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetCreationDate(CommentInfoHandle* handle, DateHandle** result);

    /**
    * \brief Get the modification date
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetModificationDate(CommentInfoHandle* handle, DateHandle** result);

    /**
    * \brief Get the page number where comment appears (0-based)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetPageNumber(CommentInfoHandle* handle, size_type* result);

    /**
    * \brief Get the position rectangle of the comment
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetRectangle(CommentInfoHandle* handle,
                                                                          real_type* left,
                                                                          real_type* bottom,
                                                                          real_type* right,
                                                                          real_type* top);

    /**
    * \brief Get the color of the comment (RGB)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetColor(CommentInfoHandle* handle,
                                                                      real_type* red,
                                                                      real_type* green,
                                                                      real_type* blue);

    /**
    * \brief Get the opacity/transparency value (0.0 to 1.0)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetOpacity(CommentInfoHandle* handle, real_type* result);

    /**
    * \brief Check if comment is a reply to another comment
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_IsReply(CommentInfoHandle* handle, boolean_type* result);

    /**
    * \brief Get the parent comment if this is a reply
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetParentComment(CommentInfoHandle* handle, CommentInfoHandle** result);

    /**
    * \brief Get replies to this comment
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetReplies(CommentInfoHandle* handle, CommentListHandle** result);

    /**
    * \brief Get the underlying annotation handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_GetAnnotation(CommentInfoHandle* handle, AnnotationHandle** result);

    /**
    * \copydoc IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentInfo_Release(CommentInfoHandle* handle);

    /** @} */

    /**
    * \memberof CommentListHandle
    * @{
    */

    /**
    * \brief Get the number of comments in the list
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentList_GetCount(CommentListHandle* handle, size_type* result);

    /**
    * \brief Get comment at specified index
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentList_GetAt(CommentListHandle* handle, size_type index, CommentInfoHandle** result);

    /**
    * \copydoc IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CommentList_Release(CommentListHandle* handle);

    /** @} */

    /* ============================= */
    /* Document-level API Functions  */
    /* ============================= */

    /**
    * \brief Get all signatures in the document
    * \param document The document handle
    * \param result Pointer to receive the signature list
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_GetSignatures(DocumentHandle* document, SignatureListHandle** result);

    /**
    * \brief Verify all signatures in the document
    * \param document The document handle
    * \param all_valid Pointer to receive whether all signatures are valid
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_VerifySignatures(DocumentHandle* document, boolean_type* all_valid);

    /**
    * \brief Get all bookmarks in the document
    * \param document The document handle
    * \param result Pointer to receive the bookmark list
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_GetBookmarks(DocumentHandle* document, BookmarkListHandle** result);

    /**
    * \brief Get all comments in the document
    * \param document The document handle
    * \param result Pointer to receive the comment list
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_GetComments(DocumentHandle* document, CommentListHandle** result);

    /**
    * \brief Get comments for a specific page
    * \param document The document handle
    * \param page_number The page number (0-based)
    * \param result Pointer to receive the comment list
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Document_GetPageComments(DocumentHandle* document, size_type page_number, CommentListHandle** result);

#ifdef __cplusplus
};
#endif

#endif /* _C_PDF_DATA_API_H */