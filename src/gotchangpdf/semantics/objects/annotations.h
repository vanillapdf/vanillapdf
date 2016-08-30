#ifndef _ANNOTATIONS_H
#define _ANNOTATIONS_H

#include "semantics_fwd.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class AnnotationBase : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit AnnotationBase(syntax::DictionaryObjectPtr root);

			static AnnotationBase* Create(syntax::DictionaryObjectPtr root, WeakReference<Document> doc);

			virtual AnnotationBase::Type GetType() const noexcept = 0;
			virtual ~AnnotationBase() = 0;
		};

		class LinkAnnotation : public AnnotationBase
		{
		public:
			explicit LinkAnnotation(syntax::DictionaryObjectPtr root);
			virtual AnnotationBase::Type GetType() const noexcept override;

			bool Destination(OutputDestinationPtr& result) const;
		};

		class PageAnnotations : public HighLevelObject<syntax::MixedArrayObjectPtr>
		{
		public:
			PageAnnotations(syntax::MixedArrayObjectPtr root);
			types::uinteger Size() const;
			AnnotationPtr At(types::uinteger index) const;
		};
	}
}

#endif /* _ANNOTATIONS_H */
