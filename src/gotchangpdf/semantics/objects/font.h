#ifndef _FONT_H
#define _FONT_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "dictionary_object.h"
#include "character_map.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class FontMap : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit FontMap(syntax::DictionaryObjectPtr obj);
			bool Contains(syntax::NameObjectPtr name) const;
			FontPtr Find(syntax::NameObjectPtr name) const;
		};

		class FontBase : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			enum Type
			{
				Composite
			};

		public:
			explicit FontBase(syntax::DictionaryObjectPtr root);
			static FontBase* Create(syntax::DictionaryObjectPtr root, WeakReference<Document> doc);

			virtual Type GetType() const noexcept = 0;
		};

		class CompositeFont : public FontBase
		{
		public:
			explicit CompositeFont(syntax::DictionaryObjectPtr root);

			virtual Type GetType() const noexcept override;

			//syntax::NameObjectPtr BaseFont() const;
			//syntax::ObjectPtr Encoding() const;
			//syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr> DescendantFonts() const;
			bool ToUnicode(OuputUnicodeCharacterMapPtr& result) const;
		};
	}
}

#endif /* _FONT_H */
