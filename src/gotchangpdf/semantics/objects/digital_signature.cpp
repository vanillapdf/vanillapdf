#include "precompiled.h"

#include "semantics/objects/digital_signature.h"
#include "semantics/utils/semantic_exceptions.h"

namespace gotchangpdf {
namespace semantics {

DigitalSignature::DigitalSignature(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
ByteRangeCollection::ByteRangeCollection(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> obj) : HighLevelObject(obj) {
	assert(obj->Size() % 2 == 0);
	if (obj->Size() % 2 != 0) {
		throw SemanticContextExceptionFactory::Construct<syntax::ArrayObject<syntax::IntegerObjectPtr>, PageRange>(obj);
	}
}

ByteRange::ByteRange(syntax::IntegerObjectPtr byte_offset, syntax::IntegerObjectPtr length)
	: m_byte_offset(byte_offset), m_length(length) {
}

syntax::IntegerObjectPtr ByteRange::ByteOffset(void) const {
	return m_byte_offset;
}

syntax::IntegerObjectPtr ByteRange::Length(void) const {
	return m_length;
}

types::uinteger ByteRangeCollection::Size(void) const {
	return _obj->Size() / 2;
}

ByteRangePtr ByteRangeCollection::At(types::uinteger at) const {
	return ByteRangePtr(_obj->At(at), _obj->At(at + 1));
}

syntax::HexadecimalStringObjectPtr DigitalSignature::Contents() {
	return _obj->FindAs<syntax::HexadecimalStringObjectPtr>(constant::Name::Contents);
}

syntax::NameObjectPtr DigitalSignature::Filter() {
	return _obj->FindAs<syntax::NameObjectPtr>(constant::Name::Filter);
}

bool DigitalSignature::ByteRange(OuputByteRangeCollectionPtr& result) {
	if (!_obj->Contains(constant::Name::ByteRange)) {
		return false;
	}

	auto ranges_array = _obj->FindAs<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>>(constant::Name::ByteRange);
	auto ranges_obj = ByteRangeCollectionPtr(ranges_array);
	result = ranges_obj;
	return true;
}

bool DigitalSignature::Reason(syntax::OutputStringObjectPtr& result) {
	if (!_obj->Contains(constant::Name::Reason)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Reason);
	return true;
}

bool DigitalSignature::Location(syntax::OutputStringObjectPtr& result) {
	if (!_obj->Contains(constant::Name::Location)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Location);
	return true;
}

bool DigitalSignature::ContactInfo(syntax::OutputStringObjectPtr& result) {
	if (!_obj->Contains(constant::Name::ContactInfo)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::ContactInfo);
	return true;
}

bool DigitalSignature::Date(OutputDatePtr& result) {
	if (!_obj->Contains(constant::Name::M)) {
		return false;
	}

	auto date_string = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::M);
	auto date_obj = DatePtr(date_string);
	result = date_obj;
	return true;
}

bool DigitalSignature::Name(syntax::OutputStringObjectPtr& result) {
	if (!_obj->Contains(constant::Name::Name)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Name);
	return true;
}

bool DigitalSignature::Revision(syntax::OutputIntegerObjectPtr& result) {
	if (!_obj->Contains(constant::Name::R)) {
		return false;
	}

	result = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::R);
	return true;
}

bool DigitalSignature::SubFilter(syntax::OutputNameObjectPtr& result) {
	if (!_obj->Contains(constant::Name::SubFilter)) {
		return false;
	}

	result = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::SubFilter);
	return true;
}

bool DigitalSignature::Certificate(syntax::OutputStringObjectPtr& result) {
	if (!_obj->Contains(constant::Name::Cert)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Cert);
	return true;
}

} // semantics
} // gotchangpdf
