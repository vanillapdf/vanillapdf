#ifndef _CATALOG_H
#define _CATALOG_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/developer_extensions.h"
#include "semantics/objects/page_tree.h"
#include "semantics/objects/page_labels.h"
#include "semantics/objects/viewer_preferences.h"
#include "semantics/objects/outline.h"
#include "semantics/objects/destinations.h"
#include "semantics/objects/actions.h"
#include "semantics/objects/interactive_forms.h"

#include "utils/pdf_version.h"
#include "utils/cached_value.h"

#include <mutex>

namespace vanillapdf {
namespace semantics {

class Catalog : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    enum class PageLayoutType {
        Undefined = 0,
        SinglePage,
        OneColumn,
        TwoColumnLeft,
        TwoColumnRight,
        TwoPageLeft,
        TwoPageRight
    };

    enum class PageModeType {
        Undefined = 0,
        UseNone,
        UseOutlines,
        UseThumbs,
        FullScreen,
        UseOC,
        UseAttachments
    };

public:
    explicit Catalog(syntax::DictionaryObjectPtr root);

    bool Pages(OutputPageTreePtr& result) const;
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
    void SetNames(NameDictionaryPtr value);
    // The interactive form over the /AcroForm entry. One instance per
    // catalog, so that the field tree the form owns is shared by everyone
    // reaching the form through the catalog - Document::Sign included; the
    // instance installed by SetAcroForm is the one handed out.
    bool AcroForm(OuputInteractiveFormPtr& result) const;
    void SetAcroForm(InteractiveFormPtr value);

    bool GetOpenAction(syntax::ObjectPtr& result) const;

    PageTreePtr CreatePages();

private:
    bool ResolvePages(OutputPageTreePtr& result) const;

    CachedValue<OutputPageTreePtr> m_pages;

    // The form instance is resolved once from /AcroForm and then handed out
    // unchanged, or installed by SetAcroForm. The setter can replace it, so
    // the lock-free fast path of CachedValue does not apply - the same
    // arrangement InteractiveForm keeps for its field tree.
    mutable std::unique_ptr<std::recursive_mutex> m_access_lock;
    mutable OuputInteractiveFormPtr m_acro_form;
};

} // semantics
} // vanillapdf

#endif /* _CATALOG_H */
