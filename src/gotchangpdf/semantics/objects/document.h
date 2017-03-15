#ifndef _DOCUMENT_H
#define _DOCUMENT_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/catalog.h"
#include "semantics/objects/document_info.h"
#include "semantics/objects/date.h"

#include "syntax/files/file.h"

#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/name_object.h"

#include <string>

namespace gotchangpdf {
namespace semantics {

class Document : public virtual IUnknown, public IWeakReferenceable<Document> {
public:
	Document(const std::string& filename);
	Document(syntax::FilePtr holder);
	~Document();

	CatalogPtr GetDocumentCatalog(void);
	bool GetDocumentInfo(OutputDocumentInfoPtr& result);
	void Save(const std::string& path);
	void SaveIncremental(const std::string& path);

	syntax::FilePtr GetFile() const;

	void AppendDocument(DocumentPtr other);
	void AppendPage(DocumentPtr other, PageObjectPtr other_page);

	void Sign();

private:
	syntax::FilePtr m_holder;

	// Cache
	mutable OutputCatalogPtr m_catalog;
	mutable OutputDocumentInfoPtr m_info;

	OutputNamedDestinationsPtr CreateNameDestinations(CatalogPtr catalog);
	OutputNameDictionaryPtr CreateNameDictionary(CatalogPtr catalog);
	OutputNameTreePtr<DestinationPtr> CreateStringDestinations(NameDictionaryPtr dictionary);

	void MergePageDestinations(DocumentPtr other, PageObjectPtr other_page, PageObjectPtr merged_page);
	void MergeNameDestinations(NamedDestinationsPtr destinations, PageObjectPtr other_page, PageObjectPtr merged_page);
	void MergeStringDestinations(NameTreePtr<DestinationPtr> destinations, PageObjectPtr other_page, PageObjectPtr merged_page);
	void AppendStringDestination(syntax::StringObjectPtr key, DestinationPtr value, PageObjectPtr other_page, PageObjectPtr merged_page);
	void AppendNameDestination(syntax::NameObjectPtr key, DestinationPtr value, PageObjectPtr other_page, PageObjectPtr merged_page);

	void FixDestinationPage(syntax::ObjectPtr cloned_page, PageObjectPtr other_page, PageObjectPtr merged_page);
	bool IsDestinationReferencingPage(DestinationPtr destination, PageObjectPtr page);
};

} // semantics
} // gotchangpdf

#endif /* _DOCUMENT_H */
