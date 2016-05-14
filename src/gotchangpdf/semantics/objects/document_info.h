#ifndef _DOCUMENT_INFO_H
#define _DOCUMENT_INFO_H

#include "semantics_fwd.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class DocumentInfo : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
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
			bool Trapped(syntax::OutputNameObjectPtr& result) const;
		};
	}
}

#endif /* _DOCUMENT_INFO_H */
