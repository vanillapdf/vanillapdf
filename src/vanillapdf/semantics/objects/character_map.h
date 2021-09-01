#ifndef _CHARACTER_MAP_H
#define _CHARACTER_MAP_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/parsers/character_map_data.h"

namespace vanillapdf {
namespace semantics {

class CharacterMapBase : public HighLevelObject<syntax::StreamObjectPtr> {
public:
	enum class Type {
		Undefined = 0,
		Embedded,
		Unicode
	};

public:
	explicit CharacterMapBase(syntax::StreamObjectPtr root);
	virtual CharacterMapBase::Type GetCharacterMapType() const noexcept = 0;

	static std::unique_ptr<CharacterMapBase> Create(syntax::StreamObjectPtr root, WeakReference<Document> doc);
};

class EmbeddedCharacterMap : public CharacterMapBase {
public:
	explicit EmbeddedCharacterMap(syntax::StreamObjectPtr root);
	virtual CharacterMapBase::Type GetCharacterMapType() const noexcept override {
		return Type::Embedded;
	}
};

class UnicodeCharacterMap : public CharacterMapBase {
public:
	explicit UnicodeCharacterMap(syntax::StreamObjectPtr root);
	virtual CharacterMapBase::Type GetCharacterMapType() const noexcept override {
		return Type::Unicode;
	}

	BufferPtr GetMappedValue(BufferPtr key) const;

private:
	mutable syntax::CharacterMapData m_data;
	mutable bool m_initialized = false;

	void Initialize() const;
};

} // semantics
} // vanillapdf

#endif /* _CHARACTER_MAP_H */
