#include "precompiled.h"
#include "token_dictionary.h"
#include "exception.h"

#include <cassert>
#include <algorithm>

namespace gotchangpdf
{
	namespace syntax
	{
		using namespace std;

		bool TokenDictionary::_initialized = false;

		//std::map<CharacterSet, Token::Type> TokenDictionary::_dictionary;
		Tree TokenDictionary::_dictionary;

		static const char STREAM_BEGIN[] = "stream";
		static const char STREAM_END[] = "endstream";
		static const char INDIRECT_OBJECT_BEGIN[] = "obj";
		static const char INDIRECT_OBJECT_END[] = "endobj";
		static const char XREF_MARKER[] = "xref";
		static const char START_XREF[] = "startxref";
		static const char END_OF_FILE_MARKER[] = "%%EOF";
		static const char TRAILER[] = "trailer";
		static const char NULL_OBJECT[] = "null";
		static const char TRUE_VALUE[] = "true";
		static const char FALSE_VALUE[] = "false";

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
		static const char COLOR_SPACE_STROKE[] = "CS";
		static const char COLOR_SPACE_NONSTROKE[] = "cs";
		static const char SET_COLOR_STROKE[] = "SC";
		static const char SET_COLOR_STROKE_EXTENDED[] = "SCN";
		static const char SET_COLOR_NONSTROKE[] = "sc";
		static const char SET_COLOR_NONSTROKE_EXTENDED[] = "scn";
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

		Token::Type TokenDictionary::Find(BufferPtr set)
		{
			Initialize();
			return _dictionary.TokenType(set);
		}

#define INSERT_TO_DICTIONARY(name) _dictionary.Insert(BufferPtr(name, sizeof(name)), Token::Type::name)

		void TokenDictionary::Initialize()
		{
			if (_initialized)
				return;

			INSERT_TO_DICTIONARY(STREAM_BEGIN);
			INSERT_TO_DICTIONARY(STREAM_END);
			INSERT_TO_DICTIONARY(INDIRECT_OBJECT_BEGIN);
			INSERT_TO_DICTIONARY(INDIRECT_OBJECT_END);
			INSERT_TO_DICTIONARY(XREF_MARKER);
			INSERT_TO_DICTIONARY(START_XREF);
			INSERT_TO_DICTIONARY(END_OF_FILE_MARKER);
			INSERT_TO_DICTIONARY(TRAILER);
			INSERT_TO_DICTIONARY(NULL_OBJECT);
			INSERT_TO_DICTIONARY(TRUE_VALUE);
			INSERT_TO_DICTIONARY(FALSE_VALUE);

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
			INSERT_TO_DICTIONARY(COLOR_SPACE_STROKE);
			INSERT_TO_DICTIONARY(COLOR_SPACE_NONSTROKE);
			INSERT_TO_DICTIONARY(SET_COLOR_STROKE);
			INSERT_TO_DICTIONARY(SET_COLOR_STROKE_EXTENDED);
			INSERT_TO_DICTIONARY(SET_COLOR_NONSTROKE);
			INSERT_TO_DICTIONARY(SET_COLOR_NONSTROKE_EXTENDED);
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

			_initialized = true;
		}
	}
}
