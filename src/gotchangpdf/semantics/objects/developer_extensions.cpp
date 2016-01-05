#include "precompiled.h"
#include "developer_extensions.h"

#include "exception.h"
#include "semantic_exceptions.h"
#include "semantic_utils.h"

namespace gotchangpdf
{
	namespace semantics
	{
		DeveloperExtensions::DeveloperExtensions(syntax::DictionaryObjectPtr root) : HighLevelObject(root)
		{
			if (root->Contains(constant::Name::Type) && *root->FindAs<syntax::NameObjectPtr>(constant::Name::Type) != constant::Name::DeveloperExtensions)
				throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, DeveloperExtensions>(root);
		}

		Version DeveloperExtensions::BaseVersion(void) const
		{
			auto ver = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::BaseVersion);
			return SemanticUtils::GetVersionFromName(ver);
		}

		syntax::IntegerObjectPtr DeveloperExtensions::ExtensionLevel(void) const
		{
			return _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::ExtensionLevel);
		}
	}
}
