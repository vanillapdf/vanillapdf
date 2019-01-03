#include "precompiled.h"

#include "syntax/parsers/tokenizer.h"

#include "utils/constants.h"
#include "utils/character.h"
#include "utils/util.h"

#include <cassert>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace vanillapdf {
namespace syntax {

Tokenizer::Tokenizer(IInputStreamPtr stream) : m_stream(stream) {
}

TokenPtr Tokenizer::ReadToken() {
	auto current_offset = m_stream->GetInputPosition();
	if (IsItemCached(current_offset)) {
		auto cache_item = GetCachedItem(current_offset);

		m_stream->SetInputPosition(cache_item.advance_position);

		return cache_item.token;
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
			case static_cast<int>(WhiteSpace::SPACE):
			case static_cast<int>(WhiteSpace::FORM_FEED):
			case static_cast<int>(WhiteSpace::HORIZONTAL_TAB):
			case static_cast<int>(WhiteSpace::NUL):
				continue;
			case static_cast<int>(Delimiter::PERCENT_SIGN):
				// We are currently silently ignoring all comments
				ReadComment();
				continue;
			case static_cast<int>(WhiteSpace::LINE_FEED):
			{
				BufferPtr chars;
				chars->push_back(WhiteSpace::LINE_FEED);
				return make_deferred<Token>(Token::Type::EOL, chars);
			}
			case static_cast<int>(WhiteSpace::CARRIAGE_RETURN):
			{
				BufferPtr chars;
				chars->push_back(WhiteSpace::CARRIAGE_RETURN);

				int line_feed = m_stream->Peek();
				if (line_feed == WhiteSpace::LINE_FEED && m_stream->Ignore()) {
					chars->push_back(WhiteSpace::LINE_FEED);
				}

				return make_deferred<Token>(Token::Type::EOL, chars);
			}
			case static_cast<int>(Delimiter::GREATER_THAN_SIGN):
			{
				int sign = m_stream->Peek();
				if (sign == Delimiter::GREATER_THAN_SIGN && m_stream->Ignore()) {
					BufferPtr chars;

					chars->push_back(Delimiter::GREATER_THAN_SIGN);
					chars->push_back(Delimiter::GREATER_THAN_SIGN);
					return make_deferred<Token>(Token::Type::DICTIONARY_END, chars);
				}

				throw GeneralException("Unexpected character at offset: " + std::to_string(m_stream->GetInputPosition()));
			}
			case static_cast<int>(Delimiter::LESS_THAN_SIGN):
			{
				int sign = m_stream->Peek();
				if (sign == Delimiter::LESS_THAN_SIGN && m_stream->Ignore()) {
					BufferPtr chars;

					chars->push_back(Delimiter::LESS_THAN_SIGN);
					chars->push_back(Delimiter::LESS_THAN_SIGN);
					return make_deferred<Token>(Token::Type::DICTIONARY_BEGIN, chars);
				}

				// empty hexadecimal string
				if (sign == Delimiter::GREATER_THAN_SIGN && m_stream->Ignore()) {
					return make_deferred<Token>(Token::Type::HEXADECIMAL_STRING);
				}

				return ReadHexadecimalString();
			}
			case static_cast<int>(Delimiter::LEFT_SQUARE_BRACKET):
			{
				BufferPtr chars;

				chars->push_back(Delimiter::LEFT_SQUARE_BRACKET);
				return make_deferred<Token>(Token::Type::ARRAY_BEGIN, chars);
			}
			case static_cast<int>(Delimiter::RIGHT_SQUARE_BRACKET):
			{
				BufferPtr chars;

				chars->push_back(Delimiter::RIGHT_SQUARE_BRACKET);
				return make_deferred<Token>(Token::Type::ARRAY_END, chars);
			}
			case static_cast<int>(Delimiter::SOLIDUS):
				return ReadName();
			case static_cast<int>(Delimiter::LEFT_PARENTHESIS):
				return ReadLiteralString();
		}

		if (ch == 'R') {
			BufferPtr chars;

			chars->push_back('R');
			return make_deferred<Token>(Token::Type::INDIRECT_REFERENCE_MARKER, chars);
		}

		return ReadUnknown(ch);
	};
}

