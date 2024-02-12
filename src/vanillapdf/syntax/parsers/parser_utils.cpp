#include "precompiled.h"

#include "syntax/parsers/parser_utils.h"
#include "syntax/parsers/token.h"


namespace vanillapdf {
namespace syntax {

BooleanObjectPtr ParserUtils::CreateBoolean(TokenPtr token) {
	if (token->GetType() == Token::Type::TRUE_VALUE)
		return make_deferred<BooleanObject>(true);

	if (token->GetType() == Token::Type::FALSE_VALUE)
		return make_deferred<BooleanObject>(false);

	assert(!"Expected boolean token type");
	throw GeneralException("Expected boolean token type");
}

types::big_int ParserUtils::GetIntegerValue(TokenPtr token) {
	assert(token->GetType() == Token::Type::INTEGER_OBJECT && "Expected integer token type");

	auto buffer = token->Value();
	return std::stoll(buffer->ToString());
}

IntegerObjectPtr ParserUtils::CreateInteger(TokenPtr token) {
	assert(token->GetType() == Token::Type::INTEGER_OBJECT && "Expected integer token type");

	types::big_int value = GetIntegerValue(token);
	return make_deferred<IntegerObject>(value);
}

RealObjectPtr ParserUtils::CreateReal(TokenPtr token) {
	assert(token->GetType() == Token::Type::REAL_OBJECT && "Expected real token type");

	auto buffer = token->Value();
	auto str = buffer->ToString();
	auto value = std::stod(str);
	auto pos = str.rfind('.');
	if (pos != std::string::npos) {
		auto precision = str.size() - pos - 1;
		auto converted = ValueConvertUtils::SafeConvert<uint32_t>(precision);
		return make_deferred<RealObject>(value, converted);
	}

	return make_deferred<RealObject>(value);
}

NameObjectPtr ParserUtils::CreateName(TokenPtr token) {
	assert(token->GetType() == Token::Type::NAME_OBJECT && "Expected name token type");

	auto buffer = token->Value();
	return NameObject::CreateFromDecoded(buffer);
}

HexadecimalStringObjectPtr ParserUtils::CreateHexString(TokenPtr token) {
	assert(token->GetType() == Token::Type::HEXADECIMAL_STRING && "Expected hexadecimal string token type");

	auto buffer = token->Value();
	return HexadecimalStringObject::CreateFromEncoded(buffer);
}

LiteralStringObjectPtr ParserUtils::CreateLitString(TokenPtr token) {
	assert(token->GetType() == Token::Type::LITERAL_STRING && "Expected literal string token type");

	auto buffer = token->Value();
	return LiteralStringObject::CreateFromEncoded(buffer);
}

} // syntax
} // vanillapdf
