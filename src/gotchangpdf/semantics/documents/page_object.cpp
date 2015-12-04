#include "precompiled.h"
#include "page_object.h"

#include "page_tree_node.h"
#include "resource_dictionary.h"
#include "content_stream.h"
#include "contents.h"
#include "rectangle.h"
#include "name_object.h"
#include "integer_object.h"
#include "exception.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace constant;
		using namespace syntax;

		PageObject::PageObject(DictionaryObjectPtr obj) :
			PageNodeBase(obj)
		{
			if (*_obj->FindAs<NameObjectPtr>(Name::Type) != Name::Page)
				throw Exception("TODO");
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
			auto specialized = box->CastToArrayType<IntegerObjectPtr>();
			return RectanglePtr(ArrayObjectPtr<IntegerObjectPtr>(specialized));
		}

		ContentsPtr PageObject::Contents() const
		{
			ContainableObjectPtr content;
			auto found = _obj->TryFind(Name::Contents, content);

			// Missing entry, return empty collection
			if (!found)
				return ContentsPtr();

			bool is_null = ObjectUtils::IsType<NullObjectPtr>(content);
			bool is_ref = ObjectUtils::IsType<StreamObjectPtr>(content);
			bool is_array = ObjectUtils::IsType<ArrayObjectPtr<StreamObjectPtr>>(content);

			// Missing entry, return empty collection
			if (is_null) {
				return ContentsPtr();
			}

			// Content shall be stream or array of references to stream
			assert(is_ref ^ is_array);
			if (!(is_ref ^ is_array)) {

				// This is programming safe check
				assert(!(is_ref && is_array) && "Error in object utils, object is stream and array at the same time");

				auto base_type_str = Object::TypeName(content->GetType());
				throw Exception("Invalid contents type: " + std::string(base_type_str));
			}

			if (is_ref) {
				auto data = ObjectUtils::ConvertTo<StreamObjectPtr>(content);
				return ContentsPtr(data);
			}

			if (is_array) {
				auto data = ObjectUtils::ConvertTo<ArrayObjectPtr<IndirectObjectReferencePtr>>(content);
				return ContentsPtr(data);
			}

			throw Exception("Unreachable code");
		}
	}
}
