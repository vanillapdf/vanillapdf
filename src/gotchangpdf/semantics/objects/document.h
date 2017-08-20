#ifndef _DOCUMENT_H
#define _DOCUMENT_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/utils/document_signature_settings.h"

#include "semantics/objects/catalog.h"
#include "semantics/objects/document_info.h"

#include "syntax/files/file.h"

#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/name_object.h"

#include <string>

namespace gotchangpdf {
namespace semantics {

class Document : public virtual IUnknown, public IWeakReferenceable<Document> {
public:
	static DocumentPtr Open(const std::string& path);
	static DocumentPtr Create(const std::string& path);
	static DocumentPtr OpenFile(syntax::FilePtr holder);

	~Document();

	bool GetDocumentCatalog(OutputCatalogPtr& result) const;
	bool GetDocumentInfo(OutputDocumentInfoPtr& result) const;

	void Save(const std::string& path);
	void SaveIncremental(const std::string& path);

	syntax::FilePtr GetFile() const;

	void AppendDocument(DocumentPtr other);
	void AppendPage(DocumentPtr other, PageObjectPtr other_page);

	void Sign(const std::string& path, DocumentSignatureSettingsPtr options);

private:
	syntax::FilePtr m_holder;

	CatalogPtr CreateCatalog();
	DocumentInfoPtr CreateDocumentInfo();
	PageTreePtr CreatePageTree(CatalogPtr catalog);
	NamedDestinationsPtr CreateNameDestinations(CatalogPtr catalog);
	NameDictionaryPtr CreateNameDictionary(CatalogPtr catalog);
	InteractiveFormPtr CreateAcroForm(CatalogPtr catalog);
	NameTreePtr<DestinationPtr> CreateStringDestinations(NameDictionaryPtr dictionary);

	void MergePageDestinations(DocumentPtr other, PageObjectPtr other_page, PageObjectPtr merged_page);
	void MergeNameDestinations(NamedDestinationsPtr destinations, PageObjectPtr other_page, PageObjectPtr merged_page);
	void MergeStringDestinations(NameTreePtr<DestinationPtr> destinations, PageObjectPtr other_page, PageObjectPtr merged_page);
	void AppendStringDestination(syntax::StringObjectPtr key, DestinationPtr value, PageObjectPtr other_page, PageObjectPtr merged_page);
	void AppendNameDestination(syntax::NameObjectPtr key, DestinationPtr value, PageObjectPtr other_page, PageObjectPtr merged_page);

	void FixDestinationPage(syntax::ObjectPtr cloned_page, PageObjectPtr other_page, PageObjectPtr merged_page);
	bool IsDestinationReferencingPage(DestinationPtr destination, PageObjectPtr page);

	void RecalculatePageContents();

private:
	Document(syntax::FilePtr holder);
};

} // semantics
} // gotchangpdf

#endif /* _DOCUMENT_H */
