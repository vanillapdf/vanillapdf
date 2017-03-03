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
	void AppendPage(PageObjectPtr page);

	void Sign();

private:
	syntax::FilePtr m_holder;

	// Cache
	mutable OutputCatalogPtr m_catalog;
	mutable OutputDocumentInfoPtr m_info;

	OutputNamedDestinationsPtr CreateNameDestinations(CatalogPtr catalog);
	OutputNameDictionaryPtr CreateNameDictionary(CatalogPtr catalog);
	OutputNameTreePtr<DestinationPtr> CreateStringDestinations(NameDictionaryPtr dictionary);

	void MergeNameDestinations(NamedDestinationsPtr destinations, types::uinteger merged_pages_count);
	void MergeStringDestinations(NameTreePtr<DestinationPtr> destinations, types::uinteger merged_pages_count);
	void AppendStringDestination(syntax::StringObjectPtr key, DestinationPtr value, types::uinteger merged_pages_count);
	void AppendNameDestination(syntax::NameObjectPtr key, DestinationPtr value, types::uinteger merged_pages_count);

	void FixDestinationPage(DestinationPtr other_destination, syntax::ObjectPtr cloned_page, types::uinteger merged_pages_count);
};

} // semantics
} // gotchangpdf

#endif /* _DOCUMENT_H */
