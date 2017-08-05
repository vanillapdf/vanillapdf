#include "precompiled.h"

#include "semantics/objects/character_map.h"
#include "syntax/parsers/parser.h"
#include "utils/streams/input_stream.h"

namespace gotchangpdf {
namespace semantics {

CharacterMapBase::CharacterMapBase(syntax::StreamObjectPtr root)
	: HighLevelObject(root) {
}

EmbeddedCharacterMap::EmbeddedCharacterMap(syntax::StreamObjectPtr root)
	: CharacterMapBase(root) {
}

UnicodeCharacterMap::UnicodeCharacterMap(syntax::StreamObjectPtr root)
	: CharacterMapBase(root) {
}

std::unique_ptr<CharacterMapBase> CharacterMapBase::Create(syntax::StreamObjectPtr root, WeakReference<Document> doc) {
	// TODO
	return make_unique<UnicodeCharacterMap>(root);
}

void UnicodeCharacterMap::Initialize() const {
	if (m_initialized) {
		return;
	}

	auto body = _obj->GetBody();
	auto body_stream = body->ToStringStream();

	InputStreamPtr input_stream = make_deferred<InputStream>(body_stream);
	syntax::CharacterMapParser parser(_obj->GetFile(), input_stream);
	m_data = parser.ReadCharacterMapData();
	m_initialized = true;
}

BufferPtr UnicodeCharacterMap::GetMappedValue(BufferPtr key) const {
	Initialize();

	for (auto range : m_data.BaseFontRanges) {
		if (range.Contains(key)) {
			return range.GetMappedValue(key);
		}
	}

	throw GeneralException("Value is not mapped in font ranges");
}

} // semantics
} // gotchangpdf
