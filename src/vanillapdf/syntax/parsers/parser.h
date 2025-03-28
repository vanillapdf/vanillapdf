#ifndef _PARSER_H
#define _PARSER_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/parsers/parser_interface.h"
#include "syntax/parsers/tokenizer.h"

#include "syntax/files/xref.h"

namespace vanillapdf {
namespace syntax {

class ParserBase : public Tokenizer {
public:
    ParserBase(WeakReference<File> file, IInputStreamPtr stream);

    virtual ObjectPtr ReadDirectObject(void);
    virtual ObjectPtr ReadDirectObject(types::stream_offset offset);
    virtual ObjectPtr PeekDirectObject(void);

    template<typename T>
    T ReadDirectObjectWithType(types::stream_offset offset) {
        auto direct = ReadDirectObject(offset);
        return ObjectUtils::ConvertTo<T>(direct);
    }

    template<typename T>
    T ReadDirectObjectWithType(void) {
        auto direct = ReadDirectObject();
        return ObjectUtils::ConvertTo<T>(direct);
    }

    BooleanObjectPtr ReadFalse();
    BooleanObjectPtr ReadTrue();
    IntegerObjectPtr ReadInteger();
    ObjectPtr ReadIndirectReference();
    RealObjectPtr ReadReal();
    NullObjectPtr ReadNull();
    DictionaryObjectPtr ReadDictionary();
    ObjectPtr ReadDictionaryStream();
    MixedArrayObjectPtr ReadArray();
    NameObjectPtr ReadName();
    LiteralStringObjectPtr ReadLiteralString();
    HexadecimalStringObjectPtr ReadHexadecimalString();

    TokenPtr ReadTokenSkip();
    TokenPtr PeekTokenSkip();
    Token::Type PeekTokenTypeSkip();
    TokenPtr ReadTokenWithTypeSkip(Token::Type type);

    WeakReference<File> GetFile(void) const;

protected:
    WeakReference<File> _file;
};

class Parser : public ParserBase, public IParser {
public:
    Parser(WeakReference<File> file, IInputStreamPtr stream);

    virtual XrefBasePtr ReadXref(void) override;
    virtual XrefBasePtr ReadXref(types::stream_offset offset) override;

    ObjectPtr ReadIndirectObject(types::big_uint& obj_number, types::ushort& gen_number);
    ObjectPtr ReadIndirectObject(types::stream_offset offset, types::big_uint& obj_number, types::ushort& gen_number);

    virtual ObjectStreamEntries ReadObjectStreamEntries(types::big_uint first, types::size_type size) override;

    HeaderPtr ReadHeader(types::stream_offset offset);
    HeaderPtr ReadHeader(void);

    XrefChainPtr FindAllObjects(void);

private:
    ObjectStreamEntries ReadObjectStreamHeaders(types::size_type size);
    ObjectStreamEntry ReadObjectStreamHeader();
    XrefEntryBasePtr ReadTableEntry(types::big_uint objNumber);
    XrefTablePtr ReadXrefTable();
    XrefStreamPtr ReadXrefStream();
    XrefStreamPtr ParseXrefStream(
        StreamObjectPtr stream,
        types::big_uint stream_obj_number,
        types::ushort stream_gen_number);
};

} // syntax
} // vanillapdf

#endif /* _PARSER_H */
