#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/parsers/parser_utils.h"
#include "syntax/utils/name_constants.h"

#include "contents/character_map_parser.h"

namespace vanillapdf {
namespace contents {

using namespace vanillapdf::syntax;

CharacterMapParser::CharacterMapParser(WeakReference<File> file, IInputStreamPtr stream)
	: ParserBase(file, stream) {
	_dictionary = make_unique<CharacterMapTokenDictionary>();
	_dictionary->Initialize();
}

CharacterMapData CharacterMapParser::ReadCharacterMapData(void) {
	CharacterMapData result;

	for (;;) {
		auto token = ReadTokenSkip();
		auto ahead = PeekTokenSkip();

		if (token->GetType() == Token::Type::END_OF_INPUT) {
			return result;
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

				ReadTokenWithTypeSkip(Token::Type::DEFINITION);
			}

			if (name == constant::Name::CMapName) {
				result.CMapName = ReadName();
				ReadTokenWithTypeSkip(Token::Type::DEFINITION);
			}

			if (name == constant::Name::CMapType) {
				result.CMapType = ReadInteger();
				ReadTokenWithTypeSkip(Token::Type::DEFINITION);
			}
		}
	}
}

} // contents
} // vanillapdf
