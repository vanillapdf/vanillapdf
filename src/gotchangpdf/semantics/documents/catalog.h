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
#include "destinations.h"

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
			bool Version(Version& result) const;
			bool Extensions(OutputDeveloperExtensionsPtr& result) const;
			bool PageLabels(OutputPageLabelsPtr& result) const;
			bool PageLayout(PageLayoutType& result) const;
			bool ViewerPreferences(OutputViewerPreferencesPtr& result) const;
			bool PageMode(PageModeType& result) const;
			bool Outlines(OutputOutlinePtr& result) const;
			bool NeedsRendering(syntax::BooleanObjectPtr& result) const;
			bool Destinations(OutputNamedDestinationsPtr& result) const;
			bool Names(OutputNameDictionaryPtr& result) const;

			// required
			PageTreePtr Pages(void) const;

		private:
			// Cache
			mutable OutputPageTreePtr m_pages;
		};

	}
}

#endif /* _CATALOG_H */
