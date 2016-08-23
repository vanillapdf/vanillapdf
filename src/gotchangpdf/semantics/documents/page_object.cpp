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

		PageTreeNodePtr PageObject::Parent() const
		{
			return PageTreeNodePtr(_obj->FindAs<DictionaryObjectPtr>(Name::Parent));
		}

		ResourceDictionaryPtr PageObject::Resources() const
		{
			auto resource = _obj->FindAs<DictionaryObjectPtr>(Name::Resources);
			return ResourceDictionaryPtr(resource);
		}

		RectanglePtr PageObject::MediaBox() const
		{
			auto box = _obj->FindAs<MixedArrayObjectPtr>(Name::MediaBox);
			return RectanglePtr(box);
		}

		bool PageObject::Contents(OutputContentsPtr& result) const
		{
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
				result = ContentsPtr(data);
				return true;
			}

			if (is_array) {
				auto data = ObjectUtils::ConvertTo<ArrayObjectPtr<IndirectObjectReferencePtr>>(content);
				result = ContentsPtr(data);
				return true;
			}

			throw GeneralException("Unreachable code");
		}

		std::unique_ptr<PageObject> PageObject::Create(DocumentPtr document)
		{
			auto file = document->GetFile()->Value();
			auto xref_chain = file->GetXrefChain();

			syntax::DictionaryObjectPtr obj;
			obj->SetFile(file);
			obj->Insert(Name::Type, Name::Page);
			obj->Insert(Name::Contents, IndirectObjectReferencePtr(145, (types::ushort)0));
			obj->Insert(Name::Parent, IndirectObjectReferencePtr(3, (types::ushort)0));

			syntax::DictionaryObjectPtr resources;
			obj->Insert(Name::Resources, resources);
			
			XrefEntryBasePtr new_entry = xref_chain->AllocateNewEntry();
			if (XrefUtils::IsType<XrefUsedEntryPtr>(new_entry)) {
				auto used_entry = XrefUtils::ConvertTo<XrefUsedEntryPtr>(new_entry);
				used_entry->SetReference(obj);
				used_entry->SetFile(file);
				used_entry->SetInitialized(true);
			}

			if (XrefUtils::IsType<XrefCompressedEntryPtr>(new_entry)) {
				auto compressed_entry = XrefUtils::ConvertTo<XrefCompressedEntryPtr>(new_entry);
				compressed_entry->SetReference(obj);
				compressed_entry->SetFile(file);
				compressed_entry->SetInitialized(true);
			}

			obj->SetObjectNumber(new_entry->GetObjectNumber());
			obj->SetGenerationNumber(new_entry->GetGenerationNumber());
			obj->SetIndirect(true);
			obj->SetInitialized(true);

			return std::make_unique<PageObject>(obj);
		}

		std::unique_ptr<PageObject> PageObject::Create(syntax::DictionaryObjectPtr obj)
		{
			return std::make_unique<PageObject>(obj);
		}
	}
}
