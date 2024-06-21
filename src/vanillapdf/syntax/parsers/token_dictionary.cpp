#include "precompiled.h"

#include "syntax/parsers/token_dictionary.h"

#include <cassert>
#include <algorithm>

#define INSERT_TO_DICTIONARY(name) m_dictionary.insert({ std::string(name, sizeof(name) - 1), Token::Type::name })
//#define INSERT_TO_DICTIONARY(name) m_dictionary.Insert(std::string_view(name, sizeof(name) - 1), Token::Type::name)

namespace vanillapdf {
namespace syntax {

// parser tokens
static const char STREAM_BEGIN[] = "stream";
static const char STREAM_END[] = "endstream";
static const char INDIRECT_OBJECT_BEGIN[] = "obj";
static const char INDIRECT_OBJECT_END[] = "endobj";
static const char XREF_MARKER[] = "xref";
static const char START_XREF[] = "startxref";
static const char TRAILER[] = "trailer";
static const char NULL_OBJECT[] = "null";
static const char TRUE_VALUE[] = "true";
static const char FALSE_VALUE[] = "false";
static const char INDIRECT_REFERENCE_MARKER[] = "R";

// reverse tokens
static const char REVERSE_END_OF_FILE_MARKER[] = "FOE%%";
static const char REVERSE_START_XREF[] = "ferxtrats";

// character map
static const char BLOCK_BEGIN[] = "begin";
static const char BLOCK_END[] = "end";
static const char FIND_RESOURCE[] = "findresource";
static const char DICTIONARY[] = "dict";
static const char BEGIN_CMAP[] = "begincmap";
static const char END_CMAP[] = "endcmap";
static const char BEGIN_CODE_SPACE_RANGE[] = "begincodespacerange";
static const char END_CODE_SPACE_RANGE[] = "endcodespacerange";
static const char BEGIN_BASE_FONT_RANGE[] = "beginbfrange";
static const char END_BASE_FONT_RANGE[] = "endbfrange";
static const char BEGIN_BASE_FONT_CHAR[] = "beginbfchar";
static const char END_BASE_FONT_CHAR[] = "endbfchar";
static const char BEGIN_NOT_DEF_RANGE[] = "beginnotdefrange";
static const char END_NOT_DEF_RANGE[] = "endnotdefrange";
static const char BEGIN_NOT_DEF_CHAR[] = "beginnotdefchar";
static const char END_NOT_DEF_CHAR[] = "endnotdefchar";
static const char BEGIN_CID_RANGE[] = "begincidrange";
static const char END_CID_RANGE[] = "endcidrange";
static const char BEGIN_CID_CHAR[] = "begincidchar";
static const char END_CID_CHAR[] = "endcidchar";
static const char DEFINITION[] = "def";
static const char CMAP_NAME[] = "CMapName";
static const char CURRENT_DICTIONARY[] = "currentdict";
static const char DEFINE_RESOURCE[] = "defineresource";
static const char STACK_POP[] = "pop";

// content streams
static const char LINE_WIDTH[] = "w";
static const char LINE_CAP[] = "J";
static const char LINE_JOIN[] = "j";
static const char MITER_LIMIT[] = "M";
static const char DASH_PATTERN[] = "d";
static const char COLOR_RENDERING_INTENT[] = "ri";
static const char FLATNESS[] = "i";
static const char GRAPHICS_STATE[] = "gs";
static const char SAVE_GRAPHICS_STATE[] = "q";
static const char RESTORE_GRAPHIC_STATE[] = "Q";
static const char TRANSFORMATION_MATRIX[] = "cm";
static const char BEGIN_SUBPATH[] = "m";
static const char LINE[] = "l";
static const char FULL_CURVE[] = "c";
static const char FINAL_CURVE[] = "v";
static const char INITIAL_CURVE[] = "y";
static const char CLOSE_SUBPATH[] = "h";
static const char RECTANGLE[] = "re";
static const char STROKE[] = "S";
static const char CLOSE_AND_STROKE[] = "s";
static const char FILL_PATH_NONZERO[] = "f";
static const char FILL_PATH_COMPATIBILITY[] = "F";
static const char FILL_PATH_EVEN_ODD[] = "f*";
static const char FILL_STROKE_NONZERO[] = "B";
static const char FILL_STROKE_EVEN_ODD[] = "B*";
static const char CLOSE_FILL_STROKE_NONZERO[] = "b";
static const char CLOSE_FILL_STROKE_EVEN_ODD[] = "b*";
static const char END_PATH[] = "n";
static const char CLIP_PATH_NONZERO[] = "W";
static const char CLIP_PATH_EVEN_ODD[] = "W*";
static const char BEGIN_TEXT[] = "BT";
static const char END_TEXT[] = "ET";
static const char CHARACTER_SPACING[] = "Tc";
static const char WORD_SPACING[] = "Tw";
static const char HORIZONTAL_SCALING[] = "Tz";
static const char LEADING[] = "TL";
static const char TEXT_FONT[] = "Tf";
static const char TEXT_RENDERING_MODE[] = "Tr";
static const char TEXT_RISE[] = "Ts";
static const char TEXT_TRANSLATE[] = "Td";
static const char TEXT_TRANSLATE_LEADING[] = "TD";
static const char TEXT_MATRIX[] = "Tm";
static const char TEXT_NEXT_LINE[] = "T*";
static const char TEXT_SHOW[] = "Tj";
static const char TEXT_SHOW_ARRAY[] = "TJ";
static const char TEXT_NEXT_LINE_SHOW[] = "'";
static const char TEXT_NEXT_LINE_SHOW_SPACING[] = "\"";
static const char SET_CHAR_WIDTH[] = "d0";
static const char SET_CACHE_DEVICE[] = "d1";

static const char SET_STROKING_COLOR_SPACE_NAME[] = "CS";
static const char SET_NONSTROKING_COLOR_SPACE_NAME[] = "cs";
static const char SET_STROKING_COLOR_SPACE_DEVICE[] = "SC";
static const char SET_STROKING_COLOR_SPACE_DEVICE_EXTENDED[] = "SCN";
static const char SET_NONSTROKING_COLOR_SPACE_DEVICE[] = "sc";
static const char SET_NONSTROKING_COLOR_SPACE_DEVICE_EXTENDED[] = "scn";
static const char SET_STROKING_COLOR_SPACE_GRAY[] = "G";
static const char SET_NONSTROKING_COLOR_SPACE_GRAY[] = "g";
static const char SET_STROKING_COLOR_SPACE_RGB[] = "RG";
static const char SET_NONSTROKING_COLOR_SPACE_RGB[] = "rg";
static const char SET_STROKING_COLOR_SPACE_CMYK[] = "K";
static const char SET_NONSTROKING_COLOR_SPACE_CMYK[] = "k";
static const char SHADING_PAINT[] = "sh";
static const char BEGIN_INLINE_IMAGE_OBJECT[] = "BI";
static const char BEGIN_INLINE_IMAGE_DATA[] = "ID";
static const char END_INLINE_IMAGE_OBJECT[] = "EI";
static const char INVOKE_X_OBJECT[] = "Do";
static const char DEFINE_MARKED_CONTENT_POINT[] = "MP";
static const char DEFINE_MARKED_CONTENT_POINT_WITH_PROPERTY_LIST[] = "DP";
static const char BEGIN_MARKED_CONTENT_SEQUENCE[] = "BMC";
static const char BEGIN_MARKED_CONTENT_SEQUENCE_WITH_PROPERTY_LIST[] = "BDC";
static const char END_MARKED_CONTENT_SEQUENCE[] = "EMC";
static const char BEGIN_COMPATIBILITY_SECTION[] = "BX";
static const char END_COMPATIBILITY_SECTION[] = "EX";

TokenDictionaryBase::~TokenDictionaryBase() {}

Token::Type TokenDictionaryBase::Find(const std::string& set) {
	Initialize();

	// std::map
	// ------------------------------------------------------------------------ -
	// Benchmark                               Time             CPU   Iterations
	// ------------------------------------------------------------------------ -
	// BM_Contents_ParseContentStream     113394 ns        69054 ns         7467

	// Custom lexical tree
	// ------------------------------------------------------------------------ -
	// Benchmark                               Time             CPU   Iterations
	// ------------------------------------------------------------------------ -
	// BM_Contents_ParseContentStream     238547 ns       167392 ns         2987

	auto found = m_dictionary.find(set);
	if (found == m_dictionary.end()) {
		return Token::Type::UNKNOWN;
	}
	
	return found->second;
}

void ParserTokenDictionary::Initialize() {
	if (m_initialized) {
		return;
	}

	INSERT_TO_DICTIONARY(STREAM_BEGIN);
	INSERT_TO_DICTIONARY(STREAM_END);
	INSERT_TO_DICTIONARY(INDIRECT_OBJECT_BEGIN);
	INSERT_TO_DICTIONARY(INDIRECT_OBJECT_END);
	INSERT_TO_DICTIONARY(XREF_MARKER);
	INSERT_TO_DICTIONARY(START_XREF);
	INSERT_TO_DICTIONARY(TRAILER);
	INSERT_TO_DICTIONARY(NULL_OBJECT);
	INSERT_TO_DICTIONARY(TRUE_VALUE);
	INSERT_TO_DICTIONARY(FALSE_VALUE);
	INSERT_TO_DICTIONARY(INDIRECT_REFERENCE_MARKER);

	m_initialized = true;
}

void ReverseParserTokenDictionary::Initialize() {
	if (m_initialized) {
		return;
	}

	INSERT_TO_DICTIONARY(REVERSE_END_OF_FILE_MARKER);
	INSERT_TO_DICTIONARY(REVERSE_START_XREF);

	m_initialized = true;
}

void CharacterMapTokenDictionary::Initialize() {
	if (m_initialized) {
		return;
	}

	INSERT_TO_DICTIONARY(BLOCK_BEGIN);
	INSERT_TO_DICTIONARY(BLOCK_END);
	INSERT_TO_DICTIONARY(FIND_RESOURCE);
	INSERT_TO_DICTIONARY(DICTIONARY);
	INSERT_TO_DICTIONARY(BEGIN_CMAP);
	INSERT_TO_DICTIONARY(END_CMAP);
	INSERT_TO_DICTIONARY(BEGIN_CODE_SPACE_RANGE);
	INSERT_TO_DICTIONARY(END_CODE_SPACE_RANGE);
	INSERT_TO_DICTIONARY(BEGIN_BASE_FONT_RANGE);
	INSERT_TO_DICTIONARY(END_BASE_FONT_RANGE);
	INSERT_TO_DICTIONARY(BEGIN_BASE_FONT_CHAR);
	INSERT_TO_DICTIONARY(END_BASE_FONT_CHAR);
	INSERT_TO_DICTIONARY(BEGIN_NOT_DEF_RANGE);
	INSERT_TO_DICTIONARY(END_NOT_DEF_RANGE);
	INSERT_TO_DICTIONARY(BEGIN_NOT_DEF_CHAR);
	INSERT_TO_DICTIONARY(END_NOT_DEF_CHAR);
	INSERT_TO_DICTIONARY(BEGIN_CID_RANGE);
	INSERT_TO_DICTIONARY(END_CID_RANGE);
	INSERT_TO_DICTIONARY(BEGIN_CID_CHAR);
	INSERT_TO_DICTIONARY(END_CID_CHAR);
	INSERT_TO_DICTIONARY(DEFINITION);
	INSERT_TO_DICTIONARY(CMAP_NAME);
	INSERT_TO_DICTIONARY(CURRENT_DICTIONARY);
	INSERT_TO_DICTIONARY(DEFINE_RESOURCE);
	INSERT_TO_DICTIONARY(STACK_POP);

	ParserTokenDictionary::Initialize();
}

void ContentStreamTokenDictionary::Initialize() {
	if (m_initialized) {
		return;
	}

	// content streams
	INSERT_TO_DICTIONARY(LINE_WIDTH);
	INSERT_TO_DICTIONARY(LINE_CAP);
	INSERT_TO_DICTIONARY(LINE_JOIN);
	INSERT_TO_DICTIONARY(MITER_LIMIT);
	INSERT_TO_DICTIONARY(DASH_PATTERN);
	INSERT_TO_DICTIONARY(COLOR_RENDERING_INTENT);
	INSERT_TO_DICTIONARY(FLATNESS);
	INSERT_TO_DICTIONARY(GRAPHICS_STATE);
	INSERT_TO_DICTIONARY(SAVE_GRAPHICS_STATE);
	INSERT_TO_DICTIONARY(RESTORE_GRAPHIC_STATE);
	INSERT_TO_DICTIONARY(TRANSFORMATION_MATRIX);
	INSERT_TO_DICTIONARY(BEGIN_SUBPATH);
	INSERT_TO_DICTIONARY(LINE);
	INSERT_TO_DICTIONARY(FULL_CURVE);
	INSERT_TO_DICTIONARY(FINAL_CURVE);
	INSERT_TO_DICTIONARY(INITIAL_CURVE);
	INSERT_TO_DICTIONARY(CLOSE_SUBPATH);
	INSERT_TO_DICTIONARY(RECTANGLE);
	INSERT_TO_DICTIONARY(STROKE);
	INSERT_TO_DICTIONARY(CLOSE_AND_STROKE);
	INSERT_TO_DICTIONARY(FILL_PATH_NONZERO);
	INSERT_TO_DICTIONARY(FILL_PATH_COMPATIBILITY);
	INSERT_TO_DICTIONARY(FILL_PATH_EVEN_ODD);
	INSERT_TO_DICTIONARY(FILL_STROKE_NONZERO);
	INSERT_TO_DICTIONARY(FILL_STROKE_EVEN_ODD);
	INSERT_TO_DICTIONARY(CLOSE_FILL_STROKE_NONZERO);
	INSERT_TO_DICTIONARY(CLOSE_FILL_STROKE_EVEN_ODD);
	INSERT_TO_DICTIONARY(END_PATH);
	INSERT_TO_DICTIONARY(CLIP_PATH_NONZERO);
	INSERT_TO_DICTIONARY(CLIP_PATH_EVEN_ODD);
	INSERT_TO_DICTIONARY(BEGIN_TEXT);
	INSERT_TO_DICTIONARY(END_TEXT);
	INSERT_TO_DICTIONARY(CHARACTER_SPACING);
	INSERT_TO_DICTIONARY(WORD_SPACING);
	INSERT_TO_DICTIONARY(HORIZONTAL_SCALING);
	INSERT_TO_DICTIONARY(LEADING);
	INSERT_TO_DICTIONARY(TEXT_FONT);
	INSERT_TO_DICTIONARY(TEXT_RENDERING_MODE);
	INSERT_TO_DICTIONARY(TEXT_RISE);
	INSERT_TO_DICTIONARY(TEXT_TRANSLATE);
	INSERT_TO_DICTIONARY(TEXT_TRANSLATE_LEADING);
	INSERT_TO_DICTIONARY(TEXT_MATRIX);
	INSERT_TO_DICTIONARY(TEXT_NEXT_LINE);
	INSERT_TO_DICTIONARY(TEXT_SHOW);
	INSERT_TO_DICTIONARY(TEXT_SHOW_ARRAY);
	INSERT_TO_DICTIONARY(TEXT_NEXT_LINE_SHOW);
	INSERT_TO_DICTIONARY(TEXT_NEXT_LINE_SHOW_SPACING);
	INSERT_TO_DICTIONARY(SET_CHAR_WIDTH);
	INSERT_TO_DICTIONARY(SET_CACHE_DEVICE);
	INSERT_TO_DICTIONARY(SET_STROKING_COLOR_SPACE_NAME);
	INSERT_TO_DICTIONARY(SET_NONSTROKING_COLOR_SPACE_NAME);
	INSERT_TO_DICTIONARY(SET_STROKING_COLOR_SPACE_DEVICE);
	INSERT_TO_DICTIONARY(SET_STROKING_COLOR_SPACE_DEVICE_EXTENDED);
	INSERT_TO_DICTIONARY(SET_NONSTROKING_COLOR_SPACE_DEVICE);
	INSERT_TO_DICTIONARY(SET_NONSTROKING_COLOR_SPACE_DEVICE_EXTENDED);
	INSERT_TO_DICTIONARY(SET_STROKING_COLOR_SPACE_GRAY);
	INSERT_TO_DICTIONARY(SET_NONSTROKING_COLOR_SPACE_GRAY);
	INSERT_TO_DICTIONARY(SET_STROKING_COLOR_SPACE_RGB);
	INSERT_TO_DICTIONARY(SET_NONSTROKING_COLOR_SPACE_RGB);
	INSERT_TO_DICTIONARY(SET_STROKING_COLOR_SPACE_CMYK);
	INSERT_TO_DICTIONARY(SET_NONSTROKING_COLOR_SPACE_CMYK);
	INSERT_TO_DICTIONARY(SHADING_PAINT);
	INSERT_TO_DICTIONARY(BEGIN_INLINE_IMAGE_OBJECT);
	INSERT_TO_DICTIONARY(BEGIN_INLINE_IMAGE_DATA);
	INSERT_TO_DICTIONARY(END_INLINE_IMAGE_OBJECT);
	INSERT_TO_DICTIONARY(INVOKE_X_OBJECT);
	INSERT_TO_DICTIONARY(DEFINE_MARKED_CONTENT_POINT);
	INSERT_TO_DICTIONARY(DEFINE_MARKED_CONTENT_POINT_WITH_PROPERTY_LIST);
	INSERT_TO_DICTIONARY(BEGIN_MARKED_CONTENT_SEQUENCE);
	INSERT_TO_DICTIONARY(BEGIN_MARKED_CONTENT_SEQUENCE_WITH_PROPERTY_LIST);
	INSERT_TO_DICTIONARY(END_MARKED_CONTENT_SEQUENCE);
	INSERT_TO_DICTIONARY(BEGIN_COMPATIBILITY_SECTION);
	INSERT_TO_DICTIONARY(END_COMPATIBILITY_SECTION);

	ParserTokenDictionary::Initialize();
}

} // syntax
} // vanillapdf
