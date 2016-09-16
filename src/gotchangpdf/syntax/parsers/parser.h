#ifndef _PARSER_H
#define _PARSER_H

#include "syntax_fwd.h"
#include "parser_interface.h"
#include "xref.h"
#include "tokenizer.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class Parser : public Tokenizer, public IParser
		{
		public:
			Parser(std::weak_ptr<File> file, CharacterSource & stream);
			Parser(const Parser & other);
			std::weak_ptr<File> GetFile(void) const;

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

			virtual XrefBasePtr ReadXref(void) override;
			virtual XrefBasePtr ReadXref(types::stream_offset offset) override;

			ObjectPtr ReadIndirectObject(types::big_uint& obj_number, types::ushort& gen_number);
			ObjectPtr ReadIndirectObject(types::stream_offset offset, types::big_uint& obj_number, types::ushort& gen_number);

			virtual ObjectPtr ReadDirectObject(void) override;
			virtual ObjectPtr ReadDirectObject(types::stream_offset offset) override;
			ObjectPtr PeekDirectObject(void);

			virtual ObjectStreamEntries ReadObjectStreamEntries(types::big_uint first, size_t size) override;
			virtual contents::BaseInstructionCollectionPtr ReadContentStreamInstructions(void) override;

			HeaderPtr ReadHeader(types::stream_offset offset);
			HeaderPtr ReadHeader(void);

			XrefChainPtr FindAllObjects(void);

		private:
			std::weak_ptr<File> _file;

			TokenPtr ReadTokenSkip();
			TokenPtr PeekTokenSkip();
			Token::Type PeekTokenTypeSkip();
			TokenPtr ReadTokenWithTypeSkip(Token::Type type);

			ObjectStreamEntries ReadObjectStreamHeaders(size_t size);
			ObjectStreamEntry ReadObjectStreamHeader();
			XrefEntryBasePtr ReadTableEntry(types::big_uint objNumber);
			XrefTablePtr ReadXrefTable();
			XrefStreamPtr ReadXrefStream();
			XrefStreamPtr ParseXrefStream(
				StreamObjectPtr stream,
				types::big_uint stream_obj_number,
				types::ushort stream_gen_number);

			BooleanObjectPtr ReadFalse();
			BooleanObjectPtr ReadTrue();
			IntegerObjectPtr ReadInteger();
			ObjectPtr ReadIntegerReference();
			RealObjectPtr ReadReal();
			NullObjectPtr ReadNull();
			DictionaryObjectPtr ReadDictionary();
			ObjectPtr ReadDictionaryStream();
			MixedArrayObjectPtr ReadArray();
			NameObjectPtr ReadName();
			LiteralStringObjectPtr ReadLiteralString();
			HexadecimalStringObjectPtr ReadHexadecimalString();

			contents::InstructionBasePtr ReadContentStreamInstruction(void);
			contents::OperationBasePtr ReadContentStreamOperation(void);
			bool IsOperand(Token::Type type);
			ObjectPtr ReadOperand();
			contents::OperationBasePtr ReadOperatorReturnOperation(const std::vector<ObjectPtr>& operands);
			contents::InlineImageObjectPtr ReadInlineImageObject(void);
		};
	}
}

#endif /* _PARSER_H */
