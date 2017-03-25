#include "precompiled.h"

#include "semantics/objects/page_labels.h"
#include "semantics/utils/semantic_exceptions.h"

#include "syntax/objects/name_object.h"
#include "syntax/utils/object_utils.h"

namespace gotchangpdf {
namespace semantics {

PageLabel::PageLabel(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {
	if (_obj->Contains(constant::Name::Type)) {
		auto type = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::Type);
		if (type != constant::Name::PageLabel) {
			throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageLabel>(obj);
		}
	}
}

bool PageLabel::Prefix(syntax::StringObjectPtr& result) const {
	if (!_obj->Contains(constant::Name::P)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::P);
	return true;
}

bool PageLabel::Start(syntax::IntegerObjectPtr& result) const {
	if (!_obj->Contains(constant::Name::St)) {
		return false;
	}

	result = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::St);
	return true;
}

bool PageLabel::Style(NumberingStyle& result) const {
	if (!_obj->Contains(constant::Name::S)) {
		return false;
	}

	auto value = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::S);
	auto buf = value->GetValue();
	if (buf->size() != 1) {
		throw GeneralException("Unknown numbering style" + buf->ToString());
	}

	if (buf[0] != static_cast<char>(NumberingStyle::Decimal)
		&& buf[0] != static_cast<char>(NumberingStyle::UpperRoman)
		&& buf[0] != static_cast<char>(NumberingStyle::LowerRoman)
		&& buf[0] != static_cast<char>(NumberingStyle::UpperLetters)
		&& buf[0] != static_cast<char>(NumberingStyle::LowerLetters)) {
		throw GeneralException("Unknown numbering style" + buf->ToString());
	}

	result = static_cast<NumberingStyle>(buf[0]);
	return true;
}

PageLabels::PageLabels(const syntax::DictionaryObjectPtr& obj)
	: HighLevelObject(obj),
	_tree(make_deferred<NumberTree<PageLabelPtr>>(obj, &ConvertFunction)) {
}

bool PageLabels::Contains(types::integer page_number) const {
	syntax::IntegerObjectPtr value = make_deferred<syntax::IntegerObject>(page_number);
	return _tree->Contains(value);
}

PageLabelPtr PageLabels::Find(types::integer page_number) const {
	syntax::IntegerObjectPtr value = make_deferred<syntax::IntegerObject>(page_number);
	return _tree->Find(value);
}

PageLabelPtr PageLabels::ConvertFunction(const syntax::ContainableObjectPtr& item) {
	auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(item);
	return make_deferred<PageLabel>(dict);
}

} // semantics

bool operator==(const semantics::PageLabelPtr& left, const semantics::PageLabelPtr& right) {
	return (left.get() == right.get());
}

bool operator!=(const semantics::PageLabelPtr& left, const semantics::PageLabelPtr& right) {
	return (left.get() != right.get());
}

bool operator<(const semantics::PageLabelPtr& left, const semantics::PageLabelPtr& right) {
	return (left.get() < right.get());
}

} // gotchangpdf
