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
#include "object_visitors.h"

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
			IsTypeVisitor<NullObjectPtr> null_visitor;
			IsTypeVisitor<StreamObjectPtr> ref_visitor;
			IsTypeVisitor<ArrayObjectPtr<StreamObjectPtr>> array_visitor;

			ContainableObject content;
			auto found = _obj->TryFind(Name::Contents, content);

			// Missing entry, return empty collection
			if (!found)
				return ContentsPtr();

			bool is_null = content.apply_visitor(null_visitor);
			bool is_ref = content.apply_visitor(ref_visitor);
			bool is_array = content.apply_visitor(array_visitor);

			// Missing entry, return empty collection
			if (is_null) {
				return ContentsPtr();
			}

			// Content shall be stream or array of references to stream
			assert(is_ref ^ is_array);
			if (!(is_ref ^ is_array)) {

				// This is programming safe check
				assert(!(is_ref && is_array) && "Error in object visitors, object is stream and array at the same time");

				ObjectBaseVisitor base_visitor;
				auto base = content.apply_visitor(base_visitor);
				auto base_type_str = Object::TypeName(base->GetType());
				throw Exception("Invalid contents type: " + std::string(base_type_str));
			}

			if (is_ref) {
				ConversionVisitor<StreamObjectPtr> visitor;
				auto data = content.apply_visitor(visitor);
				return ContentsPtr(data);
			}

			if (is_array) {
				ConversionVisitor<ArrayObjectPtr<IndirectObjectReferencePtr>> visitor;
				auto data = content.apply_visitor(visitor);
				return ContentsPtr(data);
			}

			throw Exception("Unreachable code");
		}
	}
}
