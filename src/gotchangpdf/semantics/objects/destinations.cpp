#include "precompiled.h"
#include "destinations.h"

#include "exception.h"
#include "semantic_exceptions.h"
#include "semantic_utils.h"

namespace gotchangpdf
{
	namespace semantics
	{
		DestinationBase::~DestinationBase() {}
		DestinationBase::DestinationBase(syntax::MixedArrayObjectPtr root) : HighLevelObject(root) {}
		XYZDestination::XYZDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitDestination::FitDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitHorizontalDestination::FitHorizontalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitVerticalDestination::FitVerticalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitRectangleDestination::FitRectangleDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitBoundingBoxDestination::FitBoundingBoxDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitBoundingBoxHorizontalDestination::FitBoundingBoxHorizontalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		FitBoundingBoxVerticalDestination::FitBoundingBoxVerticalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
		NamedDestinations::NamedDestinations(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

		DestinationBase* DestinationBase::Create(syntax::MixedArrayObjectPtr root)
		{
			if (root->Size() < 2) {
				throw GeneralException("Invalid destination array");
			}

			syntax::ObjectPtr page_number_obj = root->At(0);
			syntax::ObjectPtr type_obj = root->At(1);

			if (!syntax::ObjectUtils::IsType<syntax::IntegerObjectPtr>(page_number_obj)) {
				throw GeneralException("Invalid page number in destination");
			}

			if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
				throw GeneralException("Invalid destination type");
			}

			syntax::IntegerObjectPtr page_number = syntax::ObjectUtils::ConvertTo<syntax::IntegerObjectPtr>(page_number_obj);
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

		syntax::IntegerObjectPtr DestinationBase::GetPageNumber() const
		{
			return syntax::ObjectUtils::ConvertTo<syntax::IntegerObjectPtr>(_obj->At(0));
		}

		bool NamedDestinations::Contains(const syntax::NameObject& name) const
		{
			return _obj->Contains(name);
		}

		DestinationPtr NamedDestinations::Find(const syntax::NameObject& name) const
		{
			syntax::MixedArrayObjectPtr found = _obj->FindAs<syntax::MixedArrayObjectPtr>(name);
			return DestinationBase::Create(found);
		}

		void NamedDestinations::Insert(const syntax::NameObject& name, DestinationPtr value)
		{
			if (_obj->Contains(name)) {
				bool removed = _obj->Remove(name);
				assert(removed && "Unable to remove existing item"); removed;
			}

			syntax::IndirectObjectReferencePtr reference(value->GetObject());
			_obj->Insert(name, reference);
		}
	}
}
