#include "precompiled.h"

#include "semantics/objects/page_object.h"
#include "semantics/objects/page_tree_node.h"
#include "semantics/objects/resource_dictionary.h"
#include "semantics/objects/contents.h"
#include "semantics/objects/rectangle.h"
#include "semantics/utils/semantic_exceptions.h"
#include "semantics/objects/document.h"
#include "semantics/objects/annotations.h"

#include "syntax/objects/name_object.h"
#include "syntax/objects/integer_object.h"

namespace vanillapdf {
namespace semantics {

using namespace constant;
using namespace syntax;

PageObject::PageObject(DictionaryObjectPtr obj) : PageNodeBase(obj)	{
	if (_obj->FindAs<NameObjectPtr>(Name::Type) != Name::Page) {
		throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageObject>(obj);
	}
}

ResourceDictionaryPtr PageObject::GetResources() const {
	auto resource = _obj->FindAs<DictionaryObjectPtr>(Name::Resources);
	return make_deferred<ResourceDictionary>(resource);
}

void PageObject::SetResources(ResourceDictionaryPtr resources) {
	if (_obj->Contains(Name::Resources)) {
		bool removed = _obj->Remove(Name::Resources);
		assert(removed && "Unable to remove existing item"); UNUSED(removed);
	}

	_obj->Insert(Name::Resources, resources->GetObject());
}

RectanglePtr PageObject::GetMediaBox() const {
	if (!_obj->Contains(Name::MediaBox)) {
		auto parent = GetParent();
		auto parent_dictionary = parent->GetObject();
		if (!parent_dictionary->Contains(Name::MediaBox)) {
			throw GeneralException("Media box is missing");
		}

		auto box = parent_dictionary->FindAs<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>>(Name::MediaBox);
		return make_deferred<Rectangle>(box);
	}

	auto box = _obj->FindAs<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>>(Name::MediaBox);
	return make_deferred<Rectangle>(box);
}

void PageObject::SetMediaBox(RectanglePtr media_box) {
	if (_obj->Contains(Name::MediaBox)) {
		bool removed = _obj->Remove(Name::MediaBox);
		assert(removed && "Unable to remove existing item"); UNUSED(removed);
	}

	_obj->Insert(Name::MediaBox, media_box->GetObject()->Data());
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

void PageObject::SetContents(ContentsPtr contents) {
	if (_obj->Contains(Name::Contents)) {
		bool removed = _obj->Remove(Name::Contents);
		assert(removed && "Unable to remove existing item"); UNUSED(removed);
	}

	IndirectObjectReferencePtr contents_ref = make_deferred<IndirectObjectReference>(contents->GetObject());
	_obj->Insert(Name::Contents, contents_ref);
}

bool PageObject::GetContents(OutputContentsPtr& result) const {
	if (!m_contents.empty()) {
		result = m_contents.GetValue();
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

		auto base_type_str = Object::TypeName(content->GetType());
		throw GeneralException("Invalid contents type: " + std::string(base_type_str));
	}

	if (is_ref) {
		auto data = ObjectUtils::ConvertTo<StreamObjectPtr>(*content);
		ContentsPtr contents = make_deferred<Contents>(data);
		m_contents = contents;
		result = contents;
		return true;
	}

	if (is_array) {
		auto data = ObjectUtils::ConvertTo<ArrayObjectPtr<IndirectObjectReferencePtr>>(*content);
		ContentsPtr contents = make_deferred<Contents>(data);
		m_contents = contents;
		result = contents;
		return true;
	}

	throw GeneralException("Unreachable code");
}

std::unique_ptr<PageObject> PageObject::Create(DocumentPtr document) {
	auto file = document->GetFile();
	auto xref_chain = file->GetXrefChain();

	syntax::DictionaryObjectPtr obj;
	obj->Insert(Name::Type, Name::Page.Clone());

	XrefUsedEntryBasePtr new_entry = xref_chain->AllocateNewEntry();
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

} // semantics
} // vanillapdf
