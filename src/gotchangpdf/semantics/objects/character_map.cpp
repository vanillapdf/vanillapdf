#include "precompiled.h"
#include "semantics/objects/character_map.h"
#include "syntax/parsers/parser.h"

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

CharacterMapBase::Type EmbeddedCharacterMap::GetType() const noexcept {
	return Type::Embedded;
}

CharacterMapBase::Type UnicodeCharacterMap::GetType() const noexcept {
	return Type::Unicode;
}

CharacterMapBase* CharacterMapBase::Create(syntax::StreamObjectPtr root, WeakReference<Document> doc) {
	// TODO
	auto result = make_unique<UnicodeCharacterMap>(root);
	return result.release();
}

void UnicodeCharacterMap::Initialize() const {
	if (m_initialized) {
		return;
	}

	auto body = _obj->GetBody();
	auto body_stream = body->ToStringStream();
	syntax::CharacterMapParser parser(_obj->GetFile(), body_stream);
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
