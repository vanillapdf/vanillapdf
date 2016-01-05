#ifndef _CATALOG_H
#define _CATALOG_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "developer_extensions.h"
#include "page_tree.h"
#include "version.h"
#include "page_labels.h"
#include "viewer_preferences.h"
#include "outline.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class Catalog : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			enum class PageLayoutType
			{
				SinglePage = 0,
				OneColumn,
				TwoColumnLeft,
				TwoColumnRight,
				TwoPageLeft,
				TwoPageRight
			};

			enum class PageModeType
			{
				UseNone = 0,
				UseOutlines,
				UseThumbs,
				FullScreen,
				UseOC,
				UseAttachments
			};

		public:
			explicit Catalog(syntax::DictionaryObjectPtr root);

			// optional entries
			Version Version(void) const;
			DeveloperExtensionsPtr Extensions(void) const;
			PageLabelsPtr PageLabels(void) const;
			PageLayoutType PageLayout(void) const;
			bool ViewerPreferences(ViewerPreferencesPtr& result) const;
			bool PageMode(PageModeType& result) const;
			bool Outlines(OutlinePtr& result) const;

			// required
			PageTreePtr Pages(void) const;
		};

	}
}

#endif /* _CATALOG_H */
