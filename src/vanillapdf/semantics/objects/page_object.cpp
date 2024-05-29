#include "precompiled.h"

#include "semantics/objects/page_object.h"
#include "semantics/objects/page_tree_node.h"
#include "semantics/objects/resource_dictionary.h"
#include "semantics/objects/page_contents.h"
#include "semantics/objects/rectangle.h"
#include "semantics/utils/semantic_exceptions.h"
#include "semantics/objects/document.h"
#include "semantics/objects/annotations.h"

#include "syntax/objects/name_object.h"
#include "syntax/objects/integer_object.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

using namespace constant;
using namespace syntax;

PageObject::PageObject(DictionaryObjectPtr obj) : PageNodeBase(obj)	{
	if (_obj->FindAs<NameObjectPtr>(Name::Type) != Name::Page) {
		throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageObject>(obj);
	}
}

bool PageObject::GetResources(OutputResourceDictionaryPtr& result) const {
	if (!_obj->Contains(constant::Name::Resources)) {
		return false;
	}

	auto resources_obj = _obj->FindAs<DictionaryObjectPtr>(constant::Name::Resources);
	auto resources = make_deferred<ResourceDictionary>(resources_obj);
	result = resources;
	return true;
}

void PageObject::SetResources(ResourceDictionaryPtr resources) {
	if (_obj->Contains(Name::Resources)) {
		bool removed = _obj->Remove(Name::Resources);
		assert(removed && "Unable to remove existing item"); UNUSED(removed);
	}

	_obj->Insert(Name::Resources, resources->GetObject());
}

bool PageObject::GetMediaBox(OutputRectanglePtr& result) const {
	if (!_obj->Contains(Name::MediaBox)) {
		return false;
	}

	auto box_obj = _obj->FindAs<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>>(Name::MediaBox);
	auto box = make_deferred<Rectangle>(box_obj);
	result = box;
	return true;
}

void PageObject::SetMediaBox(RectanglePtr media_box) {
	if (_obj->Contains(Name::MediaBox)) {
		bool removed = _obj->Remove(Name::MediaBox);
		assert(removed && "Unable to remove existing item"); UNUSED(removed);
	}

	_obj->Insert(Name::MediaBox, media_box->GetObject());
}

bool PageObject::GetAnnotations(OutputPageAnnotationsPtr& result) const {
	if (!_obj->Contains(constant::Name::Annots)) {
		return false;
	}

	auto annots_obj = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Annots);
	auto annots = make_deferred<PageAnnotations>(annots_obj);
	result = annots;
	return true;
}

void PageObject::SetAnnotations(PageAnnotationsPtr annots) {
	if (_obj->Contains(Name::Annots)) {
		bool removed = _obj->Remove(Name::Annots);
		assert(removed && "Unable to remove existing item"); UNUSED(removed);
	}

	_obj->Insert(Name::Annots, annots->GetObject());
}

void PageObject::SetContents(PageContentsPtr contents) {
	if (_obj->Contains(Name::Contents)) {
		bool removed = _obj->Remove(Name::Contents);
		assert(removed && "Unable to remove existing item"); UNUSED(removed);
	}

	IndirectReferenceObjectPtr contents_ref = make_deferred<IndirectReferenceObject>(contents->GetObject());
	_obj->Insert(Name::Contents, contents_ref);
}

bool PageObject::GetContents(OutputPageContentsPtr& result) const {
	if (!m_contents.empty()) {
		result = m_contents;
		return true;
	}

	OutputContainableObjectPtr content;
	auto found = _obj->TryFind(Name::Contents, content);

	// Missing entry, return empty collection
	if (!found) {
		return false;
	}

	bool is_null = ObjectUtils::IsType<NullObjectPtr>(*content);
	bool is_ref = ObjectUtils::IsType<StreamObjectPtr>(*content);
	bool is_array = ObjectUtils::IsType<ArrayObjectPtr<StreamObjectPtr>>(*content);

	// Missing entry, return empty collection
	if (is_null) {
		return false;
	}

	// Content shall be stream or array of references to stream
	assert(is_ref ^ is_array);
	if (!(is_ref ^ is_array)) {

		// This is programming safe check
		assert(!(is_ref && is_array) && "Error in object utils, object is stream and array at the same time");

		auto base_type_str = Object::TypeName(content->GetObjectType());
		throw GeneralException("Invalid contents type: " + std::string(base_type_str));
	}

	if (is_ref) {
		auto data = ObjectUtils::ConvertTo<StreamObjectPtr>(*content);
		PageContentsPtr contents = make_deferred<PageContents>(data);
		m_contents = contents;
		result = contents;
		return true;
	}

	if (is_array) {
		auto data = ObjectUtils::ConvertTo<ArrayObjectPtr<IndirectReferenceObjectPtr>>(*content);
		PageContentsPtr contents = make_deferred<PageContents>(data);
		m_contents = contents;
		result = contents;
		return true;
	}

	throw GeneralException("Unreachable code");
}

std::unique_ptr<PageObject> PageObject::Create(DocumentPtr document) {
	auto file = document->GetFile();

	syntax::DictionaryObjectPtr obj;
	obj->Insert(Name::Type, Name::Page.Clone());

	XrefUsedEntryBasePtr new_entry = file->AllocateNewEntry();
	new_entry->SetReference(obj);
	new_entry->SetFile(file);
	new_entry->SetInitialized();

	obj->SetFile(file);
	obj->SetInitialized();

	return make_unique<PageObject>(obj);
}

std::unique_ptr<PageObject> PageObject::Create(syntax::DictionaryObjectPtr obj) {
	return make_unique<PageObject>(obj);
}

bool PageObjectExtensions::GetInheritableResources(const PageObject& obj, OutputResourceDictionaryPtr& result) {
	auto page_dictionary = obj.GetObject();
	if (!page_dictionary->Contains(constant::Name::Resources)) {
		if (!page_dictionary->Contains(constant::Name::Parent)) {
			return false;
		}

		auto parent_dictionary_obj = page_dictionary->FindAs<DictionaryObjectPtr>(constant::Name::Parent);
		auto parent_obj = PageObject::Create(parent_dictionary_obj);
		return GetInheritableResources(*parent_obj, result);
	}

	auto resources_obj = page_dictionary->FindAs<DictionaryObjectPtr>(constant::Name::Resources);
	auto resources = make_deferred<ResourceDictionary>(resources_obj);
	result = resources;
	return true;
}

} // semantics
} // vanillapdf
