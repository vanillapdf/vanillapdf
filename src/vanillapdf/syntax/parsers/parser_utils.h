#ifndef _PARSER_UTILS_H
#define _PARSER_UTILS_H

#include "utils/constants.h"

#include "syntax/utils/syntax_fwd.h"

namespace vanillapdf {
namespace syntax {

class ParserUtils {
public:
    static BooleanObjectPtr CreateBoolean(TokenPtr token);
    static types::big_int GetIntegerValue(TokenPtr token);
    static IntegerObjectPtr CreateInteger(TokenPtr token);
    static RealObjectPtr CreateReal(TokenPtr token);
    static NameObjectPtr CreateName(TokenPtr token);
    static HexadecimalStringObjectPtr CreateHexString(TokenPtr token);
    static LiteralStringObjectPtr CreateLitString(TokenPtr token);
};

} // syntax
} // vanillapdf

#endif /* _PARSER_UTILS_H */
