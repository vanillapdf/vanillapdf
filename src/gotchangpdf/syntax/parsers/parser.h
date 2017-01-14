#ifndef _PARSER_H
#define _PARSER_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/parsers/parser_interface.h"
#include "syntax/parsers/tokenizer.h"

#include "syntax/files/xref.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class ParserBase : public Tokenizer
		{
		public:
			ParserBase(std::weak_ptr<File> file, CharacterSource & stream);

			ObjectPtr ReadDirectObject(void);
			ObjectPtr ReadDirectObject(types::stream_offset offset);
			ObjectPtr PeekDirectObject(void);

			template<typename T>
			T ReadDirectObjectWithType(types::stream_offset offset)
			{
				auto direct = ReadDirectObject(offset);
				return ObjectUtils::ConvertTo<T>(direct);
			}

			template<typename T>
			T ReadDirectObjectWithType(void)
			{
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

			std::weak_ptr<File> GetFile(void) const;

		protected:
			std::weak_ptr<File> _file;
		};

		class Parser : public ParserBase, public IParser
		{
		public:
			Parser(std::weak_ptr<File> file, CharacterSource & stream);

			virtual XrefBasePtr ReadXref(void) override;
			virtual XrefBasePtr ReadXref(types::stream_offset offset) override;

			ObjectPtr ReadIndirectObject(types::big_uint& obj_number, types::ushort& gen_number);
			ObjectPtr ReadIndirectObject(types::stream_offset offset, types::big_uint& obj_number, types::ushort& gen_number);

			virtual ObjectStreamEntries ReadObjectStreamEntries(types::big_uint first, size_t size) override;

			HeaderPtr ReadHeader(types::stream_offset offset);
			HeaderPtr ReadHeader(void);

			XrefChainPtr FindAllObjects(void);

		private:
			ObjectStreamEntries ReadObjectStreamHeaders(size_t size);
			ObjectStreamEntry ReadObjectStreamHeader();
			XrefEntryBasePtr ReadTableEntry(types::big_uint objNumber);
			XrefTablePtr ReadXrefTable();
			XrefStreamPtr ReadXrefStream();
			XrefStreamPtr ParseXrefStream(
				StreamObjectPtr stream,
				types::big_uint stream_obj_number,
				types::ushort stream_gen_number);
		};

		class CharacterMapParser : public ParserBase, public ICharacterMapParser
		{
		public:
			CharacterMapParser(std::weak_ptr<File> file, CharacterSource & stream);
			virtual CharacterMapData ReadCharacterMapData(void) override;
		};
	}
}

#endif /* _PARSER_H */
