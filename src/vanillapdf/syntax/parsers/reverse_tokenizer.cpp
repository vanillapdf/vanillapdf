#include "precompiled.h"

#include "syntax/parsers/reverse_tokenizer.h"
#include "utils/character.h"
#include "utils/util.h"

#include <cassert>
#include <algorithm>
#include <iomanip>

namespace vanillapdf {
namespace syntax {

ReverseTokenizer::ReverseTokenizer(IInputStreamPtr stream)
	: m_stream(stream),
	_last_token_offset(constant::BAD_OFFSET),
	_advance_position(constant::BAD_OFFSET) {
	_dictionary.Initialize();
}

TokenPtr ReverseTokenizer::ReadToken() {
	auto current_offset = m_stream->GetInputPosition();
	if (_token_cached && _last_token_offset == current_offset) {
		assert(constant::BAD_OFFSET != _last_token_offset);

		auto result = _last_token;

		m_stream->SetInputPosition(_advance_position);
		_last_token_offset = constant::BAD_OFFSET;
		_advance_position = constant::BAD_OFFSET;
		_token_cached = false;

		return result;
	}

	for (;;) {

		if (m_stream->Eof()) {
			return make_deferred<Token>(Token::Type::END_OF_INPUT);
		}

		// Test if the next character is EOF
		int peek_test = m_stream->Peek();
		if (peek_test == std::char_traits<char>::eof()) {
			return make_deferred<Token>(Token::Type::END_OF_INPUT);
		}

		int ch = m_stream->Get();

		switch (ch) {
			case static_cast<int>(WhiteSpace::LINE_FEED):
			{
				int carriage_return = m_stream->Peek();
				if (carriage_return == WhiteSpace::CARRIAGE_RETURN) {
					m_stream->Ignore();
				}

				return make_deferred<Token>(Token::Type::REVERSE_EOL);
			}
			case static_cast<int>(WhiteSpace::SPACE):
			case static_cast<int>(WhiteSpace::FORM_FEED):
			case static_cast<int>(WhiteSpace::HORIZONTAL_TAB):
			case static_cast<int>(WhiteSpace::NUL):
				continue;
			case static_cast<int>(WhiteSpace::CARRIAGE_RETURN):
				return make_deferred<Token>(Token::Type::REVERSE_EOL);
		}

		return ReadUnknown(ch);
	};
}

TokenPtr ReverseTokenizer::ReadUnknown(int ch) {
	auto current = ValueConvertUtils::SafeConvert<unsigned char>(ch);

	std::string chars;
	chars.push_back(current);

	if (IsNumeric(current)) {
		while (IsNumeric(m_stream->Peek())) {
			auto numeric = static_cast<unsigned char>(m_stream->Get());
			chars.push_back(numeric);
		}

		while ((m_stream->Peek() == '+') || (m_stream->Peek() == '-')) {
			auto next = static_cast<unsigned char>(m_stream->Get());
			chars.push_back(next);
		}

		return make_deferred<Token>(Token::Type::REVERSE_INTEGER_OBJECT, chars);
	}

	for (;;) {
		auto next_meta = m_stream->Peek();
		if (std::char_traits<char>::eof() == next_meta) {
			break;
		}

		auto next = ValueConvertUtils::SafeConvert<unsigned char>(next_meta);
		if (IsWhiteSpace(next)) {
			break;
		}

		chars.push_back(next);
		m_stream->Ignore();
	}

	auto result_type = _dictionary.Find(chars);
	return make_deferred<Token>(result_type, chars);
}

/* Peek need cache */
TokenPtr ReverseTokenizer::PeekToken() {
	auto current = m_stream->GetInputPosition();
	if (_token_cached && _last_token_offset == current) {
		assert(constant::BAD_OFFSET != _advance_position);
		assert(constant::BAD_OFFSET != _last_token_offset);

		return _last_token;
	}

	_last_token = ReadToken();
	_advance_position = m_stream->GetInputPosition();
	_last_token_offset = current;
	_token_cached = true;

	if (constant::BAD_OFFSET == _advance_position && constant::BAD_OFFSET == _last_token_offset) {
		assert(_last_token->GetType() == Token::Type::END_OF_INPUT);
	}

	m_stream->SetInputPosition(_last_token_offset);
	return _last_token;
}

TokenPtr ReverseTokenizer::ReadTokenWithType(Token::Type type) {
	auto current_type = PeekTokenType();
	if (current_type != type) {
		throw GeneralException("Unexpected token type");
	}

	return ReadToken();
}

Token::Type ReverseTokenizer::PeekTokenType(void) {
	auto current = PeekToken();
	return current->GetType();
}

} // syntax
} // vanillapdf
