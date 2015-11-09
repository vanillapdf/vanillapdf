#include "precompiled.h"
#include "contents.h"

#include "content_stream_grammar.h"

namespace gotchangpdf
{
	namespace documents
	{
		Contents::Contents(StreamObjectPtr obj)
		{
			ContentStreamPtr stream(obj);
			_contents.push_back(obj);
		}

		Contents::Contents(ArrayObjectPtr<IndirectObjectReferencePtr> obj)
		{
			for (auto& ref : *obj) {
				auto item = ref->GetReferencedObjectAs<StreamObjectPtr>();
				_contents.push_back(item);
			}
		}

		ContentInstructionCollection Contents::Operations(void) const
		{
			lexical::ContentStreamOperationCollection ops;

			for (auto item : _contents) {
				auto operations = item->Operations();
				ops.insert(ops.end(), operations.begin(), operations.end());
			}

			// visitors
			lexical::IsContentStreamOperatorVisitor<lexical::BeginTextOperatorPtr> is_begin_text;
			lexical::IsContentStreamOperatorVisitor<lexical::EndTextOperatorPtr> is_end_text;

			ContentInstructionCollection result;
			auto it = ops.begin();
			while (it != ops.end()) {
				if (it->second.apply_visitor(is_begin_text)) {
					// begin text
					assert(it->first.size() == 0);

					auto last = std::find_if(it, ops.end(), [is_end_text](const lexical::ContentStreamOperationCollection::value_type& item) {
						return item.second.apply_visitor(is_end_text);
					});

					lexical::ContentStreamOperationCollection text_object_data(it, last);
					ContentObjectPtr text_object = TextObjectPtr(text_object_data);
					result.push_back(text_object);
					it = last;
				}
				else {
					// generic
				}

				++it;
			}

			return result;
		}

		types::uinteger Contents::GetOperationsSize(void) const
		{
			auto ops = Operations();
			return ops.size();
		}

		ContentInstructionPtr Contents::GetOperationAt(types::uinteger at) const
		{
			auto ops = Operations();
			return ops.at(at);
		}
	}
}

