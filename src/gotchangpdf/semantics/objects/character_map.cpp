#include "precompiled.h"
#include "character_map.h"

namespace gotchangpdf
{
	namespace semantics
	{
		CharacterMapBase::CharacterMapBase(syntax::StreamObjectPtr root) : HighLevelObject(root) {}
		EmbeddedCharacterMap::EmbeddedCharacterMap(syntax::StreamObjectPtr root) : CharacterMapBase(root) {}
		UnicodeCharacterMap::UnicodeCharacterMap(syntax::StreamObjectPtr root) : CharacterMapBase(root) {}

		CharacterMapBase::Type EmbeddedCharacterMap::GetType() const noexcept { return Type::Embedded; }
		CharacterMapBase::Type UnicodeCharacterMap::GetType() const noexcept { return Type::Unicode; }

		CharacterMapBase* CharacterMapBase::Create(syntax::StreamObjectPtr root, WeakReference<Document> doc)
		{
			// TODO
			auto result = std::make_unique<UnicodeCharacterMap>(root);
			result->SetDocument(doc);
			return result.release();
		}
	}
}
