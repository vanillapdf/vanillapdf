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

DictionaryObjectPtr CharacterMapParser::ReadDictionary() {
    // Peek ahead to determine dictionary type
    auto next_token = PeekTokenSkip();
    
    // Check for PostScript-style dictionary: "N dict dup begin"
    if (next_token->GetType() == Token::Type::DICTIONARY) {
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
    // Handle PostScript dictionary syntax: "N dict dup begin ... end def"
    // Example: /CIDSystemInfo 3 dict dup begin
    //            /Registry (Adobe) def
    //            /Ordering (Identity) def
    //            /Supplement 0 def
    //          end def
    
    DictionaryObjectPtr dictionary;
    dictionary->SetFile(_file);
    
    // Read the number (e.g., "3")
    auto count_token = PeekTokenSkip();
    if (count_token->GetType() == Token::Type::INTEGER_OBJECT) {
        ReadTokenSkip(); // consume the number
    }
    
    // Read "dict"
    auto dict_token = PeekTokenSkip();
    if (dict_token->GetType() == Token::Type::DICTIONARY) {
        ReadTokenWithTypeSkip(Token::Type::DICTIONARY);
    }
    
    // Read "dup" if present
    auto dup_token = PeekTokenSkip();
    if (dup_token->GetType() == Token::Type::DUP) {
        ReadTokenWithTypeSkip(Token::Type::DUP);
    }
    
    // Read "begin"
    auto begin_token = PeekTokenSkip();
    if (begin_token->GetType() == Token::Type::BLOCK_BEGIN) {
        ReadTokenWithTypeSkip(Token::Type::BLOCK_BEGIN);
    }
    
    // Read dictionary entries until "end"
    while (PeekTokenTypeSkip() != Token::Type::BLOCK_END) {
        auto key = ReadDirectObject();
        
        // Skip if not a name
        if (key->GetObjectType() != Object::Type::Name) {
            continue;
        }
        
        auto name = ConvertUtils<ObjectPtr>::ConvertTo<NameObjectPtr>(key);
        auto value = ReadDirectObject();
        
        // Skip "def" if present after the value
        auto def_token = PeekTokenSkip();
        if (def_token->GetType() == Token::Type::DEFINITION) {
            ReadTokenWithTypeSkip(Token::Type::DEFINITION);
        }
        
        if (value->GetObjectType() != Object::Type::Null) {
            auto containable = ConvertUtils<ObjectPtr>::ConvertTo<ContainableObjectPtr>(value);
            dictionary->Insert(name, containable);
        }
    }
    
    // Read "end"
    ReadTokenWithTypeSkip(Token::Type::BLOCK_END);
    
    // Read "def" if present after "end"
    auto final_def = PeekTokenSkip();
    if (final_def->GetType() == Token::Type::DEFINITION) {
        ReadTokenWithTypeSkip(Token::Type::DEFINITION);
    }
    
    return dictionary;
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
    
    while (PeekTokenTypeSkip() != Token::Type::DICTIONARY_END) {
        auto key = ReadDirectObject();
        
        // Skip if not a name
        if (key->GetObjectType() != Object::Type::Name) {
            auto key_type_str = Object::TypeName(key->GetObjectType());
            spdlog::warn("Found dictionary key with type {}, skipping", key_type_str);
            continue;
        }
        
        auto name = ConvertUtils<ObjectPtr>::ConvertTo<NameObjectPtr>(key);
        auto value = ReadDirectObject();
        
        // Handle "def" after each value in the dictionary
        auto def_token = PeekTokenSkip();
        if (def_token->GetType() == Token::Type::DEFINITION) {
            ReadTokenWithTypeSkip(Token::Type::DEFINITION);
        }
        
        if (value->GetObjectType() != Object::Type::Null) {
            if (dictionary->Contains(name)) {
                spdlog::warn("Found duplicate entry for {}, skipping", name->ToString());
                continue;
            }
            
            auto containable = ConvertUtils<ObjectPtr>::ConvertTo<ContainableObjectPtr>(value);
            dictionary->Insert(name, containable);
        }
    }
    
    ReadTokenWithTypeSkip(Token::Type::DICTIONARY_END);
    
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
