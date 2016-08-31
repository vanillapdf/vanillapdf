#include "precompiled.h"
#include "annotations.h"
#include "destinations.h"
#include "document.h"

#include "exception.h"
#include "semantic_exceptions.h"
#include "semantic_utils.h"

namespace gotchangpdf
{
	namespace semantics
	{
		AnnotationBase::AnnotationBase(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
		LinkAnnotation::LinkAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
		PageAnnotations::PageAnnotations(syntax::MixedArrayObjectPtr root) : HighLevelObject(root) {}
		AnnotationBase::~AnnotationBase() {}

		AnnotationBase::Type LinkAnnotation::GetType() const noexcept { return Type::Link; }

		types::uinteger PageAnnotations::Size() const { return _obj->Size(); }
		AnnotationPtr PageAnnotations::At(types::uinteger index) const
		{
			auto obj = _obj->At(index);

			assert(syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(obj) && "Linked annotation is not dictionary");
			if (!syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(obj)) {
				throw GeneralException("Invalid annotation reference");
			}

			auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(obj);
			return AnnotationBase::Create(dict, GetDocument());
		}

		AnnotationBase* AnnotationBase::Create(syntax::DictionaryObjectPtr root, WeakReference<Document> doc)
		{
			if (!root->Contains(constant::Name::Type)) {
				throw GeneralException("Dictionary does not contain type");
			}

			if (!root->Contains(constant::Name::Subtype)) {
				throw GeneralException("Dictionary does not contain subtype");
			}

			syntax::ObjectPtr type_obj = root->Find(constant::Name::Type);
			syntax::ObjectPtr subtype_obj = root->Find(constant::Name::Subtype);

			if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
				throw GeneralException("Invalid annotation type");
			}

			if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(subtype_obj)) {
				throw GeneralException("Invalid annotation subtype");
			}

			syntax::NameObjectPtr type = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);
			syntax::NameObjectPtr subtype = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(subtype_obj);

			if (type != constant::Name::Annot) {
				throw GeneralException("Invalid annotation type");
			}

			if (subtype == constant::Name::Link) {
				std::unique_ptr<LinkAnnotation> result = std::make_unique<LinkAnnotation>(root);
				result->SetDocument(doc);
				return result.release();
			}

			throw GeneralException("Unknown annotation subtype");
		}

		bool LinkAnnotation::Destination(OutputDestinationPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Dest)) {
				return false;
			}

			auto dest_obj = _obj->Find(constant::Name::Dest);
			if (syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(dest_obj)) {
				auto document_ref = GetDocument();

				assert(!document_ref.IsEmpty() && "Document reference was not set");
				if (!document_ref.IsActive()) {
					return false;
				}

				DocumentPtr document = document_ref.GetReference();
				auto catalog = document->GetDocumentCatalog();

				OutputNamedDestinationsPtr destinations_ptr;
				bool has_destinations = catalog->Destinations(destinations_ptr);
				if (!has_destinations) {
					return false;
				}

				auto destinations = destinations_ptr.GetValue();

				auto destination_name = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(dest_obj);

				assert(destinations->Contains(destination_name) && "Referenced destination does not exist");
				if (!destinations->Contains(destination_name)) {
					return false;
				}

				result = destinations->Find(destination_name);
				return true;
			}

			if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(dest_obj)) {
				auto array_obj = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(dest_obj);
				result = DestinationBase::Create(array_obj, GetDocument());
				return true;
			}

			throw GeneralException("Unknown link type");
		}
	}
}
