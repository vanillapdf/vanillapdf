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
			if (!_operations.empty())
				return _operations;

			lexical::ContentStreamOperationCollection ops;

			for (auto item : _contents) {
				auto operations = item->Operations();
				auto size = operations.size();
				ops.reserve(ops.size() + size);
				for (unsigned int i = 0; i < size; ++i) {
					auto op = operations.at(i);
					ops.push_back(op);
				}
			}

			// visitor
			lexical::ContentStreamOperatorTypeVisitor type_visitor;

			ContentInstructionCollection result;
			auto it = ops.begin();
			while (it != ops.end()) {
				auto type = it->second.apply_visitor(type_visitor);
				if (type == lexical::OperatorBase::Type::BeginText) {
					// begin text
					assert(it->first.size() == 0);

					auto last = std::find_if(it, ops.end(), [type_visitor](const decltype(it)::value_type& item) {
						auto op = item.second;
						auto type = op.apply_visitor(type_visitor);
						return (type == lexical::OperatorBase::Type::EndText);
					});

					lexical::ContentStreamOperationCollection text_object_data(it, last);
					ContentObjectPtr text_object = TextObjectPtr(text_object_data);
					result.push_back(text_object);
					it = last;
				}
				else {
					// generic
					ContentOperationPtr generic_operation = *it;
					result.push_back(generic_operation);
				}

				++it;
			}

			_operations = result;
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

