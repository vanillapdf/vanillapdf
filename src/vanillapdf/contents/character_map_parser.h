#ifndef _CHARACTER_MAP_PARSER_H
#define _CHARACTER_MAP_PARSER_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/parsers/parser.h"

#include "contents/character_map_data.h"

namespace vanillapdf {
namespace contents {

class CharacterMapParser : public syntax::ParserBase {
public:
	CharacterMapParser(WeakReference<syntax::File> file, IInputStreamPtr stream);
	CharacterMapData ReadCharacterMapData(void);
};

} // contents
} // vanillapdf

#endif /* _CHARACTER_MAP_PARSER_H */
