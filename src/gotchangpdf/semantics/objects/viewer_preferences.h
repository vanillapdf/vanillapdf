#ifndef _VIEWER_PREFERENCES_H
#define _VIEWER_PREFERENCES_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/array_object.h"

namespace gotchangpdf {
namespace semantics {

class PageRange : public HighLevelObject<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>> {
public:
	class SubRange : public IUnknown {
	public:
		SubRange(syntax::IntegerObjectPtr first, syntax::IntegerObjectPtr last);

		syntax::IntegerObjectPtr FirstPage(void) const;
		syntax::IntegerObjectPtr LastPage(void) const;

	private:
		syntax::IntegerObjectPtr _first_page;
		syntax::IntegerObjectPtr _last_page;
	};

	using SubRangePtr = Deferred<SubRange>;

public:
	explicit PageRange(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> obj);

	types::uinteger Size(void) const;
	SubRangePtr At(types::uinteger at) const;
};

class ViewerPreferences : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	enum class NonFullScreenPageModeType {
		UseNone = 0,
		UseOutlines,
		UseThumbs,
		UseOC
	};

	enum class ReadingOrderType {
		LeftToRight = 0,
		RightToLeft
	};

	enum class PrintScalingType {
		AppDefault = 0,
		None
	};

	enum class DuplexType {
		Simplex = 0,
		DuplexFlipShortEdge,
		DuplexFlipLongEdge
	};

public:
	explicit ViewerPreferences(syntax::DictionaryObjectPtr root);
	bool HideToolbar(syntax::BooleanObjectPtr& result) const;
	bool HideMenubar(syntax::BooleanObjectPtr& result) const;
	bool HideWindowUI(syntax::BooleanObjectPtr& result) const;
	bool FitWindow(syntax::BooleanObjectPtr& result) const;
	bool CenterWindow(syntax::BooleanObjectPtr& result) const;
	bool DisplayDocTitle(syntax::BooleanObjectPtr& result) const;
	bool NonFullScreenPageMode(NonFullScreenPageModeType& result) const;
	bool Direction(ReadingOrderType& result) const;
	bool ViewArea(syntax::NameObjectPtr& result) const;
	bool ViewClip(syntax::NameObjectPtr& result) const;
	bool PrintArea(syntax::NameObjectPtr& result) const;
	bool PrintClip(syntax::NameObjectPtr& result) const;
	bool PrintScaling(PrintScalingType& result) const;
	bool Duplex(DuplexType& result) const;
	bool PickTrayByPDFSize(syntax::BooleanObjectPtr& result) const;
	bool PrintPageRange(OutputPageRangePtr& result) const;
	bool NumCopies(syntax::IntegerObjectPtr& result) const;
};

} // semantics
} // gotchangpdf

#endif /* _VIEWER_PREFERENCES_H */
