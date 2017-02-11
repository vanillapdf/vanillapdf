#include "precompiled.h"
#include "semantics/objects/font.h"

namespace gotchangpdf {
namespace semantics {

FontMap::FontMap(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {}
FontBase::FontBase(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
Type1Font::Type1Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
Type3Font::Type3Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
CompositeFont::CompositeFont(syntax::DictionaryObjectPtr root) : FontBase(root) {}

FontBase::Type Type1Font::GetType() const noexcept { return Type::Type1; }
FontBase::Type Type3Font::GetType() const noexcept { return Type::Type3; }
FontBase::Type CompositeFont::GetType() const noexcept { return Type::Composite; }

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
			throw GeneralException("Invalid font type");
		}

		syntax::NameObjectPtr type = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);

		if (type != constant::Name::Font) {
			throw GeneralException("Invalid font type");
		}
	}

	if (!root->Contains(constant::Name::Subtype)) {
		throw GeneralException("Dictionary does not contain subtype");
	}

	syntax::ObjectPtr subtype_obj = root->Find(constant::Name::Subtype);

	if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(subtype_obj)) {
		throw GeneralException("Invalid annotation subtype");
	}

	syntax::NameObjectPtr subtype = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(subtype_obj);

	if (subtype == constant::Name::Type0) {
		auto result = make_unique<CompositeFont>(root);
		return result.release();
	}

	if (subtype == constant::Name::Type1) {
		auto result = make_unique<Type1Font>(root);
		return result.release();
	}

	if (subtype == constant::Name::Type3) {
		auto result = make_unique<Type3Font>(root);
		return result.release();
	}

	throw GeneralException("Unknown font subtype");
}

bool CompositeFont::ToUnicode(OuputUnicodeCharacterMapPtr& result) const {
	if (!_obj->Contains(constant::Name::ToUnicode)) {
		return false;
	}

	auto stream = _obj->FindAs<syntax::StreamObjectPtr>(constant::Name::ToUnicode);
	result = UnicodeCharacterMapPtr(stream);
	return true;
}

} // semantics
} // gotchangpdf
