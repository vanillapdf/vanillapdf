#ifndef _DOCUMENT_H
#define _DOCUMENT_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/utils/document_encryption_settings.h"
#include "semantics/utils/document_signature_settings.h"

#include "semantics/objects/catalog.h"
#include "semantics/objects/document_info.h"

#include "syntax/files/file.h"

#include "syntax/objects/array_object.h"
#include "syntax/objects/string_object.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/name_object.h"

#include "utils/cached_value.h"

#include <string>

namespace vanillapdf {
namespace semantics {

class Document : public IUnknown, public IWeakReferenceable<Document> {
public:
    static DocumentPtr Open(const std::string& path);
    static DocumentPtr Open(const std::string& path, IOStrategy strategy);
    static DocumentPtr OpenFile(syntax::FilePtr holder);

    static DocumentPtr Create(const std::string& path);
    static DocumentPtr Create(const std::string& path, IOStrategy strategy);
    static DocumentPtr CreateFile(syntax::FilePtr holder);

    static syntax::ArrayObjectPtr<syntax::HexadecimalStringObjectPtr> GenerateDocumentId();

    ~Document();

    bool GetDocumentCatalog(OutputCatalogPtr& result) const;
    bool GetDocumentInfo(OutputDocumentInfoPtr& result) const;
    void SetDocumentInfo(DocumentInfoPtr value);

private:
    bool ResolveCatalog(OutputCatalogPtr& result) const;

public:

    void Save(const std::string& path);
    void Save(syntax::FilePtr destination);
    void SaveIncremental(const std::string& path);
    void SaveIncremental(syntax::FilePtr destination);

    syntax::FilePtr GetFile() const;

    void AppendDocument(DocumentPtr other);
    void AppendPage(DocumentPtr other, PageObjectPtr other_page);

    // Signs the document, creating an invisible signature field of its own on
    // the first page.
    void Sign(syntax::FilePtr destination, DocumentSignatureSettingsPtr options);

    // Signs into a signature field the caller has already built and attached,
    // which is how a signature gets a visible appearance - the field carries
    // the widget annotation, its rectangle and its appearance stream.
    //
    // The field is named rather than passed, so that resolving it through this
    // document's field hierarchy is what establishes that it belongs here and
    // is reachable from the interactive form. The name is the fully qualified
    // one, in the form FieldTree::TryFindField takes.
    //
    // ObjectMissingException when no field of that name exists, and
    // InvalidParameterException when it is not a signature field or already
    // carries a signature.
    void SignField(syntax::FilePtr destination, std::string_view qualified_name, DocumentSignatureSettingsPtr options);

    void AddEncryption(DocumentEncryptionSettingsPtr settings);
    void RemoveEncryption();

private:
    syntax::FilePtr m_holder;

    CatalogPtr CreateCatalog();
    DocumentInfoPtr CreateDocumentInfo();
    PageTreePtr CreatePageTree(CatalogPtr catalog);
    NamedDestinationsPtr CreateNameDestinations(CatalogPtr catalog);
    NameDictionaryPtr CreateNameDictionary(CatalogPtr catalog);
    InteractiveFormPtr CreateAcroForm(CatalogPtr catalog);

    // Builds the signature dictionary and registers it. /ByteRange and
    // /Contents are left as placeholders that the signer overwrites while the
    // document is being written.
    syntax::DictionaryObjectPtr CreateSignatureDictionary(DocumentSignatureSettingsPtr options);

    // Marks the interactive form as carrying signatures
    void UpdateSignatureFlags(InteractiveFormPtr interactive_form);

    // Writes the document out, the signer filling the placeholders in passing
    void WriteSignedDocument(syntax::FilePtr destination, syntax::DictionaryObjectPtr signature_dictionary, DocumentSignatureSettingsPtr options);
    NameTreePtr<DestinationPtr> CreateStringDestinations(NameDictionaryPtr dictionary);

    void MergePageDestinations(DocumentPtr other, PageObjectPtr other_page, PageObjectPtr merged_page);
    void MergeNameDestinations(NamedDestinationsPtr destinations, PageObjectPtr other_page, PageObjectPtr merged_page);
    void MergeStringDestinations(NameTreePtr<DestinationPtr> destinations, PageObjectPtr other_page, PageObjectPtr merged_page);
    void AppendStringDestination(syntax::StringObjectPtr key, DestinationPtr value, PageObjectPtr other_page, PageObjectPtr merged_page);
    void AppendNameDestination(syntax::NameObjectPtr key, DestinationPtr value, PageObjectPtr other_page, PageObjectPtr merged_page);

    void FixDestinationPage(syntax::ObjectPtr cloned_page, PageObjectPtr other_page, PageObjectPtr merged_page);
    bool IsDestinationReferencingPage(DestinationPtr destination, PageObjectPtr page);

    void ForceObjectInitialization();
    void ForceObjectInitialization(syntax::ObjectPtr obj);

private:
    explicit Document(syntax::FilePtr holder);

    CachedValue<OutputCatalogPtr> m_catalog;

    friend class SemanticUtils;
};

} // semantics
} // vanillapdf

#endif /* _DOCUMENT_H */
