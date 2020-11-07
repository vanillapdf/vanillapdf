#include "precompiled.h"

#include "utils/character.h"

#include "syntax/objects/name_object.h"
#include "utils/streams/output_stream_interface.h"

#include <sstream>

namespace vanillapdf {
namespace syntax {

NameObject::NameObject() {
	_value->Subscribe(this);
}

NameObject::NameObject(const char * value)
	: NameObject(make_deferred_container<Buffer>(value)) {
}

NameObject::NameObject(const std::string& chars)
	: NameObject(make_deferred_container<Buffer>(chars)) {

}

NameObject::NameObject(BufferPtr name) : _value(name) {
	_value->Subscribe(this);
	_value->SetInitialized();
}

size_t NameObject::Hash() const {
	return _value->Hash();
}

void NameObject::ObserveeChanged(const IModifyObservable*) {
	OnChanged();
}

BufferPtr NameObject::GetValue() const {
	return _value;
}

void NameObject::SetValue(BufferPtr value) {
	_value->assign(value.begin(), value.end());
}

bool NameObject::Equals(const NameObject& other) const {
	return _value->Equals(other._value);
}

bool NameObject::Equals(ObjectPtr other) const {
	if (!ObjectUtils::IsType<NameObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<NameObjectPtr>(other);
	return Equals(*other_obj);
}

Object::Type NameObject::GetObjectType(void) const noexcept {
	return Object::Type::Name;
}

void NameObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
	output->Write("/");
	output->Write(ToString());
}

NameObject* NameObject::Clone(void) const {
	NameObjectPtr result(pdf_new NameObject(), false);
	result->SetValue(_value->Clone());

	CloneBaseProperties(result);
	return result.detach();
}

NameObject::~NameObject() {
	_value->Unsubscribe(this);
}

std::string NameObject::GetHexadecimalNotation(char ch) const {
	std::stringstream ss;
	int converted = static_cast<int>(ch);
	ss << '#';
	ss << std::hex << converted;

	return ss.str();
}

std::string NameObject::ToString(void) const {
	std::stringstream ss;
	auto size = _value->size();
	for (decltype(size) i = 0; i < size; ++i) {
		auto current = _value[i];

		if ('#' == current) {
			ss << GetHexadecimalNotation(current);
			continue;
		}

		if (!IsRegular(current)) {
			ss << GetHexadecimalNotation(current);
			continue;
		}

		if (current < '!' || current > '~') {
			/* Regular characters that are outside the range
			EXCLAMATION MARK(21h) (!) to TILDE (7Eh) (~)
			should be written using the hexadecimal notation */

			ss << GetHexadecimalNotation(current);
			continue;
		}

		ss << current;
	}

	return ss.str();
}

} // syntax

bool operator==(const syntax::NameObject& left, const syntax::NameObject& right) {
	return (left.GetValue() == right.GetValue());
}

bool operator!=(const syntax::NameObject& left, const syntax::NameObject& right) {
	return (left.GetValue() != right.GetValue());
}

bool operator<(const syntax::NameObject& left, const syntax::NameObject& right) {
	return (left.GetValue() < right.GetValue());
}

bool operator==(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right) {
	return (*left == *right);
}

bool operator!=(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right) {
	return (*left != *right);
}

bool operator<(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right) {
	return (*left < *right);
}

} // vanillapdf

namespace std {
size_t hash<vanillapdf::syntax::NameObject>::operator()(const vanillapdf::syntax::NameObject& name) const {
	return name.Hash();
}

} // std
