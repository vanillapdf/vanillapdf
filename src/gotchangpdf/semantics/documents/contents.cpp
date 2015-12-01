#include "precompiled.h"
#include "contents.h"

#include "content_stream_grammar.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace syntax;

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

		ContentInstructionCollection Contents::Instructions(void) const
		{
			if (!_instructions.empty())
				return _instructions;

			contents::OperationCollection ops;

			for (auto item : _contents) {
				auto operations = item->Operations();
				auto size = operations.size();
				ops.reserve(ops.size() + size);
				for (unsigned int i = 0; i < size; ++i) {
					auto op = operations.at(i);
					ops.push_back(op);
				}
			}

			// visitors
			contents::IsOperatorTypeVisitor<contents::BeginTextOperatorPtr> is_begin_text;
			contents::IsOperatorTypeVisitor<contents::EndTextOperatorPtr> is_end_text;

			ContentInstructionCollection result;
			auto it = ops.begin();
			while (it != ops.end()) {
				auto item = *it;
				if (item->GetOperator().apply_visitor(is_begin_text)) {
					// begin text
					assert(item->GetOperands().size() == 0);

					auto last = std::find_if(it, ops.end(), [is_end_text](const decltype(it)::value_type& item) {
						return item->GetOperator().apply_visitor(is_end_text);
					});

					contents::OperationCollection text_object_data(it, last);
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

			_instructions = result;
			return result;
		}

		types::uinteger Contents::GetInstructionsSize(void) const
		{
			auto ops = Instructions();
			return ops.size();
		}

		ContentInstructionPtr Contents::GetInstructionAt(types::uinteger at) const
		{
			auto ops = Instructions();
			return ops.at(at);
		}
	}
}

