#ifndef _DEVELOPER_EXTENSIONS_H
#define _DEVELOPER_EXTENSIONS_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "version.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class DeveloperExtensions : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit DeveloperExtensions(syntax::DictionaryObjectPtr root);
			Version BaseVersion(void) const;
			syntax::IntegerObjectPtr ExtensionLevel(void) const;
		};
	}
}

#endif /* _DEVELOPER_EXTENSIONS_H */
