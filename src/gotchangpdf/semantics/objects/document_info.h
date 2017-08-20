#ifndef _DOCUMENT_INFO_H
#define _DOCUMENT_INFO_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/date.h"

namespace gotchangpdf {
namespace semantics {

enum class DocumentTrapped {
	Unknown = 0,
	True,
	False
};

class DocumentInfo : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	explicit DocumentInfo(syntax::DictionaryObjectPtr root);

	// optional
	bool Title(syntax::OutputStringObjectPtr& result) const;
	bool Author(syntax::OutputStringObjectPtr& result) const;
	bool Subject(syntax::OutputStringObjectPtr& result) const;
	bool Keywords(syntax::OutputStringObjectPtr& result) const;
	bool Creator(syntax::OutputStringObjectPtr& result) const;
	bool Producer(syntax::OutputStringObjectPtr& result) const;
	bool CreationDate(OutputDatePtr& result) const;
	bool ModificationDate(OutputDatePtr& result) const;
	bool Trapped(DocumentTrapped& result) const;

	void SetTitle(syntax::StringObjectPtr value);
	void SetAuthor(syntax::StringObjectPtr value);
	void SetSubject(syntax::StringObjectPtr value);
	void SetKeywords(syntax::StringObjectPtr value);
	void SetCreator(syntax::StringObjectPtr value);
	void SetProducer(syntax::StringObjectPtr value);
	void SetCreationDate(DatePtr value);
	void SetModificationDate(DatePtr value);
	void SetTrapped(DocumentTrapped value);
};

} // semantics
} // gotchangpdf

#endif /* _DOCUMENT_INFO_H */
