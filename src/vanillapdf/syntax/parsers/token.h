#ifndef _TOKEN_H
#define _TOKEN_H

#include "syntax/utils/syntax_fwd.h"

#include "utils/buffer.h"

namespace vanillapdf {
namespace syntax {

class Token : public IUnknown {
public:
    enum class Type : unsigned char {
        UNKNOWN = 0,
        EOL,
        DICTIONARY_BEGIN,
        DICTIONARY_END,
        ARRAY_BEGIN,
        ARRAY_END,
        NAME_OBJECT,
        HEXADECIMAL_STRING,
        LITERAL_STRING,
        INTEGER_OBJECT,
        REAL_OBJECT,
        INDIRECT_REFERENCE_MARKER,
        STREAM_BEGIN,
        STREAM_END,
        INDIRECT_OBJECT_BEGIN,
        INDIRECT_OBJECT_END,
        XREF_MARKER,
        START_XREF,
        TRAILER,
        NULL_OBJECT,
        TRUE_VALUE,
        FALSE_VALUE,
        END_OF_INPUT,

        // reverse tokens
        REVERSE_EOL,
        REVERSE_INTEGER_OBJECT,
        REVERSE_END_OF_FILE_MARKER,
        REVERSE_START_XREF,

        // character map
        BLOCK_BEGIN,
        BLOCK_END,
        FIND_RESOURCE,
        DICTIONARY,
        BEGIN_CMAP,
        END_CMAP,
        BEGIN_CODE_SPACE_RANGE,
        END_CODE_SPACE_RANGE,
        BEGIN_BASE_FONT_RANGE,
        END_BASE_FONT_RANGE,
        BEGIN_BASE_FONT_CHAR,
        END_BASE_FONT_CHAR,
        BEGIN_NOT_DEF_RANGE,
        END_NOT_DEF_RANGE,
        BEGIN_NOT_DEF_CHAR,
        END_NOT_DEF_CHAR,
        BEGIN_CID_RANGE,
        END_CID_RANGE,
        BEGIN_CID_CHAR,
        END_CID_CHAR,
        DEFINITION,
        CMAP_NAME,
        CURRENT_DICTIONARY,
        DEFINE_RESOURCE,
        STACK_POP,

        // content streams
        LINE_WIDTH,
        LINE_CAP,
        LINE_JOIN,
        MITER_LIMIT,
        DASH_PATTERN,
        COLOR_RENDERING_INTENT,
        FLATNESS,
        GRAPHICS_STATE,
        SAVE_GRAPHICS_STATE,
        RESTORE_GRAPHIC_STATE,
        TRANSFORMATION_MATRIX,
        BEGIN_SUBPATH,
        LINE,
        FULL_CURVE,
        FINAL_CURVE,
        INITIAL_CURVE,
        CLOSE_SUBPATH,
        RECTANGLE,
        STROKE,
        CLOSE_AND_STROKE,
        FILL_PATH_NONZERO,
        FILL_PATH_COMPATIBILITY,
        FILL_PATH_EVEN_ODD,
        FILL_STROKE_NONZERO,
        FILL_STROKE_EVEN_ODD,
        CLOSE_FILL_STROKE_NONZERO,
        CLOSE_FILL_STROKE_EVEN_ODD,
        END_PATH,
        CLIP_PATH_NONZERO,
        CLIP_PATH_EVEN_ODD,
        BEGIN_TEXT,
        END_TEXT,
        CHARACTER_SPACING,
        WORD_SPACING,
        HORIZONTAL_SCALING,
        LEADING,
        TEXT_FONT,
        TEXT_RENDERING_MODE,
        TEXT_RISE,
        TEXT_TRANSLATE,
        TEXT_TRANSLATE_LEADING,
        TEXT_MATRIX,
        TEXT_NEXT_LINE,
        TEXT_SHOW,
        TEXT_SHOW_ARRAY,
        TEXT_NEXT_LINE_SHOW,
        TEXT_NEXT_LINE_SHOW_SPACING,
        SET_CHAR_WIDTH,
        SET_CACHE_DEVICE,
        SET_STROKING_COLOR_SPACE_NAME,
        SET_NONSTROKING_COLOR_SPACE_NAME,
        SET_STROKING_COLOR_SPACE_DEVICE,
        SET_STROKING_COLOR_SPACE_DEVICE_EXTENDED,
        SET_NONSTROKING_COLOR_SPACE_DEVICE,
        SET_NONSTROKING_COLOR_SPACE_DEVICE_EXTENDED,
        SET_STROKING_COLOR_SPACE_GRAY,
        SET_NONSTROKING_COLOR_SPACE_GRAY,
        SET_STROKING_COLOR_SPACE_RGB,
        SET_NONSTROKING_COLOR_SPACE_RGB,
        SET_STROKING_COLOR_SPACE_CMYK,
        SET_NONSTROKING_COLOR_SPACE_CMYK,
        SHADING_PAINT,
        BEGIN_INLINE_IMAGE_OBJECT,
        BEGIN_INLINE_IMAGE_DATA,
        END_INLINE_IMAGE_OBJECT,
        INVOKE_X_OBJECT,
        DEFINE_MARKED_CONTENT_POINT,
        DEFINE_MARKED_CONTENT_POINT_WITH_PROPERTY_LIST,
        BEGIN_MARKED_CONTENT_SEQUENCE,
        BEGIN_MARKED_CONTENT_SEQUENCE_WITH_PROPERTY_LIST,
        END_MARKED_CONTENT_SEQUENCE,
        BEGIN_COMPATIBILITY_SECTION,
        END_COMPATIBILITY_SECTION,
    };

    Token() = default;
    explicit Token(Type type);
    Token(Type type, std::string_view chars);

    Token::Type GetType(void) const { return _type; }
    std::string Value() const { return _value; }

private:
    Type _type = Type::UNKNOWN;
    std::string _value;
};

} // syntax
} // vanillapdf

#endif /* _TOKEN_H */
