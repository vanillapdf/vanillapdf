#ifndef _CATALOG_H
#define _CATALOG_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "developer_extensions.h"
#include "page_tree.h"
#include "version.h"
#include "page_labels.h"

namespace gotchangpdf
{
	namespace semantics
	{
		enum class PageLayout
		{
			SinglePage = 0,
			OneColumn,
			TwoColumnLeft,
			TwoColumnRight,
			TwoPageLeft,
			TwoPageRight
		};

		class Catalog : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit Catalog(syntax::DictionaryObjectPtr root);

			// optional entries
			Version Version(void) const;
			DeveloperExtensionsPtr Extensions(void) const;
			PageLabelsPtr PageLabels(void) const;
			PageLayout PageLayout(void) const;

			// required
			PageTreePtr Pages(void) const;
		};

	}
}

#endif /* _CATALOG_H */
