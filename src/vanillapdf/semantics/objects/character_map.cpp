#include "precompiled.h"

#include "syntax/files/file.h"
#include "contents/character_map_parser.h"

#include "semantics/objects/character_map.h"

namespace vanillapdf {
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
	auto input_stream = body->ToInputStream();

	contents::CharacterMapParser parser(_obj->GetFile(), input_stream);
	m_data = parser.ReadCharacterMapData();
	m_initialized = true;
}

BufferPtr UnicodeCharacterMap::GetMappedValue(BufferPtr key) const {
	Initialize();

	bool in_codespace = false;
	for (auto range : m_data.CodeSpaceRanges) {
		auto begin_range = range.Begin->GetValue();
		auto end_range = range.End->GetValue();

		if (key->ValueEqualLessThan(end_range) && begin_range->ValueEqualLessThan(key)) {
			in_codespace = true;
			break;
		}
	}

	if (!in_codespace) {

		auto weak_file = GetObject()->GetFile();
		auto file = weak_file.GetReference();
		auto log_scope = file->GetFilenameString();

		std::stringstream error_stream;
		error_stream << "The key " << key->ToHexString() << " was not found in the UnicodeCharacterMap codespace range";

		LOG_ERROR(log_scope) << error_stream.str();

		auto codespace_range_size = m_data.CodeSpaceRanges.size();
		for (decltype(codespace_range_size) i = 0; i < codespace_range_size; ++i) {
			LOG_ERROR(log_scope) << "Codespace ranges [" << i << "]" << " " << m_data.CodeSpaceRanges[i].Begin->ToString() << ":" << m_data.CodeSpaceRanges[i].End->ToString();
		}

		throw GeneralException(error_stream.str());
	}

	// Check base font ranges
	for (auto range : m_data.BaseFontRanges) {
		if (range.Contains(key)) {
			return range.GetMappedValue(key);
		}
	}

	// Check base font chars
	for (auto char_mapping : m_data.BaseFontCharMapping) {
		if (char_mapping.Source->GetValue() == key) {
			return char_mapping.Destination->GetValue();
		}
	}

	auto weak_file = GetObject()->GetFile();
	auto file = weak_file.GetReference();
	auto log_scope = file->GetFilenameString();

	std::stringstream error_stream;
	error_stream << "The key " << key->ToHexString() << " was not found in the UnicodeCharacterMap";

	LOG_ERROR(log_scope) << error_stream.str();

	LOG_ERROR(log_scope) << "CodeSpaceRanges: " << m_data.CodeSpaceRanges.size();

	auto codespace_range_size = m_data.CodeSpaceRanges.size();
	for (decltype(codespace_range_size) i = 0; i < codespace_range_size; ++i) {
		LOG_ERROR(log_scope) << "Codespace range [" << i << "]" << " " << m_data.CodeSpaceRanges[i].Begin->ToString() << ":" << m_data.CodeSpaceRanges[i].End->ToString();
	}

	LOG_ERROR(log_scope) << "BaseFontRanges: " << m_data.BaseFontRanges.size();

	auto font_range_size = m_data.BaseFontRanges.size();
	for (decltype(font_range_size) i = 0; i < font_range_size; ++i) {
		LOG_ERROR(log_scope) << "Font range [" << i << "]" << " " << m_data.BaseFontRanges[i].GetRangeLow()->ToString() << ":" << m_data.BaseFontRanges[i].GetRangeHigh()->ToString();
	}

	LOG_ERROR(log_scope) << "BaseFontCharMapping: " << m_data.BaseFontCharMapping.size();

	auto font_char_size = m_data.BaseFontCharMapping.size();
	for (decltype(font_char_size) i = 0; i < font_char_size; ++i) {
		LOG_ERROR(log_scope) << "Font char mapping [" << i << "]" << " " << m_data.BaseFontCharMapping[i].Source->ToString() << ":" << m_data.BaseFontCharMapping[i].Destination->ToString();
	}

	throw GeneralException(error_stream.str());
}

} // semantics
} // vanillapdf
