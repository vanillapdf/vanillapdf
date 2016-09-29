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

		Contents::Contents(StreamObjectPtr obj) : HighLevelObject(obj)
		{
			m_instructions->Subscribe(this);
		}

		Contents::Contents(ArrayObjectPtr<IndirectObjectReferencePtr> obj)
			: HighLevelObject(obj->Data())
		{
			m_instructions->Subscribe(this);
		}

		Contents::~Contents()
		{
			m_instructions->Unsubscribe(this);
		}

		BaseInstructionCollectionPtr Contents::Instructions(void) const
		{
			if (m_instructions->IsInitialized())
				return m_instructions;

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
				ss << stream_object->GetBody();
			}

			syntax::ContentStreamParser parser(_obj->GetFile(), ss);
			auto instructions = parser.ReadContentStreamInstructions();
			for (auto instruction : *instructions) {
				m_instructions->push_back(instruction);
			}

			m_instructions->SetInitialized();
			return m_instructions;
		}

		types::uinteger Contents::GetInstructionsSize(void) const
		{
			if (m_instructions->IsInitialized())
				return m_instructions->size();

			Instructions();
			return m_instructions->size();
		}

		InstructionBasePtr Contents::GetInstructionAt(types::uinteger at) const
		{
			if (!m_instructions->IsInitialized())
				return m_instructions->at(at);

			Instructions();
			return m_instructions->at(at);
		}
	}
}

