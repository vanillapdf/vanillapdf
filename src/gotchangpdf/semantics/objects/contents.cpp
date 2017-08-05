#include "precompiled.h"

#include "semantics/objects/contents.h"

#include "contents/content_stream_parser.h"
#include "contents/content_stream_operations.h"
#include "contents/content_stream_objects.h"

#include "utils/streams/input_stream.h"

#include <numeric>

namespace gotchangpdf {
namespace semantics {

using namespace syntax;
using namespace contents;

Contents::Contents(StreamObjectPtr obj) : HighLevelObject(obj) {
	m_instructions->Subscribe(this);
}

Contents::Contents(ArrayObjectPtr<IndirectObjectReferencePtr> obj)
	: HighLevelObject(obj->Data()) {
	m_instructions->Subscribe(this);
}

Contents::~Contents() {
	m_instructions->Unsubscribe(this);
}

BaseInstructionCollectionPtr Contents::Instructions(void) const {
	if (m_instructions->IsInitialized())
		return m_instructions;

	std::vector<ContentStreamPtr> contents;
	if (ObjectUtils::IsType<StreamObjectPtr>(_obj)) {
		auto converted = ObjectUtils::ConvertTo<StreamObjectPtr>(_obj);
		auto content_stream = make_deferred<ContentStream>(converted);
		contents.push_back(content_stream);
	} else if (ObjectUtils::IsType<ArrayObjectPtr<IndirectObjectReferencePtr>>(_obj)) {
		auto converted = ObjectUtils::ConvertTo<ArrayObjectPtr<IndirectObjectReferencePtr>>(_obj);
		for (auto ref : converted) {
			auto stream = ref->GetReferencedObjectAs<StreamObjectPtr>();
			auto content_stream = make_deferred<ContentStream>(stream);
			contents.push_back(content_stream);
		}
	} else {
		throw GeneralException("Contents was constructed from unrecognized element: " + _obj->ToString());
	}

	// We are not using contents.Instructions, because objects can we separated
	// into multiple content streams
	auto ss = std::make_shared<std::stringstream>();
	for (auto item : contents) {
		auto stream_object = item->GetObject();
		*ss << stream_object->GetBody();
	}

	InputStreamPtr input_stream = make_deferred<InputStream>(ss);
	contents::ContentStreamParser parser(_obj->GetFile(), input_stream);
	auto instructions = parser.ReadContentStreamInstructions();
	for (auto instruction : instructions) {
		m_instructions->push_back(instruction);
	}

	m_instructions->SetInitialized();
	return m_instructions;
}

types::size_type Contents::GetInstructionsSize(void) const {
	if (m_instructions->IsInitialized()) {
		return m_instructions->size();
	}

	Instructions();
	return m_instructions->size();
}

InstructionBasePtr Contents::GetInstructionAt(types::size_type at) const {
	if (!m_instructions->IsInitialized()) {
		return m_instructions->at(at);
	}

	Instructions();
	return m_instructions->at(at);
}

} // semantics
} // gotchangpdf

