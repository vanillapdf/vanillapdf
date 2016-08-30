#include "precompiled.h"
#include "destinations.h"

#include "exception.h"
#include "semantic_exceptions.h"
#include "semantic_utils.h"

namespace gotchangpdf
{
	namespace semantics
	{
		DestinationBase::DestinationBase(syntax::MixedArrayObjectPtr root) : HighLevelObject(root) {}
		XYZDestination::XYZDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitDestination::FitDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitHorizontalDestination::FitHorizontalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitVerticalDestination::FitVerticalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitRectangleDestination::FitRectangleDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitBoundingBoxDestination::FitBoundingBoxDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitBoundingBoxHorizontalDestination::FitBoundingBoxHorizontalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitBoundingBoxVerticalDestination::FitBoundingBoxVerticalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}

		DestinationBase::DestinationBase(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
		XYZDestination::XYZDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
		FitDestination::FitDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
		FitHorizontalDestination::FitHorizontalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
		FitVerticalDestination::FitVerticalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
		FitRectangleDestination::FitRectangleDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
		FitBoundingBoxDestination::FitBoundingBoxDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
		FitBoundingBoxHorizontalDestination::FitBoundingBoxHorizontalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
		FitBoundingBoxVerticalDestination::FitBoundingBoxVerticalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}

		NamedDestinations::NamedDestinations(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
		DestinationBase::~DestinationBase() {}

		DestinationBase* DestinationBase::Create(syntax::ObjectPtr root)
		{
			if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(root)) {
				auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(root);
				return Create(arr);
			}

			if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(root)) {
				auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(root);
				return Create(dict);
			}

			throw GeneralException("Invalid object type");
		}

		DestinationBase* DestinationBase::Create(syntax::MixedArrayObjectPtr root)
		{
			if (root->Size() < 2) {
				throw GeneralException("Invalid destination array");
			}

			syntax::ObjectPtr page_number_obj = root->At(0);
			syntax::ObjectPtr type_obj = root->At(1);

			// page_number_obj shall be indirect reference to page object
			// for remote go to actions it can be integer value of destination page
			if (!syntax::ObjectUtils::IsType<syntax::IntegerObjectPtr>(page_number_obj)
				&& !syntax::ObjectUtils::IsType<syntax::IndirectObjectReferencePtr>(page_number_obj)) {
				throw GeneralException("Invalid page number in destination");
			}

			if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
				throw GeneralException("Invalid destination type");
			}

			syntax::NameObjectPtr type = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);

			if (type == constant::Name::XYZ) {
				return new XYZDestination(root);
			}

			if (type == constant::Name::Fit) {
				return new FitDestination(root);
			}

			if (type == constant::Name::FitH) {
				return new FitHorizontalDestination(root);
			}

			if (type == constant::Name::FitV) {
				return new FitVerticalDestination(root);
			}

			if (type == constant::Name::FitR) {
				return new FitRectangleDestination(root);
			}

			if (type == constant::Name::FitB) {
				return new FitBoundingBoxDestination(root);
			}

			if (type == constant::Name::FitBH) {
				return new FitBoundingBoxHorizontalDestination(root);
			}

			if (type == constant::Name::FitBV) {
				return new FitBoundingBoxVerticalDestination(root);
			}

			throw GeneralException("Unknown destination type");
		}

		DestinationBase* DestinationBase::Create(syntax::DictionaryObjectPtr root)
		{
			if (!root->Contains(constant::Name::D)) {
				throw GeneralException("Invalid destination dictionary");
			}

			auto destination = root->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::D);

			if (destination->Size() < 2) {
				throw GeneralException("Invalid destination array");
			}

			syntax::ObjectPtr page_number_obj = destination->At(0);
			syntax::ObjectPtr type_obj = destination->At(1);

			// page_number_obj shall be indirect reference to page object
			// for remote go to actions it can be integer value of destination page
			if (!syntax::ObjectUtils::IsType<syntax::IntegerObjectPtr>(page_number_obj)
				&& !syntax::ObjectUtils::IsType<syntax::IndirectObjectReferencePtr>(page_number_obj)) {
				throw GeneralException("Invalid page number in destination");
			}

			if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
				throw GeneralException("Invalid destination type");
			}

			syntax::NameObjectPtr type = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);

			if (type == constant::Name::XYZ) {
				return new XYZDestination(root);
			}

			if (type == constant::Name::Fit) {
				return new FitDestination(root);
			}

			if (type == constant::Name::FitH) {
				return new FitHorizontalDestination(root);
			}

			if (type == constant::Name::FitV) {
				return new FitVerticalDestination(root);
			}

			if (type == constant::Name::FitR) {
				return new FitRectangleDestination(root);
			}

			if (type == constant::Name::FitB) {
				return new FitBoundingBoxDestination(root);
			}

			if (type == constant::Name::FitBH) {
				return new FitBoundingBoxHorizontalDestination(root);
			}

			if (type == constant::Name::FitBV) {
				return new FitBoundingBoxVerticalDestination(root);
			}

			throw GeneralException("Unknown destination type");
		}

		bool DestinationBase::HasAttribute(const syntax::NameObject& name) const
		{
			if (!syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(_obj)) {
				return false;
			}

			auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(_obj);
			return dict->Contains(name);
		}

		syntax::ObjectPtr DestinationBase::GetAttribute(const syntax::NameObject& name) const
		{
			if (!syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(_obj)) {
				return false;
			}

			auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(_obj);
			return dict->Find(name);
		}

		syntax::ObjectPtr DestinationBase::GetPage() const
		{
			if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(_obj)) {
				auto destination = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
				return destination->At(0);
			}

			if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(_obj)) {
				auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(_obj);
				auto destination = dict->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::D);
				return destination->At(0);
			}

			assert(false && "Destination was created but object is neither array nor dictionary");
			throw GeneralException("Destination was created but object is neither array nor dictionary");
		}

		bool NamedDestinations::Contains(const syntax::NameObject& name) const
		{
			return _obj->Contains(name);
		}

		DestinationPtr NamedDestinations::Find(const syntax::NameObject& name) const
		{
			auto found_obj = _obj->Find(name);
			if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(found_obj)) {
				syntax::MixedArrayObjectPtr found_array = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(found_obj);
				return DestinationBase::Create(found_array);
			}

			if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(found_obj)) {
				syntax::DictionaryObjectPtr found_dictionary = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(found_obj);
				return DestinationBase::Create(found_dictionary);
			}

			throw GeneralException("Unable to find entry");
		}

		void NamedDestinations::Insert(const syntax::NameObject& name, DestinationPtr value)
		{
			if (_obj->Contains(name)) {
				bool removed = _obj->Remove(name);
				assert(removed && "Unable to remove existing item"); removed;
			}

			auto raw_obj = value->GetObject();
			if (raw_obj->IsIndirect()) {
				syntax::IndirectObjectReferencePtr reference(raw_obj);
				_obj->Insert(name, reference);
			}
			else {
				auto containable = syntax::ObjectUtils::ConvertTo<syntax::ContainableObjectPtr>(raw_obj);
				_obj->Insert(name, containable);
			}
		}
	}
}
