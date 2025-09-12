#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/parsers/parser_utils.h"
#include "syntax/utils/name_constants.h"
#include "syntax/utils/convert_utils.h"

#include "contents/character_map_parser.h"

#include <spdlog/spdlog.h>

namespace vanillapdf {
namespace contents {

using namespace vanillapdf::syntax;

CharacterMapParser::CharacterMapParser(WeakReference<File> file, IInputStreamPtr stream)
    : ParserBase(file, stream) {
    _dictionary = make_unique<CharacterMapTokenDictionary>();
    _dictionary->Initialize();
}

// Stack manipulation helpers
void CharacterMapParser::PushToStack(ObjectPtr obj) {
    m_operand_stack.push(obj);
}

ObjectPtr CharacterMapParser::PopFromStack() {
    if (m_operand_stack.empty()) {
        spdlog::warn("Attempt to pop from empty operand stack");
        return NullObject::GetInstance();
    }
    auto obj = m_operand_stack.top();
    m_operand_stack.pop();
    return obj;
}

ObjectPtr CharacterMapParser::PeekStack() {
    if (m_operand_stack.empty()) {
        return NullObject::GetInstance();
    }
    return m_operand_stack.top();
}

bool CharacterMapParser::IsStackEmpty() const {
    return m_operand_stack.empty();
}

size_t CharacterMapParser::StackSize() const {
    return m_operand_stack.size();
}

DictionaryObjectPtr CharacterMapParser::GetCurrentDictionary() {
    if (m_dictionary_stack.empty()) {
        return nullptr;
    }
    return m_dictionary_stack.back();
}

// PostScript stack operators implementation
void CharacterMapParser::ExecuteDup() {
    // dup: duplicate the top element on the stack
    if (!m_operand_stack.empty()) {
        auto top = m_operand_stack.top();
        m_operand_stack.push(top);
    }
}

void CharacterMapParser::ExecuteExch() {
    // exch: exchange the top two elements on the stack
    if (m_operand_stack.size() >= 2) {
        auto top = PopFromStack();
        auto second = PopFromStack();
        PushToStack(top);
        PushToStack(second);
    }
}

void CharacterMapParser::ExecutePop() {
    // pop: remove the top element from the stack
    if (!m_operand_stack.empty()) {
        m_operand_stack.pop();
    }
}

void CharacterMapParser::ExecuteDict() {
    // dict: create a new dictionary
    // Stack: int -> dict
    auto count_obj = PopFromStack();
    
    DictionaryObjectPtr new_dict;
    new_dict->SetFile(_file);
    
    PushToStack(new_dict);
}

void CharacterMapParser::ExecuteBegin() {
    // begin: push dictionary onto dictionary stack
    // Stack: dict -> -
    auto dict_obj = PopFromStack();
    if (dict_obj->GetObjectType() == Object::Type::Dictionary) {
        auto dict = ConvertUtils<ObjectPtr>::ConvertTo<DictionaryObjectPtr>(dict_obj);
        m_dictionary_stack.push_back(dict);
    }
}

void CharacterMapParser::ExecuteEnd() {
    // end: pop dictionary from dictionary stack
    if (!m_dictionary_stack.empty()) {
        m_dictionary_stack.pop_back();
    }
}

void CharacterMapParser::ExecuteDef() {
    // def: define a key-value pair in the current dictionary
    // Stack: key value -> -
    auto value = PopFromStack();
    auto key = PopFromStack();
    
    auto current_dict = GetCurrentDictionary();
    if (current_dict && key->GetObjectType() == Object::Type::Name) {
        auto name = ConvertUtils<ObjectPtr>::ConvertTo<NameObjectPtr>(key);
        if (value->GetObjectType() != Object::Type::Null) {
            auto containable = ConvertUtils<ObjectPtr>::ConvertTo<ContainableObjectPtr>(value);
            current_dict->Insert(name, containable);
        }
    }
}

void CharacterMapParser::ProcessPostScriptToken(TokenPtr token) {
    // Process a single PostScript token
    switch (token->GetType()) {
        case Token::Type::DUP:
            ExecuteDup();
            break;
        case Token::Type::EXCH:
            ExecuteExch();
            break;
        case Token::Type::STACK_POP:
            ExecutePop();
            break;
        case Token::Type::DICTIONARY:
            ExecuteDict();
            break;
        case Token::Type::BLOCK_BEGIN:
            ExecuteBegin();
            break;
        case Token::Type::BLOCK_END:
            ExecuteEnd();
            break;
        case Token::Type::DEFINITION:
            ExecuteDef();
            break;
        default:
            // Not a PostScript operator, treat as operand
            break;
    }
}

ObjectPtr CharacterMapParser::ExecutePostScriptSequence() {
    // Execute a sequence of PostScript operations until we get a result
    // This is used for parsing dictionary constructs
    
    while (true) {
        auto token = PeekTokenSkip();
        
        if (token->GetType() == Token::Type::END_OF_INPUT) {
            break;
        }
        
        // Check if this is a PostScript operator
        bool is_operator = false;
        switch (token->GetType()) {
            case Token::Type::DUP:
            case Token::Type::EXCH:
            case Token::Type::STACK_POP:
            case Token::Type::DICTIONARY:
            case Token::Type::BLOCK_BEGIN:
            case Token::Type::BLOCK_END:
            case Token::Type::DEFINITION:
                is_operator = true;
                break;
            default:
                break;
        }
        
        if (is_operator) {
            ReadTokenSkip(); // consume the operator
            ProcessPostScriptToken(token);
            
            // If we just executed 'def' and the stack is empty, we're done
            if (token->GetType() == Token::Type::DEFINITION && m_operand_stack.empty()) {
                break;
            }
        } else {
            // Read as operand and push to stack
            auto obj = ReadDirectObject();
            PushToStack(obj);
        }
    }
    
    // Return the top of the stack if anything remains
    if (!m_operand_stack.empty()) {
        return PopFromStack();
    }
    
    return NullObject::GetInstance();
}

DictionaryObjectPtr CharacterMapParser::ReadDictionary() {
    // Clear the stacks for a fresh start
    while (!m_operand_stack.empty()) {
        m_operand_stack.pop();
    }
    m_dictionary_stack.clear();
    
    // Peek ahead to determine dictionary type
    auto next_token = PeekTokenSkip();
    
    // Check for PostScript-style dictionary: "N dict dup begin"
    if (next_token->GetType() == Token::Type::INTEGER_OBJECT) {
        // This might be PostScript dictionary syntax
        return ReadPostScriptDictionary();
    }
    // Check for PDF-style dictionary: "<<"
    else if (next_token->GetType() == Token::Type::DICTIONARY_BEGIN) {
        return ReadPDFDictionaryWithDef();
    }
    
    // Fallback to base implementation
    return ParserBase::ReadDictionary();
}

DictionaryObjectPtr CharacterMapParser::ReadPostScriptDictionary() {
    // Handle PostScript dictionary syntax using stack-based execution
    // Example: /CIDSystemInfo 3 dict dup begin
    //            /Registry (Adobe) def
    //            /Ordering (Identity) def
    //            /Supplement 0 def
    //          end def
    
    // Read the integer (dictionary size)
    auto count_obj = ReadDirectObject();
    PushToStack(count_obj);
    
    // Process tokens using PostScript stack semantics
    bool in_dict_sequence = false;
    DictionaryObjectPtr result_dict;
    
    while (true) {
        auto token = PeekTokenSkip();
        
        if (token->GetType() == Token::Type::END_OF_INPUT) {
            break;
        }
        
        // Handle the dict operator
        if (token->GetType() == Token::Type::DICTIONARY) {
            ReadTokenSkip();
            ExecuteDict();
            in_dict_sequence = true;
            continue;
        }
        
        // Handle dup operator
        if (token->GetType() == Token::Type::DUP) {
            ReadTokenSkip();
            ExecuteDup();
            continue;
        }
        
        // Handle begin operator
        if (token->GetType() == Token::Type::BLOCK_BEGIN) {
            ReadTokenSkip();
            ExecuteBegin();
            continue;
        }
        
        // Handle end operator
        if (token->GetType() == Token::Type::BLOCK_END) {
            ReadTokenSkip();
            
            // Get the dictionary before we pop it from the stack
            if (!m_dictionary_stack.empty()) {
                result_dict = m_dictionary_stack.back();
            }
            
            ExecuteEnd();
            
            // Push the dictionary back onto the operand stack for potential def
            if (result_dict) {
                PushToStack(result_dict);
            }
            
            // Check if there's a def after end
            auto next = PeekTokenSkip();
            if (next->GetType() == Token::Type::DEFINITION) {
                ReadTokenSkip();
                // The def here would normally store the dictionary somewhere,
                // but for our purposes, we just return it
                break;
            }
            break;
        }
        
        // Inside the dictionary, handle key-value-def sequences
        if (!m_dictionary_stack.empty()) {
            // Read key
            auto key = ReadDirectObject();
            if (key->GetObjectType() != Object::Type::Name) {
                continue;
            }
            PushToStack(key);
            
            // Read value
            auto value = ReadDirectObject();
            PushToStack(value);
            
            // Look for def
            auto def_token = PeekTokenSkip();
            if (def_token->GetType() == Token::Type::DEFINITION) {
                ReadTokenSkip();
                ExecuteDef();
            }
        } else {
            // We're not in a dictionary context yet
            break;
        }
    }
    
    return result_dict ? result_dict : DictionaryObjectPtr();
}

DictionaryObjectPtr CharacterMapParser::ReadPDFDictionaryWithDef() {
    // Handle PDF-style dictionary with def inside: "<< /Key (Value) def ... >> def"
    // Example: /CIDSystemInfo <<
    //            /Registry (Times-Roman+0) def
    //            /Ordering (T1UV) def
    //            /Supplement 0 def
    //          >> def
    
    DictionaryObjectPtr dictionary;
    dictionary->SetFile(_file);
    
    ReadTokenWithTypeSkip(Token::Type::DICTIONARY_BEGIN);
    
    // Push the dictionary onto both stacks for def operations
    m_dictionary_stack.push_back(dictionary);
    
    while (PeekTokenTypeSkip() != Token::Type::DICTIONARY_END) {
        auto key = ReadDirectObject();
        
        // Skip if not a name
        if (key->GetObjectType() != Object::Type::Name) {
            auto key_type_str = Object::TypeName(key->GetObjectType());
            spdlog::warn("Found dictionary key with type {}, skipping", key_type_str);
            continue;
        }
        
        PushToStack(key);
        
        auto value = ReadDirectObject();
        PushToStack(value);
        
        // Handle "def" after each value in the dictionary
        auto def_token = PeekTokenSkip();
        if (def_token->GetType() == Token::Type::DEFINITION) {
            ReadTokenWithTypeSkip(Token::Type::DEFINITION);
            ExecuteDef();
        } else {
            // If no def, manually insert into dictionary
            auto name = ConvertUtils<ObjectPtr>::ConvertTo<NameObjectPtr>(key);
            if (value->GetObjectType() != Object::Type::Null) {
                if (dictionary->Contains(name)) {
                    spdlog::warn("Found duplicate entry for {}, skipping", name->ToString());
                } else {
                    auto containable = ConvertUtils<ObjectPtr>::ConvertTo<ContainableObjectPtr>(value);
                    dictionary->Insert(name, containable);
                }
            }
            // Pop the values we didn't use for def
            PopFromStack();
            PopFromStack();
        }
    }
    
    ReadTokenWithTypeSkip(Token::Type::DICTIONARY_END);
    
    // Pop dictionary from dictionary stack
    if (!m_dictionary_stack.empty()) {
        m_dictionary_stack.pop_back();
    }
    
    // Read "def" if present after ">>"
    auto final_def = PeekTokenSkip();
    if (final_def->GetType() == Token::Type::DEFINITION) {
        ReadTokenWithTypeSkip(Token::Type::DEFINITION);
    }
    
    return dictionary;
}

CharacterMapData CharacterMapParser::ReadCharacterMapData(void) {
    CharacterMapData result;

    for (;;) {
        auto token = ReadTokenSkip();
        auto ahead = PeekTokenSkip();

        if (token->GetType() == Token::Type::END_OF_INPUT) {
            return result;
        }

        // Fixed: Better token handling for various CMap format variations
        // Issue: Parser failed on files with different structural token arrangements
        if (token->GetType() == Token::Type::BLOCK_BEGIN ||
            token->GetType() == Token::Type::BLOCK_END ||
            token->GetType() == Token::Type::FIND_RESOURCE ||
            token->GetType() == Token::Type::DICTIONARY ||
            token->GetType() == Token::Type::CURRENT_DICTIONARY ||
            token->GetType() == Token::Type::DEFINE_RESOURCE ||
            token->GetType() == Token::Type::STACK_POP ||
            token->GetType() == Token::Type::DEFINITION) {
            continue;
        }

        if (token->GetType() == Token::Type::INTEGER_OBJECT) {
            if (ahead->GetType() == Token::Type::BEGIN_CODE_SPACE_RANGE) {
                ReadTokenWithTypeSkip(Token::Type::BEGIN_CODE_SPACE_RANGE);

                auto count = ParserUtils::GetIntegerValue(token);
                for (decltype(count) i = 0; i < count; ++i) {
                    CodeSpaceRange range;
                    range.Begin = ReadHexadecimalString();
                    range.End = ReadHexadecimalString();

                    range.Begin->SetEncryptionExempted();
                    range.End->SetEncryptionExempted();

                    result.CodeSpaceRanges.push_back(range);
                }

                ReadTokenWithTypeSkip(Token::Type::END_CODE_SPACE_RANGE);
            }

            if (ahead->GetType() == Token::Type::BEGIN_BASE_FONT_RANGE) {
                ReadTokenWithTypeSkip(Token::Type::BEGIN_BASE_FONT_RANGE);

                auto count = ParserUtils::GetIntegerValue(token);
                for (decltype(count) i = 0; i < count; ++i) {
                    auto low = ReadHexadecimalString();
                    auto high = ReadHexadecimalString();
                    auto dest = ReadDirectObject();

                    BaseFontRange range;
                    range.SetRangeLow(low);
                    range.SetRangeHigh(high);
                    range.SetDestination(dest);

                    range.GetRangeLow()->SetEncryptionExempted();
                    range.GetRangeHigh()->SetEncryptionExempted();
                    range.GetDestination()->SetEncryptionExempted();

                    result.BaseFontRanges.push_back(range);
                }

                ReadTokenWithTypeSkip(Token::Type::END_BASE_FONT_RANGE);
            }

            if (ahead->GetType() == Token::Type::BEGIN_BASE_FONT_CHAR) {
                ReadTokenWithTypeSkip(Token::Type::BEGIN_BASE_FONT_CHAR);

                auto count = ParserUtils::GetIntegerValue(token);
                for (decltype(count) i = 0; i < count; ++i) {
                    auto source = ReadHexadecimalString();
                    auto destination = ReadHexadecimalString();

                    FontCharMapping base_font_char_mapping;
                    base_font_char_mapping.Source = source;
                    base_font_char_mapping.Destination = destination;

                    base_font_char_mapping.Source->SetEncryptionExempted();
                    base_font_char_mapping.Destination->SetEncryptionExempted();

                    result.BaseFontCharMapping.push_back(base_font_char_mapping);
                }

                ReadTokenWithTypeSkip(Token::Type::END_BASE_FONT_CHAR);
            }

            if (ahead->GetType() == Token::Type::BEGIN_NOT_DEF_RANGE) {
                ReadTokenWithTypeSkip(Token::Type::BEGIN_NOT_DEF_RANGE);

                auto count = ParserUtils::GetIntegerValue(token);
                for (decltype(count) i = 0; i < count; ++i) {
                    auto low = ReadHexadecimalString();
                    auto high = ReadHexadecimalString();
                    auto dest = ReadDirectObject();

                    BaseFontRange range;
                    range.SetRangeLow(low);
                    range.SetRangeHigh(high);
                    range.SetDestination(dest);

                    range.GetRangeLow()->SetEncryptionExempted();
                    range.GetRangeHigh()->SetEncryptionExempted();
                    range.GetDestination()->SetEncryptionExempted();

                    result.NotDefinedRanges.push_back(range);
                }

                ReadTokenWithTypeSkip(Token::Type::END_NOT_DEF_RANGE);
            }

            if (ahead->GetType() == Token::Type::BEGIN_NOT_DEF_CHAR) {
                ReadTokenWithTypeSkip(Token::Type::BEGIN_NOT_DEF_CHAR);

                auto count = ParserUtils::GetIntegerValue(token);
                for (decltype(count) i = 0; i < count; ++i) {
                    auto source = ReadHexadecimalString();
                    auto destination = ReadHexadecimalString();

                    FontCharMapping not_defined_char_mapping;
                    not_defined_char_mapping.Source = source;
                    not_defined_char_mapping.Destination = destination;

                    not_defined_char_mapping.Source->SetEncryptionExempted();
                    not_defined_char_mapping.Destination->SetEncryptionExempted();

                    result.NotDefinedCharMapping.push_back(not_defined_char_mapping);
                }

                ReadTokenWithTypeSkip(Token::Type::END_NOT_DEF_CHAR);
            }

            if (ahead->GetType() == Token::Type::BEGIN_CID_RANGE) {
                ReadTokenWithTypeSkip(Token::Type::BEGIN_CID_RANGE);

                auto count = ParserUtils::GetIntegerValue(token);
                for (decltype(count) i = 0; i < count; ++i) {
                    auto low = ReadHexadecimalString();
                    auto high = ReadHexadecimalString();
                    auto dest = ReadDirectObject();

                    BaseFontRange range;
                    range.SetRangeLow(low);
                    range.SetRangeHigh(high);
                    range.SetDestination(dest);

                    range.GetRangeLow()->SetEncryptionExempted();
                    range.GetRangeHigh()->SetEncryptionExempted();
                    range.GetDestination()->SetEncryptionExempted();

                    result.CIDRanges.push_back(range);
                }

                ReadTokenWithTypeSkip(Token::Type::END_CID_RANGE);
            }

            if (ahead->GetType() == Token::Type::BEGIN_CID_CHAR) {
                ReadTokenWithTypeSkip(Token::Type::BEGIN_CID_CHAR);

                auto count = ParserUtils::GetIntegerValue(token);
                for (decltype(count) i = 0; i < count; ++i) {
                    auto source = ReadHexadecimalString();
                    auto destination = ReadHexadecimalString();

                    FontCharMapping cid_char_mapping;
                    cid_char_mapping.Source = source;
                    cid_char_mapping.Destination = destination;

                    cid_char_mapping.Source->SetEncryptionExempted();
                    cid_char_mapping.Destination->SetEncryptionExempted();

                    result.CIDCharMapping.push_back(cid_char_mapping);
                }

                ReadTokenWithTypeSkip(Token::Type::END_CID_CHAR);
            }
        }

        if (token->GetType() == Token::Type::NAME_OBJECT) {
            auto name = ParserUtils::CreateName(token);

            if (name == constant::Name::CIDSystemInfo) {
                DictionaryObjectPtr system_info = ReadDictionary();
                system_info->SetEncryptionExempted();

                result.SystemInfo.Registry = system_info->FindAs<StringObjectPtr>(constant::Name::Registry);
                result.SystemInfo.Ordering = system_info->FindAs<StringObjectPtr>(constant::Name::Ordering);
                result.SystemInfo.Supplement = system_info->FindAs<IntegerObjectPtr>(constant::Name::Supplement);

                // Fixed: 'def' keyword is optional in CMap files - check before consuming
                // Issue: Parser failed on files with << >> dict syntax or missing 'def' keywords
                auto next_token = PeekTokenSkip();
                if (next_token->GetType() == Token::Type::DEFINITION) {
                    ReadTokenWithTypeSkip(Token::Type::DEFINITION);
                }
            }

            if (name == constant::Name::CMapName) {
                result.CMapName = ReadName();
                // Fixed: Same optional 'def' handling as CIDSystemInfo
                auto next_token = PeekTokenSkip();
                if (next_token->GetType() == Token::Type::DEFINITION) {
                    ReadTokenWithTypeSkip(Token::Type::DEFINITION);
                }
            }

            if (name == constant::Name::CMapType) {
                result.CMapType = ReadInteger();
                // Fixed: Same optional 'def' handling as CIDSystemInfo
                auto next_token = PeekTokenSkip();
                if (next_token->GetType() == Token::Type::DEFINITION) {
                    ReadTokenWithTypeSkip(Token::Type::DEFINITION);
                }
            }

            // Fixed: Added WMode parameter support - was missing from original parser
            if (name->ToString() == "WMode") {
                auto wmode_value = ReadInteger();
                auto next_token = PeekTokenSkip();
                if (next_token->GetType() == Token::Type::DEFINITION) {
                    ReadTokenWithTypeSkip(Token::Type::DEFINITION);
                }
            }
        }
    }
}

} // contents
} // vanillapdf
