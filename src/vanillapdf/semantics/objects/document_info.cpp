#include "precompiled.h"

#include "utils/misc_utils.h"
#include "syntax/files/file.h"

#include "semantics/objects/document_info.h"
#include "semantics/objects/date.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

DocumentInfo::DocumentInfo(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {
}

bool DocumentInfo::Title(syntax::OutputStringObjectPtr& result) const {
	if (!_obj->Contains(constant::Name::Title)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Title);
	return true;
}

bool DocumentInfo::Author(syntax::OutputStringObjectPtr& result) const {
	if (!_obj->Contains(constant::Name::Author)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Author);
	return true;
}

bool DocumentInfo::Subject(syntax::OutputStringObjectPtr& result) const {
	if (!_obj->Contains(constant::Name::Subject)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Subject);
	return true;
}

bool DocumentInfo::Keywords(syntax::OutputStringObjectPtr& result) const {
	if (!_obj->Contains(constant::Name::Keywords)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Keywords);
	return true;
}

bool DocumentInfo::Creator(syntax::OutputStringObjectPtr& result) const {
	if (!_obj->Contains(constant::Name::Creator)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Creator);
	return true;
}

bool DocumentInfo::Producer(syntax::OutputStringObjectPtr& result) const {
	if (!_obj->Contains(constant::Name::Producer)) {
		return false;
	}

	result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Producer);
	return true;
}

bool DocumentInfo::CreationDate(OutputDatePtr& result) const {
	if (!_obj->Contains(constant::Name::CreationDate)) {
		return false;
	}

	try {
		auto date_string = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::CreationDate);
		result = make_deferred<Date>(date_string);
		return true;
	}
	catch (GeneralException& ex) {
		auto file = _obj->GetFile();
		auto locked_file = file.GetReference();
		auto filename = locked_file->GetFilenameString();
		LOG_ERROR(filename) << "Error parsing datetime: " << ex.what();
		return false;
	}
}

bool DocumentInfo::ModificationDate(OutputDatePtr& result) const {
	if (!_obj->Contains(constant::Name::ModDate)) {
		return false;
	}

	auto date_string = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::ModDate);
	result = make_deferred<Date>(date_string);
	return true;
}

bool DocumentInfo::Trapped(DocumentTrapped& result) const {
	if (!_obj->Contains(constant::Name::Trapped)) {
		return false;
	}

	std::string trapped_value;
	auto trapped = _obj->Find(constant::Name::Trapped);
	if (syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(trapped)) {
		auto name = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(trapped);
		auto name_buffer = name->GetValue();
		trapped_value = name_buffer->ToString();
	}

	if (syntax::ObjectUtils::IsType<syntax::StringObjectPtr>(trapped)) {
		auto string = syntax::ObjectUtils::ConvertTo<syntax::StringObjectPtr>(trapped);
		auto string_buffer = string->GetValue();
		trapped_value = string_buffer->ToString();
	}

	auto true_buffer = constant::Name::True.GetValue();
	auto false_buffer = constant::Name::False.GetValue();
	auto unknown_buffer = constant::Name::Unknown.GetValue();

	auto true_string = true_buffer->ToString();
	auto false_string = false_buffer->ToString();
	auto unknown_string = unknown_buffer->ToString();

	if (MiscUtils::CaseInsensitiveCompare(trapped_value, true_string)) {
		result = DocumentTrapped::True;
		return true;
	}

	if (MiscUtils::CaseInsensitiveCompare(trapped_value, false_string)) {
		result = DocumentTrapped::False;
		return true;
	}

	if (MiscUtils::CaseInsensitiveCompare(trapped_value, unknown_string)) {
		result = DocumentTrapped::Unknown;
		return true;
	}

	assert(false && "Document trapped is neither of name or string");
	return false;
}

void DocumentInfo::SetTitle(syntax::StringObjectPtr value) {
	_obj->Insert(constant::Name::Title, value);
}

void DocumentInfo::SetAuthor(syntax::StringObjectPtr value) {
	_obj->Insert(constant::Name::Author, value);
}

void DocumentInfo::SetSubject(syntax::StringObjectPtr value) {
	_obj->Insert(constant::Name::Subject, value);
}

void DocumentInfo::SetKeywords(syntax::StringObjectPtr value) {
	_obj->Insert(constant::Name::Keywords, value);
}

void DocumentInfo::SetCreator(syntax::StringObjectPtr value) {
	_obj->Insert(constant::Name::Creator, value);
}

void DocumentInfo::SetProducer(syntax::StringObjectPtr value) {
	_obj->Insert(constant::Name::Producer, value);
}

void DocumentInfo::SetCreationDate(DatePtr value) {
	_obj->Insert(constant::Name::CreationDate, value->GetObject());
}

void DocumentInfo::SetModificationDate(DatePtr value) {
	_obj->Insert(constant::Name::ModDate, value->GetObject());
}

void DocumentInfo::SetTrapped(DocumentTrapped value) {

	syntax::NameObjectPtr name;

	switch (value) {
		case DocumentTrapped::Unknown:
			name = syntax::ObjectUtils::Clone<syntax::NameObjectPtr>(constant::Name::Unknown);
			break;
		case DocumentTrapped::True:
			name = syntax::ObjectUtils::Clone<syntax::NameObjectPtr>(constant::Name::True);
			break;
		case DocumentTrapped::False:
			name = syntax::ObjectUtils::Clone<syntax::NameObjectPtr>(constant::Name::False);
			break;
		default:
			throw GeneralException("Unknown trapped type");
	}

	_obj->Insert(constant::Name::Trapped, name);
}

} // semantics
} // vanillapdf
