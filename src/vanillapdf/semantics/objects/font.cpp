#include "precompiled.h"
#include "semantics/objects/font.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

FontMap::FontMap(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {}
FontBase::FontBase(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
Type0Font::Type0Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
Type1Font::Type1Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
MMType1Font::MMType1Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
Type3Font::Type3Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
TrueTypeFont::TrueTypeFont(syntax::DictionaryObjectPtr root) : FontBase(root) {}
CIDFontType0Font::CIDFontType0Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
CIDFontType2Font::CIDFontType2Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}

FontBase::Type Type0Font::GetFontType() const noexcept { return Type::Type0; }
FontBase::Type Type1Font::GetFontType() const noexcept { return Type::Type1; }
FontBase::Type MMType1Font::GetFontType() const noexcept { return Type::MMType1; }
FontBase::Type Type3Font::GetFontType() const noexcept { return Type::Type3; }
FontBase::Type TrueTypeFont::GetFontType() const noexcept { return Type::TrueType; }
FontBase::Type CIDFontType0Font::GetFontType() const noexcept { return Type::CIDFontType0; }
FontBase::Type CIDFontType2Font::GetFontType() const noexcept { return Type::CIDFontType2; }

bool FontMap::Contains(const syntax::NameObject& name) const {
	return _obj->Contains(name);
}

FontPtr FontMap::Find(const syntax::NameObject& name) const {
	auto dict = _obj->FindAs<syntax::DictionaryObjectPtr>(name);
	return FontBase::Create(dict);
}

FontBase* FontBase::Create(syntax::DictionaryObjectPtr root) {
	// Verify type, if it is included
	if (root->Contains(constant::Name::Type)) {
		syntax::ObjectPtr type_obj = root->Find(constant::Name::Type);
		if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
			throw GeneralException("Invalid font type object");
		}

		syntax::NameObjectPtr font_type = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);

		if (font_type != constant::Name::Font) {
			throw GeneralException("Invalid font type: " + font_type->ToString());
		}
	}

	if (!root->Contains(constant::Name::Subtype)) {
		throw GeneralException("Dictionary does not contain subtype");
	}

	syntax::ObjectPtr subtype_obj = root->Find(constant::Name::Subtype);

	if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(subtype_obj)) {
		throw GeneralException("Invalid font subtype object");
	}

	syntax::NameObjectPtr subtype = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(subtype_obj);

	if (subtype == constant::Name::Type0) {
		auto result = make_unique<Type0Font>(root);
		return result.release();
	}

	if (subtype == constant::Name::Type1) {
		auto result = make_unique<Type1Font>(root);
		return result.release();
	}

	if (subtype == constant::Name::MMType1) {
		auto result = make_unique<MMType1Font>(root);
		return result.release();
	}

	if (subtype == constant::Name::Type3) {
		auto result = make_unique<Type3Font>(root);
		return result.release();
	}

	if (subtype == constant::Name::TrueType) {
		auto result = make_unique<TrueTypeFont>(root);
		return result.release();
	}

	if (subtype == constant::Name::CIDFontType0) {
		auto result = make_unique<CIDFontType0Font>(root);
		return result.release();
	}

	if (subtype == constant::Name::CIDFontType2) {
		auto result = make_unique<CIDFontType2Font>(root);
		return result.release();
	}

	throw GeneralException("Unknown font subtype: " + subtype->ToString());
}

bool Type0Font::ToUnicode(OuputUnicodeCharacterMapPtr& result) const {
	if (!m_character_map.empty()) {
		result = m_character_map;
		return true;
	}

	if (!_obj->Contains(constant::Name::ToUnicode)) {
		return false;
	}

	auto stream = _obj->FindAs<syntax::StreamObjectPtr>(constant::Name::ToUnicode);
	m_character_map = make_deferred<UnicodeCharacterMap>(stream);
	result = m_character_map;
	return true;
}

} // semantics
} // vanillapdf
