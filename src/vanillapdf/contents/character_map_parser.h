#ifndef _CHARACTER_MAP_PARSER_H
#define _CHARACTER_MAP_PARSER_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/parsers/parser.h"

#include "contents/character_map_data.h"

#include <stack>
#include <vector>
#include <memory>

namespace vanillapdf {
namespace contents {

class CharacterMapParser : public syntax::ParserBase {
public:
    CharacterMapParser(WeakReference<syntax::File> file, IInputStreamPtr stream);
    CharacterMapData ReadCharacterMapData(void);
    
    // Override to handle PostScript dictionary syntax and def inside dictionaries
    virtual syntax::DictionaryObjectPtr ReadDictionary() override;
    
private:
    // PostScript interpreter components
    std::stack<syntax::ObjectPtr> m_operand_stack;
    std::vector<syntax::DictionaryObjectPtr> m_dictionary_stack;
    
    // PostScript stack operators
    void ExecuteDup();
    void ExecuteExch();
    void ExecutePop();
    void ExecuteDict();
    void ExecuteBegin();
    void ExecuteEnd();
    void ExecuteDef();
    
    // Helper methods
    void PushToStack(syntax::ObjectPtr obj);
    syntax::ObjectPtr PopFromStack();
    syntax::ObjectPtr PeekStack();
    bool IsStackEmpty() const;
    size_t StackSize() const;
    
    // Dictionary management
    syntax::DictionaryObjectPtr GetCurrentDictionary();
    
    // PostScript-aware parsing
    syntax::DictionaryObjectPtr ReadPostScriptDictionary();
    syntax::DictionaryObjectPtr ReadPDFDictionaryWithDef();
    void ProcessPostScriptToken(syntax::TokenPtr token);
    syntax::ObjectPtr ExecutePostScriptSequence();
};

} // contents
} // vanillapdf

#endif /* _CHARACTER_MAP_PARSER_H */
