#include "precompiled.h"
#include "page_object.h"

#include "page_tree_node.h"
#include "resource_dictionary.h"
#include "contents.h"
#include "rectangle.h"
#include "name_object.h"
#include "integer_object.h"
#include "semantic_exceptions.h"

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
	}
}