TokenPtr Tokenizer::ReadUnknown(int ch) {
	auto current = ValueConvertUtils::SafeConvert<unsigned char>(ch);
	bool has_dot = (current == '.');
	if (IsNumeric(current) || (current == '+') || (current == '-') || has_dot) {
		BufferPtr chars;

		chars->push_back(current);
		while (IsNumeric(m_stream->Peek())) {
			auto numeric = static_cast<char>(m_stream->Get());
			chars->push_back(numeric);
		}

		if (has_dot) {
			return make_deferred<Token>(Token::Type::REAL_OBJECT, chars);
		}

		if (m_stream->Eof() || m_stream->Peek() != '.') {
			return make_deferred<Token>(Token::Type::INTEGER_OBJECT, chars);
		}

		// The next character is dot
		chars->push_back('.');

		// Consume it
		m_stream->Ignore();

		while (IsNumeric(m_stream->Peek())) {
			auto next = static_cast<char>(m_stream->Get());
			chars->push_back(next);
		}

		return make_deferred<Token>(Token::Type::REAL_OBJECT, chars);
	}

	BufferPtr chars;

	// Insert the character we read at the beginning
	chars->push_back(current);

	for (;;) {
		if (m_stream->Eof()) {
			break;
		}

		auto next_meta = m_stream->Peek();

		// Terminate at the end of the stream
		if (std::char_traits<char>::eof() == next_meta) {
			break;
		}

		auto next = ValueConvertUtils::SafeConvert<unsigned char>(next_meta);

		// Terminate on non-regular character such as whitespace
		if (!IsRegular(next)) {
			break;
		}

		// Insert regular character into collection
		chars->push_back(next);

		// Ignore the input, because we used peek and not get
		m_stream->Ignore();
	}

	auto result_type = _dictionary->Find(chars);
	return make_deferred<Token>(result_type, chars);
}

TokenPtr Tokenizer::ReadComment(void) {
	// Comments are currently discarded

	for (;;) {
		auto eof_test = m_stream->Peek();
		if (eof_test == std::char_traits<char>::eof()) {
			break;
		}

		auto current_meta = m_stream->Get();
		if (current_meta == '\r') {
			auto line_feed = m_stream->Peek();
			if (line_feed == '\n') {
				m_stream->Ignore();
			}

			break;
		}

		if (current_meta == '\n') {
			break;
		}
	}

	return TokenPtr();
}

TokenPtr Tokenizer::ReadHexadecimalString(void) {
	BufferPtr chars;

	for (;;) {
		auto eof_test = m_stream->Peek();
		if (eof_test == std::char_traits<char>::eof()) {
			break;
		}

		auto current_meta = m_stream->Get();
		auto current = ValueConvertUtils::SafeConvert<unsigned char>(current_meta);
		if (current == Delimiter::GREATER_THAN_SIGN) {
			break;
		}

		if (IsNumeric(current) || IsAlpha(current)) {
			chars->push_back(current);
			continue;
		}

		throw GeneralException("Unexpected character in hexadecimal string " + std::to_string(current));
	}

	return make_deferred<Token>(Token::Type::HEXADECIMAL_STRING, chars);
}

TokenPtr Tokenizer::ReadName(void) {
	BufferPtr chars;

	while (IsRegular(m_stream->Peek())) {
		auto current = static_cast<char>(m_stream->Get());
		if (current == '#') {
			auto values = m_stream->Read(2);
			auto str = values->ToString();
			auto val = stoi(str, 0, 16);
			auto parsed = ValueConvertUtils::SafeConvert<unsigned char, int>(val);
			char converted = reinterpret_cast<char&>(parsed);
			chars->push_back(converted);
			continue;
		}

		chars->push_back(current);
	}

	return make_deferred<Token>(Token::Type::NAME_OBJECT, chars);
}

