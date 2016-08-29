#ifndef _DESTINATIONS_H
#define _DESTINATIONS_H

#include "semantics_fwd.h"
#include "high_level_object.h"

#include "dictionary_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class DestinationBase : public HighLevelObject<syntax::MixedArrayObjectPtr>
		{
		public:
			enum Type
			{
				XYZ = 0,
				Fit,
				FitHorizontal,
				FitVertical,
				FitRectangle,
				FitBoundingBox,
				FitBoundingBoxHorizontal,
				FitBoundingBoxVertical
			};

		public:
			explicit DestinationBase(syntax::MixedArrayObjectPtr root);
			static DestinationBase* Create(syntax::MixedArrayObjectPtr root);

			syntax::IntegerObjectPtr GetPageNumber() const;

			virtual ~DestinationBase() = 0;
		};

		class XYZDestination : public DestinationBase
		{
		public:
			explicit XYZDestination(syntax::MixedArrayObjectPtr root);
		};

		class FitDestination : public DestinationBase
		{
		public:
			explicit FitDestination(syntax::MixedArrayObjectPtr root);
		};

		class FitHorizontalDestination : public DestinationBase
		{
		public:
			explicit FitHorizontalDestination(syntax::MixedArrayObjectPtr root);
		};

		class FitVerticalDestination : public DestinationBase
		{
		public:
			explicit FitVerticalDestination(syntax::MixedArrayObjectPtr root);
		};

		class FitRectangleDestination : public DestinationBase
		{
		public:
			explicit FitRectangleDestination(syntax::MixedArrayObjectPtr root);
		};

		class FitBoundingBoxDestination : public DestinationBase
		{
		public:
			explicit FitBoundingBoxDestination(syntax::MixedArrayObjectPtr root);
		};

		class FitBoundingBoxHorizontalDestination : public DestinationBase
		{
		public:
			explicit FitBoundingBoxHorizontalDestination(syntax::MixedArrayObjectPtr root);
		};

		class FitBoundingBoxVerticalDestination : public DestinationBase
		{
		public:
			explicit FitBoundingBoxVerticalDestination(syntax::MixedArrayObjectPtr root);
		};

		class NamedDestinations : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit NamedDestinations(syntax::DictionaryObjectPtr root);

			bool Contains(syntax::NameObjectPtr& name) const;
			DestinationPtr Find(syntax::NameObjectPtr& name) const;
		};
	}
}

#endif /* _NAME_DICTIONARY_H */
