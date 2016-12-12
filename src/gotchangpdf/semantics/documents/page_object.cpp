#include "precompiled.h"
#include "page_object.h"

#include "page_tree_node.h"
#include "resource_dictionary.h"
#include "contents.h"
#include "rectangle.h"
#include "name_object.h"
#include "integer_object.h"
#include "semantic_exceptions.h"
#include "document.h"
#include "annotations.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace constant;
		using namespace syntax;

		PageObject::PageObject(DictionaryObjectPtr obj) :
			PageNodeBase(obj)
		{
			if (_obj->FindAs<NameObjectPtr>(Name::Type) != Name::Page)
				throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageObject>(obj);
		}

		PageTreeNodePtr PageObject::GetParent() const
		{
			auto parent = _obj->FindAs<DictionaryObjectPtr>(Name::Parent);
			return PageTreeNodePtr(parent);
		}

		void PageObject::SetParent(PageTreeNodePtr parent)
		{
			if (_obj->Contains(Name::Parent)) {
				bool removed = _obj->Remove(Name::Parent);
				assert(removed && "Unable to remove existing item"); removed;
			}

			IndirectObjectReferencePtr parent_ref(parent->GetObject());
			_obj->Insert(Name::Parent, parent_ref);
		}

		ResourceDictionaryPtr PageObject::GetResources() const
		{
			auto resource = _obj->FindAs<DictionaryObjectPtr>(Name::Resources);
			return ResourceDictionaryPtr(resource);
		}

		void PageObject::SetResources(ResourceDictionaryPtr resources)
		{
			if (_obj->Contains(Name::Resources)) {
				bool removed = _obj->Remove(Name::Resources);
				assert(removed && "Unable to remove existing item"); removed;
			}

			_obj->Insert(Name::Resources, resources->GetObject());
		}

		RectanglePtr PageObject::GetMediaBox() const
		{
			if (!_obj->Contains(Name::MediaBox)) {
				auto parent = GetParent();
				auto parent_dictionary = parent->GetObject();
				if (!parent_dictionary->Contains(Name::MediaBox)) {
					throw GeneralException("Media box is missing");
				}

				auto box = parent_dictionary->FindAs<MixedArrayObjectPtr>(Name::MediaBox);
				return RectanglePtr(box);
			}

			auto box = _obj->FindAs<MixedArrayObjectPtr>(Name::MediaBox);
			return RectanglePtr(box);
		}

		void PageObject::SetMediaBox(RectanglePtr media_box)
		{
			if (_obj->Contains(Name::MediaBox)) {
				bool removed = _obj->Remove(Name::MediaBox);
				assert(removed && "Unable to remove existing item"); removed;
			}

			_obj->Insert(Name::MediaBox, media_box->GetObject()->Data());
		}

		bool PageObject::GetAnnotations(OutputPageAnnotationsPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Annots))
				return false;

			auto annots_obj = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Annots);
			auto annots = PageAnnotationsPtr(annots_obj);
			result = annots;
			return true;
		}

		void PageObject::SetAnnotations(PageAnnotationsPtr annots)
		{
			if (_obj->Contains(Name::Annots)) {
				bool removed = _obj->Remove(Name::Annots);
				assert(removed && "Unable to remove existing item"); removed;
			}

			_obj->Insert(Name::Annots, annots->GetObject());
		}

		void PageObject::SetContents(ContentsPtr contents)
		{
			if (_obj->Contains(Name::Contents)) {
				bool removed = _obj->Remove(Name::Contents);
				assert(removed && "Unable to remove existing item"); removed;
			}

			IndirectObjectReferencePtr contents_ref(contents->GetObject());
			_obj->Insert(Name::Contents, contents_ref);
		}

		bool PageObject::GetContents(OutputContentsPtr& result) const
		{
			if (!m_contents.empty()) {
				result = m_contents;
				return true;
			}

			OutputContainableObjectPtr output;
			auto found = _obj->TryFind(Name::Contents, output);

			// Missing entry, return empty collection
			if (!found)
				return false;

			ContainableObjectPtr content = output.GetValue();
			bool is_null = ObjectUtils::IsType<NullObjectPtr>(content);
			bool is_ref = ObjectUtils::IsType<StreamObjectPtr>(content);
			bool is_array = ObjectUtils::IsType<ArrayObjectPtr<StreamObjectPtr>>(content);

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
				auto data = ObjectUtils::ConvertTo<StreamObjectPtr>(content);
				ContentsPtr contents = ContentsPtr(data);
				m_contents = contents;
				result = contents;
				return true;
			}

			if (is_array) {
				auto data = ObjectUtils::ConvertTo<ArrayObjectPtr<IndirectObjectReferencePtr>>(content);
				ContentsPtr contents = ContentsPtr(data);
				m_contents = contents;
				result = contents;
				return true;
			}

			throw GeneralException("Unreachable code");
		}

		std::unique_ptr<PageObject> PageObject::Create(DocumentPtr document)
		{
			auto file = document->GetFile()->Value();
			auto xref_chain = file->GetXrefChain();

			syntax::DictionaryObjectPtr obj;
			obj->Insert(Name::Type, Name::Page);
			
			XrefUsedEntryBasePtr new_entry = xref_chain->AllocateNewEntry();
			new_entry->SetReference(obj);
			new_entry->SetFile(file);
			new_entry->SetInitialized();

			obj->SetFile(file);
			obj->SetInitialized();

			return make_unique<PageObject>(obj);
		}

		std::unique_ptr<PageObject> PageObject::Create(syntax::DictionaryObjectPtr obj)
		{
			return make_unique<PageObject>(obj);
		}
	}
}
