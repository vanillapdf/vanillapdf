#include "precompiled.h"

#include "syntax/files/file.h"

#include "semantics/objects/page_contents.h"

#include "contents/content_stream_parser.h"
#include "contents/content_stream_operations.h"
#include "contents/content_stream_objects.h"

#include "utils/streams/input_stream.h"

#include <list>
#include <numeric>
#include <sstream>

namespace vanillapdf {
namespace semantics {

using namespace syntax;
using namespace contents;

PageContents::PageContents(StreamObjectPtr obj) : HighLevelObject(obj) {
    m_instructions->Subscribe(this);
}

PageContents::PageContents(ArrayObjectPtr<IndirectReferenceObjectPtr> obj)
    : HighLevelObject(obj->Data()) {
    m_instructions->Subscribe(this);
}

PageContents::~PageContents() {
    m_instructions->Unsubscribe(this);
}

void PageContents::ObserveeChanged(const IModifyObservable*) {
    SetDirty(true);
}

bool PageContents::IsDirty() const {
    return GetObject()->IsDirty();
}

void PageContents::SetDirty(bool dirty) {
    GetObject()->SetDirty(dirty);
}

bool PageContents::RecalculateStreamData() {

    auto obj_number = GetObject()->GetRootObjectNumber();
    auto gen_number = GetObject()->GetRootGenerationNumber();

    if (!IsDirty()) {
        spdlog::debug("Page {} {} contents are not dirty", obj_number, gen_number);
        return false;
    }

    spdlog::info("Page {} {} contents are dirty, recalculating", obj_number, gen_number);

    std::stringstream ss;
    for (auto instruction : m_instructions) {
        ss << instruction->ToPdf() << std::endl;
    }

    auto object = GetObject();

    StreamObjectPtr stream_object;
    if (ObjectUtils::IsType<StreamObjectPtr>(object)) {
        stream_object = ObjectUtils::ConvertTo<StreamObjectPtr>(object);
    }

    if (ObjectUtils::IsType<ArrayObjectPtr<StreamObjectPtr>>(object)) {
        auto stream_array = ObjectUtils::ConvertTo<ArrayObjectPtr<StreamObjectPtr>>(object);
        auto stream_array_size = stream_array->GetSize();

        assert(0 != stream_array_size && "Content stream array is empty");
        if (0 == stream_array_size) {
            throw GeneralException("Content stream array is empty");
        }

        for (decltype(stream_array_size) j = 0; j < stream_array_size; ++j) {
            auto referenced_stram = stream_array->GetValue(j);

            // TODO divide output into multiple streams
            BufferPtr empty_body;
            referenced_stram->SetBody(empty_body);
        }

        stream_object = stream_array->GetValue(0);
    }

    std::string string_body = ss.str();
    BufferPtr new_body = make_deferred_container<Buffer>(string_body.begin(), string_body.end());
    stream_object->SetBody(new_body);

    return true;
}

BaseInstructionCollectionPtr PageContents::Instructions(void) const {
    if (m_instructions->IsInitialized()) {
        return m_instructions;
    }

    std::list<ContentStreamPtr> contents;
    if (ObjectUtils::IsType<StreamObjectPtr>(_obj)) {
        auto converted = ObjectUtils::ConvertTo<StreamObjectPtr>(_obj);
        auto content_stream = make_deferred<ContentStream>(converted);
        contents.push_back(content_stream);
    } else if (ObjectUtils::IsType<ArrayObjectPtr<IndirectReferenceObjectPtr>>(_obj)) {
        auto converted = ObjectUtils::ConvertTo<ArrayObjectPtr<IndirectReferenceObjectPtr>>(_obj);
        for (auto ref : converted) {
            auto stream = ref->GetReferencedObjectAs<StreamObjectPtr>();
            auto content_stream = make_deferred<ContentStream>(stream);
            contents.push_back(content_stream);
        }
    } else {
        throw GeneralException("Contents was constructed from unrecognized element: " + _obj->ToString());
    }

    // We are not using contents.Instructions, because objects can be separated
    // into multiple content streams
    auto ss = std::make_shared<std::stringstream>();
    for (auto item : contents) {
        auto stream_object = item->GetObject();
        *ss << stream_object->GetBody();
    }

    InputStreamPtr input_stream = make_deferred<InputStream>(ss);
    contents::ContentStreamParser parser(_obj->GetFile(), input_stream);

    m_instructions = parser.ReadInstructions();
    m_instructions->SetInitialized();

    return m_instructions;
}

} // semantics
} // vanillapdf
