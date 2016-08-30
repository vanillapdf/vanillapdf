#ifndef _DESTINATIONS_H
#define _DESTINATIONS_H

#include "semantics_fwd.h"
#include "high_level_object.h"

#include "dictionary_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class DestinationBase : public HighLevelObject<syntax::ObjectPtr>
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
			explicit DestinationBase(syntax::DictionaryObjectPtr root);

			static DestinationBase* Create(syntax::MixedArrayObjectPtr root, WeakReference<Document> doc);
			static DestinationBase* Create(syntax::DictionaryObjectPtr root, WeakReference<Document> doc);
			static DestinationBase* Create(syntax::ObjectPtr root, WeakReference<Document> doc);

			syntax::ObjectPtr GetPage() const;
			bool HasAttribute(const syntax::NameObject& name) const;
			syntax::ObjectPtr GetAttribute(const syntax::NameObject& name) const;

			virtual ~DestinationBase() = 0;
		};

		class XYZDestination : public DestinationBase
		{
		public:
			explicit XYZDestination(syntax::MixedArrayObjectPtr root);
			explicit XYZDestination(syntax::DictionaryObjectPtr root);
		};

		class FitDestination : public DestinationBase
		{
		public:
			explicit FitDestination(syntax::MixedArrayObjectPtr root);
			explicit FitDestination(syntax::DictionaryObjectPtr root);
		};

		class FitHorizontalDestination : public DestinationBase
		{
		public:
			explicit FitHorizontalDestination(syntax::MixedArrayObjectPtr root);
			explicit FitHorizontalDestination(syntax::DictionaryObjectPtr root);
		};

		class FitVerticalDestination : public DestinationBase
		{
		public:
			explicit FitVerticalDestination(syntax::MixedArrayObjectPtr root);
			explicit FitVerticalDestination(syntax::DictionaryObjectPtr root);
		};

		class FitRectangleDestination : public DestinationBase
		{
		public:
			explicit FitRectangleDestination(syntax::MixedArrayObjectPtr root);
			explicit FitRectangleDestination(syntax::DictionaryObjectPtr root);
		};

		class FitBoundingBoxDestination : public DestinationBase
		{
		public:
			explicit FitBoundingBoxDestination(syntax::MixedArrayObjectPtr root);
			explicit FitBoundingBoxDestination(syntax::DictionaryObjectPtr root);
		};

		class FitBoundingBoxHorizontalDestination : public DestinationBase
		{
		public:
			explicit FitBoundingBoxHorizontalDestination(syntax::MixedArrayObjectPtr root);
			explicit FitBoundingBoxHorizontalDestination(syntax::DictionaryObjectPtr root);
		};

		class FitBoundingBoxVerticalDestination : public DestinationBase
		{
		public:
			explicit FitBoundingBoxVerticalDestination(syntax::MixedArrayObjectPtr root);
			explicit FitBoundingBoxVerticalDestination(syntax::DictionaryObjectPtr root);
		};

		class NamedDestinations : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit NamedDestinations(syntax::DictionaryObjectPtr root);

			void Insert(const syntax::NameObject& name, DestinationPtr value);
			bool Contains(const syntax::NameObject& name) const;
			DestinationPtr Find(const syntax::NameObject& name) const;
		};
	}
}

#endif /* _NAME_DICTIONARY_H */
