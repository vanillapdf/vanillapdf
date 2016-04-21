#ifndef _VIEWER_PREFERENCES_H
#define _VIEWER_PREFERENCES_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "dictionary_object.h"
#include "array_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class PageRange : public HighLevelObject<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>>
		{
		public:
			class SubRange : public IUnknown
			{
			public:
				SubRange(syntax::IntegerObjectPtr first, syntax::IntegerObjectPtr last) :
					_first_page(first), _last_page(last) {}

				syntax::IntegerObjectPtr FirstPage(void) const { return _first_page; }
				syntax::IntegerObjectPtr LastPage(void) const { return _last_page; }

			private:
				syntax::IntegerObjectPtr _first_page;
				syntax::IntegerObjectPtr _last_page;
			};

			using SubRangePtr = Deferred<SubRange>;

		public:
			explicit PageRange(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> obj)
				: HighLevelObject(obj) { assert(obj->Size() % 2 == 0); }

			types::uinteger Size(void) const { return _obj->Size() / 2; }
			SubRangePtr At(types::uinteger at) const
			{
				return SubRangePtr(_obj->At(at), _obj->At(at + 1));
			}
		};

		class ViewerPreferences : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			enum class NonFullScreenPageModeType
			{
				UseNone = 0,
				UseOutlines,
				UseThumbs,
				UseOC
			};

			enum class ReadingOrderType
			{
				LeftToRight = 0,
				RightToLeft
			};

			enum class PrintScalingType
			{
				AppDefault = 0,
				None
			};

			enum class DuplexType
			{
				Simplex = 0,
				DuplexFlipShortEdge,
				DuplexFlipLongEdge
			};

		public:
			explicit ViewerPreferences(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

			bool HideToolbar(syntax::BooleanObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::HideToolbar))
					return false;

				result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::HideToolbar);
				return true;
			}

			bool HideMenubar(syntax::BooleanObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::HideMenubar))
					return false;

				result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::HideMenubar);
				return true;
			}

			bool HideWindowUI(syntax::BooleanObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::HideWindowUI))
					return false;

				result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::HideWindowUI);
				return true;
			}

			bool FitWindow(syntax::BooleanObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::FitWindow))
					return false;

				result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::FitWindow);
				return true;
			}

			bool CenterWindow(syntax::BooleanObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::CenterWindow))
					return false;

				result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::CenterWindow);
				return true;
			}

			bool DisplayDocTitle(syntax::BooleanObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::DisplayDocTitle))
					return false;

				result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::DisplayDocTitle);
				return true;
			}

			bool NonFullScreenPageMode(NonFullScreenPageModeType& result) const
			{
				if (!_obj->Contains(constant::Name::NonFullScreenPageMode))
					return false;

				auto name = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::NonFullScreenPageMode);
				if (name == constant::Name::UseNone)
					result = NonFullScreenPageModeType::UseNone;
				else if (name == constant::Name::UseOutlines)
					result = NonFullScreenPageModeType::UseOutlines;
				else if (name == constant::Name::UseThumbs)
					result = NonFullScreenPageModeType::UseThumbs;
				else if (name == constant::Name::UseOC)
					result = NonFullScreenPageModeType::UseOC;
				else
					throw GeneralException("Unknown page mode type: " + name->ToString());

				return true;
			}

			bool Direction(ReadingOrderType& result) const
			{
				if (!_obj->Contains(constant::Name::Direction))
					return false;

				auto name = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::Direction);
				if (name == constant::Name::L2R)
					result = ReadingOrderType::LeftToRight;
				else if (name == constant::Name::R2L)
					result = ReadingOrderType::RightToLeft;
				else
					throw GeneralException("Unknown reading order: " + name->ToString());

				return true;
			}

			bool ViewArea(syntax::NameObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::ViewArea))
					return false;

				result = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::ViewArea);
				return true;
			}

			bool ViewClip(syntax::NameObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::ViewClip))
					return false;

				result = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::ViewClip);
				return true;
			}

			bool PrintArea(syntax::NameObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::PrintArea))
					return false;

				result = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::PrintArea);
				return true;
			}

			bool PrintClip(syntax::NameObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::PrintClip))
					return false;

				result = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::PrintClip);
				return true;
			}

			bool PrintScaling(PrintScalingType& result) const
			{
				if (!_obj->Contains(constant::Name::PrintScaling))
					return false;

				auto name = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::PrintScaling);
				if (name == constant::Name::AppDefault)
					result = PrintScalingType::AppDefault;
				else if (name == constant::Name::None)
					result = PrintScalingType::None;
				else
					throw GeneralException("Unknown print scaling: " + name->ToString());

				return true;
			}

			bool Duplex(DuplexType& result) const
			{
				if (!_obj->Contains(constant::Name::Duplex))
					return false;

				auto name = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::Duplex);
				if (name == constant::Name::UseNone)
					result = DuplexType::Simplex;
				else if (name == constant::Name::DuplexFlipShortEdge)
					result = DuplexType::DuplexFlipShortEdge;
				else if (name == constant::Name::DuplexFlipLongEdge)
					result = DuplexType::DuplexFlipLongEdge;
				else
					throw GeneralException("Unknown duplex: " + name->ToString());

				return true;
			}

			bool PickTrayByPDFSize(syntax::BooleanObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::PickTrayByPDFSize))
					return false;

				result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::PickTrayByPDFSize);
				return true;
			}

			bool PrintPageRange(OutputPageRangePtr& result) const
			{
				if (!_obj->Contains(constant::Name::PrintPageRange))
					return false;

				auto range = _obj->FindAs<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>>(constant::Name::PrintPageRange);
				result = PageRangePtr(range);
				return true;
			}

			bool NumCopies(syntax::IntegerObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::NumCopies))
					return false;

				result = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::NumCopies);
				return true;
			}
		};
	}
}

#endif /* _VIEWER_PREFERENCES_H */
