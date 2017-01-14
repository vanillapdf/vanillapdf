#include "precompiled.h"
#include "semantics/objects/developer_extensions.h"

#include "semantics/utils/semantic_exceptions.h"
#include "semantics/utils/semantic_utils.h"

namespace gotchangpdf {
namespace semantics {

DeveloperExtensionPtr DeveloperExtensions::Iterator::Second() const {
	auto containable = _it->second;
	if (!syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(containable))
		throw GeneralException("Developer extension value is not dictionary");

	auto converted = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(containable);
	return DeveloperExtensionPtr(converted);
}

DeveloperExtensions::DeveloperExtensions(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

bool DeveloperExtensions::TryFind(const syntax::NameObjectPtr& name, OutputDeveloperExtensionPtr& result) const {
	syntax::OutputContainableObjectPtr containable;
	bool found = _obj->TryFind(name, containable);
	if (!found) return false;
	if (!syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(containable.GetValue())) return false;
	auto converted = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(containable.GetValue());
	result = DeveloperExtensionPtr(converted);
	return true;
}

bool DeveloperExtensions::Contains(const syntax::NameObjectPtr& name) const {
	return _obj->Contains(name);
}

void DeveloperExtensions::Insert(const syntax::NameObjectPtr& name, const DeveloperExtension& value) {
	_obj->Insert(name, value.GetObject());
}

bool DeveloperExtensions::Remove(const syntax::NameObjectPtr& name) {
	return _obj->Remove(name);
}

DeveloperExtensions::IteratorPtr DeveloperExtensions::Begin(void) const {
	return _obj->begin();
}

DeveloperExtensions::IteratorPtr DeveloperExtensions::End(void) const {
	return _obj->end();
}

DeveloperExtension::DeveloperExtension(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {
	if (root->Contains(constant::Name::Type) && root->FindAs<syntax::NameObjectPtr>(constant::Name::Type) != constant::Name::DeveloperExtensions)
		throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, DeveloperExtensions>(root);
}

Version DeveloperExtension::BaseVersion(void) const {
	auto ver = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::BaseVersion);
	return SemanticUtils::GetVersionFromName(ver);
}

syntax::IntegerObjectPtr DeveloperExtension::ExtensionLevel(void) const {
	return _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::ExtensionLevel);
}

} // semantics
} // gotchangpdf
