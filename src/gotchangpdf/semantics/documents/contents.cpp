#include "precompiled.h"
#include "contents.h"

#include "parser.h"
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
		void Contents::ObserveeChanged(IModifyObservable*) { m_dirty = true; }

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

			// We are not using contents.Instructions, because objects can we separated
			// into multiple content streams
			std::stringstream ss;
			for (auto item : contents) {
				auto stream_object = item->GetObject();
				auto body = stream_object->GetBody();
				ss << body->ToString();
			}

			auto parser = syntax::Parser(_obj->GetFile(), ss);
			_instructions = parser.ReadContentStreamInstructions();
			return _instructions;
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

