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

			ObjectPtr ReadIndirectObject(void);
			ObjectPtr ReadIndirectObject(types::stream_offset offset);

			virtual ObjectPtr ReadDirectObject(void) override;
			virtual ObjectPtr ReadDirectObject(types::stream_offset offset) override;
			ObjectPtr PeekDirectObject(void);

			virtual std::vector<ObjectPtr> ReadObjectStreamEntries(types::integer first, types::integer size) override;
			virtual std::vector<contents::OperationGenericPtr> ReadContentStreamOperations(void) override;

		private:
			std::weak_ptr<File> _file;

			TokenPtr ReadTokenSkip();
			TokenPtr PeekTokenSkip();
			Token::Type PeekTokenTypeSkip();
			TokenPtr ReadTokenWithTypeSkip(Token::Type type);

			ObjectStreamHeaders ReadObjectStreamHeaders(types::integer size);
			ObjectStreamHeader ReadObjectStreamHeader();
			XrefEntryBasePtr ReadTableEntry(types::integer objNumber);
			XrefTablePtr ReadXrefTable();
			XrefStreamPtr ReadXrefStream();

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

			contents::OperationGenericPtr ReadContentStreamOperation(void);
			bool IsOperand(Token::Type type);
			ObjectPtr ReadOperand();
			contents::OperatorBasePtr ReadOperator();
		};
	}
}

#endif /* _PARSER_H */
