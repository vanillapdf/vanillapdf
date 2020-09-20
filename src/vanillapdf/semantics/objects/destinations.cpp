#include "precompiled.h"

#include "semantics/objects/destinations.h"
#include "semantics/utils/semantic_exceptions.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

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

DestinationBase::Type XYZDestination::GetType() const noexcept { return DestinationBase::Type::XYZ; }
DestinationBase::Type FitDestination::GetType() const noexcept { return DestinationBase::Type::Fit; }
DestinationBase::Type FitHorizontalDestination::GetType() const noexcept { return DestinationBase::Type::FitHorizontal; }
DestinationBase::Type FitVerticalDestination::GetType() const noexcept { return DestinationBase::Type::FitVertical; }
DestinationBase::Type FitRectangleDestination::GetType() const noexcept { return DestinationBase::Type::FitRectangle; }
DestinationBase::Type FitBoundingBoxDestination::GetType() const noexcept { return DestinationBase::Type::FitBoundingBox; }
DestinationBase::Type FitBoundingBoxHorizontalDestination::GetType() const noexcept { return DestinationBase::Type::FitBoundingBoxHorizontal; }
DestinationBase::Type FitBoundingBoxVerticalDestination::GetType() const noexcept { return DestinationBase::Type::FitBoundingBoxVertical; }

NamedDestinations::NamedDestinations(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

std::unique_ptr<DestinationBase> DestinationBase::Create(syntax::ObjectPtr root) {
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

std::unique_ptr<DestinationBase> DestinationBase::Create(syntax::MixedArrayObjectPtr root) {
	if (root->GetSize() < 2) {
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
		return make_unique<XYZDestination>(root);
	}

	if (type == constant::Name::Fit) {
		return make_unique<FitDestination>(root);
	}

	if (type == constant::Name::FitH) {
		return make_unique<FitHorizontalDestination>(root);
	}

	if (type == constant::Name::FitV) {
		return make_unique<FitVerticalDestination>(root);
	}

	if (type == constant::Name::FitR) {
		return make_unique<FitRectangleDestination>(root);
	}

	if (type == constant::Name::FitB) {
		return make_unique<FitBoundingBoxDestination>(root);
	}

	if (type == constant::Name::FitBH) {
		return make_unique<FitBoundingBoxHorizontalDestination>(root);
	}

	if (type == constant::Name::FitBV) {
		return make_unique<FitBoundingBoxVerticalDestination>(root);
	}

	throw GeneralException("Unknown destination type");
}

std::unique_ptr<DestinationBase> DestinationBase::Create(syntax::DictionaryObjectPtr root) {
	if (!root->Contains(constant::Name::D)) {
		throw GeneralException("Invalid destination dictionary");
	}

	auto destination = root->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::D);

	if (destination->GetSize() < 2) {
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
		return make_unique<XYZDestination>(root);
	}

	if (type == constant::Name::Fit) {
		return make_unique<FitDestination>(root);
	}

	if (type == constant::Name::FitH) {
		return make_unique<FitHorizontalDestination>(root);
	}

	if (type == constant::Name::FitV) {
		return make_unique<FitVerticalDestination>(root);
	}

	if (type == constant::Name::FitR) {
		return make_unique<FitRectangleDestination>(root);
	}

	if (type == constant::Name::FitB) {
		return make_unique<FitBoundingBoxDestination>(root);
	}

	if (type == constant::Name::FitBH) {
		return make_unique<FitBoundingBoxHorizontalDestination>(root);
	}

	if (type == constant::Name::FitBV) {
		return make_unique<FitBoundingBoxVerticalDestination>(root);
	}

	throw GeneralException("Unknown destination type");
}

bool DestinationBase::HasAttribute(const syntax::NameObject& name) const {
	if (!syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(_obj)) {
		return false;
	}

	auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(_obj);
	return dict->Contains(name);
}

syntax::ObjectPtr DestinationBase::GetAttribute(const syntax::NameObject& name) const {
	if (!syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(_obj)) {
		return syntax::NullObject::GetInstance();
	}

	auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(_obj);
	return dict->Find(name);
}

syntax::ObjectPtr DestinationBase::GetPage() const {
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

bool NamedDestinations::Contains(const syntax::NameObject& name) const {
	return _obj->Contains(name);
}

DestinationPtr NamedDestinations::Find(const syntax::NameObject& name) const {
	auto found_obj = _obj->Find(name);
	if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(found_obj)) {
		auto found_array = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(found_obj);
		auto destination = DestinationBase::Create(found_array);
		auto raw_ptr = destination.release();
		return DestinationPtr(raw_ptr);
	}

	if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(found_obj)) {
		auto found_dictionary = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(found_obj);
		auto destination = DestinationBase::Create(found_dictionary);
		auto raw_ptr = destination.release();
		return DestinationPtr(raw_ptr);
	}

	throw GeneralException("Unable to find entry");
}

void NamedDestinations::Insert(const syntax::NameObject& name, DestinationPtr value) {
	if (_obj->Contains(name)) {
		bool removed = _obj->Remove(name);
		assert(removed && "Unable to remove existing item"); UNUSED(removed);
	}

	auto raw_obj = value->GetObject();
	if (raw_obj->IsIndirect()) {
		syntax::IndirectObjectReferencePtr reference = make_deferred<syntax::IndirectObjectReference>(raw_obj);
		_obj->Insert(name, reference);
	} else {
		auto containable = syntax::ObjectUtils::ConvertTo<syntax::ContainableObjectPtr>(raw_obj);
		_obj->Insert(name, containable);
	}
}

} // semantics
} // vanillapdf
