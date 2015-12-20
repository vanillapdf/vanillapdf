#include "precompiled.h"
#include "contents.h"

#include "content_utils.h"
#include "content_stream_operations.h"
#include "content_stream_objects.h"

#include <numeric>

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace syntax;
		using namespace syntax::contents;
		using namespace contents;

		Contents::Contents(StreamObjectPtr obj) : HighLevelObject(obj) {}
		Contents::Contents(ArrayObjectPtr<IndirectObjectReferencePtr> obj) : HighLevelObject(obj->Data()) {}

		BaseInstructionCollection Contents::Instructions(void) const
		{
			if (!_instructions.empty())
				return _instructions;

			std::vector<ContentStreamPtr> contents;
			if (ObjectUtils::IsType<StreamObjectPtr>(_obj)) {
				auto converted = ObjectUtils::ConvertTo<StreamObjectPtr>(_obj);
				contents.push_back(converted);
			}
			else if (ObjectUtils::IsType<ArrayObjectPtr<IndirectObjectReferencePtr>>(_obj)) {
				auto converted = ObjectUtils::ConvertTo<ArrayObjectPtr<IndirectObjectReferencePtr>>(_obj);
				for (auto ref : *converted) {
					auto item = ref->GetReferencedObjectAs<StreamObjectPtr>();
					contents.push_back(item);
				}
			}
			else {
				throw GeneralException("Contents was constructed from unrecognized element: " + _obj->ToString());
			}

			int final_size = std::accumulate(contents.begin(), contents.end(), 0, [](unsigned int sum, const ContentStreamPtr& stream) { return sum + stream->Operations().size(); });
			BaseOperationCollection ops;
			ops.reserve(final_size);

			for (auto item : contents) {
				auto operations = item->Operations();
				auto size = operations.size();
				for (unsigned int i = 0; i < size; ++i) {
					auto op = operations.at(i);
					auto converted = ContentUtils::ConvertGenericOperation(op);
					ops.push_back(converted);
				}
			}

			BaseInstructionCollection result;
			for (auto it = ops.begin(); it != ops.end(); ++it) {
				if (ContentUtils::IsType<OperationBeginTextPtr>(*it)) {
					auto last = std::find_if(it + 1, ops.end(), [it](const decltype(it)::value_type& item) {
						return ContentUtils::IsType<OperationEndTextPtr>(item);
					});

					assert(ops.end() != last && "End of current Text Object was not found");
					if (ops.end() == last) {
						result.push_back(*it);
						continue;
					}

					// Construct the collection
					BaseOperationCollection text_object_data(it, last);

					// Erase Begin and End of Text Object, because they are not part of object
					text_object_data.erase(text_object_data.begin());
					text_object_data.pop_back();

					TextObjectPtr text_object(text_object_data);
					result.push_back(text_object);
					it = last;
				}
				else {
					// unknown
					result.push_back(*it);
				}
			}

			_instructions = result;
			return result;
		}

		types::uinteger Contents::GetInstructionsSize(void) const
		{
			if (!_instructions.empty())
				return _instructions.size();

			return Instructions().size();
		}

		InstructionBasePtr Contents::GetInstructionAt(types::uinteger at) const
		{
			if (!_instructions.empty())
				return _instructions.at(at);

			return Instructions().at(at);
		}
	}
}