TokenPtr Tokenizer::ReadLiteralString(void)	{
	BufferPtr chars;

	int nested_count = 0;
	for (;;) {
		auto eof_test = m_stream->Peek();
		if (eof_test == std::char_traits<char>::eof()) {
			throw GeneralException("Improperly terminated literal string sequence: " + chars->ToString());
		}

		int current_meta = m_stream->Get();
		auto current = ValueConvertUtils::SafeConvert<unsigned char>(current_meta);

		if (current == Delimiter::LEFT_PARENTHESIS) {
			nested_count++;
		}

		if (current == Delimiter::RIGHT_PARENTHESIS) {
			if (0 == nested_count) {
				break;
			}

			nested_count--;
			assert(nested_count >= 0);
			continue;
		}

		if (current == '\r') {
			auto line_feed = m_stream->Peek();
			if (line_feed == '\n') {
				m_stream->Ignore();
			}

			chars->push_back(WhiteSpace::LINE_FEED);
			continue;
		}

		if (current != '\\') {
			chars->push_back(current);
			continue;
		}

		auto next = m_stream->Peek();
		if (next == std::char_traits<char>::eof()) {
			continue;
		}

		// escaped characters
		if (next == 'r' && m_stream->Ignore()) {
			chars->push_back('\r');
			continue;
		}

		if (next == 'f' && m_stream->Ignore()) {
			chars->push_back('\f');
			continue;
		}

		if (next == 't' && m_stream->Ignore()) {
			chars->push_back('\t');
			continue;
		}

		if (next == 'n' && m_stream->Ignore()) {
			chars->push_back('\n');
			continue;
		}

		if (next == 'b' && m_stream->Ignore()) {
			chars->push_back('\b');
			continue;
		}

		if (next == '(' && m_stream->Ignore()) {
			chars->push_back('(');
			continue;
		}

		if (next == ')' && m_stream->Ignore()) {
			chars->push_back(')');
			continue;
		}

		if (next == '\\' && m_stream->Ignore()) {
			chars->push_back('\\');
			continue;
		}

		// Backslash at the EOL shall be disregarded
		if (next == '\r' && m_stream->Ignore()) {
			if (m_stream->Peek() == '\n') {
				m_stream->Ignore();
			}

			continue;
		}

		// Backslash at the EOL shall be disregarded
		if (next == '\n' && m_stream->Ignore()) {
			continue;
		}

		if (!IsNumeric(next)) {
			continue;
		}

		std::stringstream octal;
		for (int i = 0; i < 3; ++i) {
			auto numeric_meta = m_stream->Peek();
			if (std::char_traits<char>::eof() == numeric_meta) {
				break;
			}

			auto numeric = ValueConvertUtils::SafeConvert<unsigned char>(numeric_meta);
			if (IsNumeric(numeric) && m_stream->Ignore()) {
				octal << numeric;
				continue;
			}

			assert(!"Found invalid value in octal representation inside literal string");
			throw GeneralException("Expected octal value inside literal string: " + octal.str());
		}

		int value = 0;
		octal >> std::oct >> value;
		auto converted = ValueConvertUtils::SafeConvert<unsigned char, int>(value);
		char char_converted = reinterpret_cast<char&>(converted);
		chars->push_back(char_converted);
		continue;
	}

	return make_deferred<Token>(Token::Type::LITERAL_STRING, chars);
}

bool Tokenizer::IsItemCached(types::stream_offset offset) {
	return (_cache.find(offset) != _cache.end());
}

const Tokenizer::CacheItem& Tokenizer::GetCachedItem(types::stream_offset offset) {
	return _cache[offset];
}

/* Peek need cache */
TokenPtr Tokenizer::PeekToken() {
	auto current = m_stream->GetInputPosition();
	if (IsItemCached(current)) {
		auto cache_item = GetCachedItem(current);
		return cache_item.token;
	}

	CacheItem cache_item;
	cache_item.token = ReadToken();
	cache_item.advance_position = m_stream->GetInputPosition();

	if (_cache.size() == constant::TOKENIZER_CACHE_SIZE) {
		_cache.erase(_cache.begin());
	}

	_cache[current] = cache_item;

	if (constant::BAD_OFFSET == cache_item.advance_position && constant::BAD_OFFSET == current) {
		assert(cache_item.token->GetType() == Token::Type::END_OF_INPUT);
	}

	m_stream->SetInputPosition(current);
	return cache_item.token;
}

TokenPtr Tokenizer::ReadTokenWithType(Token::Type type) {
	auto current_type = PeekTokenType();
	if (current_type != type) {
		throw GeneralException("Unexpected token type");
	}

	return ReadToken();
}

Token::Type Tokenizer::PeekTokenType(void) {
	auto current = PeekToken();
	return current->GetType();
}

} // syntax
} // vanillapdf
