#include "precompiled.h"
#include "parser.h"
#include "exception.h"
#include "token.h"
#include "file.h"
#include "character.h"

#include "content_stream_operation_generic.h"
#include "content_stream_operators.h"

#include <regex>

namespace gotchangpdf
{
	namespace syntax
	{
#pragma region Objects

		Parser::Parser(std::weak_ptr<File> file, CharacterSource & stream)
			: Tokenizer(stream), _file(file) {}

		Parser::Parser(const Parser & other)
			: Tokenizer(other), _file(other._file) {}

		std::weak_ptr<File> Parser::GetFile(void) const { return _file; }

		class ObjectFactory
		{
		public:
			static BooleanObjectPtr CreateBoolean(TokenPtr token)
			{
				if (token->GetType() == Token::Type::TRUE_VALUE)
					return BooleanObjectPtr(true);

				if (token->GetType() == Token::Type::FALSE_VALUE)
					return BooleanObjectPtr(false);

				assert(!"Expected boolean token type");
				throw GeneralException("Expected boolean token type");
			}

			static IntegerObjectPtr CreateInteger(TokenPtr token)
			{
				assert(token->GetType() == Token::Type::INTEGER_OBJECT && "Expected integer token type");

				auto buffer = token->Value();
				auto value = std::stoll(buffer->ToString());
				return IntegerObjectPtr(value);
			}

			static RealObjectPtr CreateReal(TokenPtr token)
			{
				assert(token->GetType() == Token::Type::REAL_OBJECT && "Expected real token type");

				auto buffer = token->Value();
				auto str = buffer->ToString();
				auto value = std::stod(str);
				auto pos = str.rfind('.');
				if (-1 != pos) {
					auto precision = str.size() - pos - 1;
					auto converted = SafeConvert<uint32_t>(precision);
					return RealObjectPtr(value, converted);
				}

				return RealObjectPtr(value);
			}

			static NameObjectPtr CreateName(TokenPtr token)
			{
				assert(token->GetType() == Token::Type::NAME_OBJECT && "Expected name token type");

				auto buffer = token->Value();
				return NameObjectPtr(buffer);
			}

			static HexadecimalStringObjectPtr CreateHexString(TokenPtr token)
			{
				assert(token->GetType() == Token::Type::HEXADECIMAL_STRING && "Expected hexadecimal string token type");

				auto buffer = token->Value();
				return HexadecimalStringObjectPtr(buffer);
			}

			static LiteralStringObjectPtr CreateLitString(TokenPtr token)
			{
				assert(token->GetType() == Token::Type::LITERAL_STRING && "Expected literal string token type");

				auto buffer = token->Value();
				return LiteralStringObjectPtr(buffer);
			}
		};

		IntegerObjectPtr Parser::ReadInteger()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto result = ObjectFactory::CreateInteger(token);
			result->SetFile(_file);
			return result;
		}

		ObjectPtr Parser::ReadIntegerReference()
		{
			auto integer = ReadInteger();

			auto pos = tellg();
			if (PeekTokenTypeSkip() == Token::Type::INTEGER_OBJECT) {
				auto ahead = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
				auto gen_number = ObjectFactory::CreateInteger(ahead);

				if (PeekTokenTypeSkip() == Token::Type::INDIRECT_REFERENCE_MARKER) {
					auto reference_marker = ReadTokenWithTypeSkip(Token::Type::INDIRECT_REFERENCE_MARKER);
					IndirectObjectReferencePtr result(integer->Value(), gen_number->SafeConvert<types::ushort>());
					result->SetFile(_file);
					return result;
				}

				// TODO we can peek only one next token, therefore we need to seek back
				seekg(pos);
			}

			return integer;
		}

		RealObjectPtr Parser::ReadReal()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::REAL_OBJECT);
			auto result = ObjectFactory::CreateReal(token);
			result->SetFile(_file);
			return result;
		}

		NullObjectPtr Parser::ReadNull()
		{
			ReadTokenWithTypeSkip(Token::Type::NULL_OBJECT);
			auto result = NullObject::GetInstance();
			result->SetFile(_file);
			return result;
		}

		DictionaryObjectPtr Parser::ReadDictionary()
		{
			DictionaryObjectPtr dictionary;
			ReadTokenWithTypeSkip(Token::Type::DICTIONARY_BEGIN);
			while (PeekTokenTypeSkip() != Token::Type::DICTIONARY_END)
			{
				auto name = ReadDirectObjectWithType<NameObjectPtr>();
				auto val = ReadDirectObject();

				if (val->GetType() == Object::Type::Null)
					continue;

				auto containable_ptr = dynamic_cast<ContainableObject*>(val.get());
				if (nullptr == containable_ptr)
					throw ConversionExceptionFactory<ContainableObject>::Construct(val);

				dictionary->_list[name] = ContainableObjectPtr(containable_ptr);
			}

			ReadTokenWithTypeSkip(Token::Type::DICTIONARY_END);
			dictionary->SetFile(_file);
			return dictionary;
		}

		ObjectPtr Parser::ReadDictionaryStream()
		{
			auto dictionary = ReadDictionary();
			if (PeekTokenTypeSkip() == Token::Type::STREAM_BEGIN)
			{
				ReadTokenWithTypeSkip(Token::Type::STREAM_BEGIN);
				ReadTokenWithTypeSkip(Token::Type::EOL);
				auto stream_offset = tellg();
				auto length = dictionary->FindAs<IntegerObjectPtr>(constant::Name::Length);
				seekg(length->Value(), ios_base::cur);
				ReadTokenWithTypeSkip(Token::Type::STREAM_END);

				auto result = StreamObjectPtr(dictionary, stream_offset);
				result->SetFile(_file);
				result->GetHeader()->SetFile(_file);
				return result;
			}

			return dictionary;
		}

		MixedArrayObjectPtr Parser::ReadArray()
		{
			MixedArrayObjectPtr result;
			ReadTokenWithTypeSkip(Token::Type::ARRAY_BEGIN);
			while (PeekTokenTypeSkip() != Token::Type::ARRAY_END)
			{
				auto val = ReadDirectObject();
				auto containable_ptr = dynamic_cast<ContainableObject*>(val.get());
				if (nullptr == containable_ptr)
					throw ConversionExceptionFactory<ContainableObject>::Construct(val);

				result->push_back(containable_ptr);
			}

			ReadTokenWithTypeSkip(Token::Type::ARRAY_END);
			result->SetFile(_file);
			return result;
		}

		NameObjectPtr Parser::ReadName()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::NAME_OBJECT);
			auto result = ObjectFactory::CreateName(token);
			result->SetFile(_file);
			return result;
		}

		LiteralStringObjectPtr Parser::ReadLiteralString()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::LITERAL_STRING);
			auto result = ObjectFactory::CreateLitString(token);
			result->SetFile(_file);
			return result;
		}

		HexadecimalStringObjectPtr Parser::ReadHexadecimalString()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::HEXADECIMAL_STRING);
			auto result = ObjectFactory::CreateHexString(token);
			result->SetFile(_file);
			return result;
		}

		BooleanObjectPtr Parser::ReadTrue()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::TRUE_VALUE);
			auto result = ObjectFactory::CreateBoolean(token);
			result->SetFile(_file);
			return result;
		}

		BooleanObjectPtr Parser::ReadFalse()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::FALSE_VALUE);
			auto result = ObjectFactory::CreateBoolean(token);
			result->SetFile(_file);
			return result;
		}

		ObjectPtr Parser::ReadIndirectObject(void)
		{
			auto offset = tellg();
			auto obj_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto gen_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto begin_token = ReadTokenWithTypeSkip(Token::Type::INDIRECT_OBJECT_BEGIN);
			auto direct = ReadDirectObject();
			auto end_token = ReadTokenWithTypeSkip(Token::Type::INDIRECT_OBJECT_END);

			auto locked_file = _file.lock();
			if (!locked_file)
				throw FileDisposedException();

			auto obj_number = ObjectFactory::CreateInteger(obj_number_token);
			auto gen_number = ObjectFactory::CreateInteger(gen_number_token);
			direct->SetObjectNumber(obj_number->Value());
			direct->SetGenerationNumber(gen_number->SafeConvert<types::ushort>());
			direct->SetOffset(offset);
			direct->SetFile(_file);

			assert(direct->IsIndirect());
			return direct;
		}

		ObjectPtr Parser::ReadIndirectObject(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			return ReadIndirectObject();
		}

		ObjectPtr Parser::ReadDirectObject()
		{
			auto offset = tellg();
			switch (PeekTokenTypeSkip())
			{
			case Token::Type::DICTIONARY_BEGIN:
				return ReadDictionaryStream();
			case Token::Type::INTEGER_OBJECT:
				return ReadIntegerReference();
			case Token::Type::ARRAY_BEGIN:
				return ReadArray();
			case Token::Type::NAME_OBJECT:
				return ReadName();
			case Token::Type::HEXADECIMAL_STRING:
				return ReadHexadecimalString();
			case Token::Type::LITERAL_STRING:
				return ReadLiteralString();
			case Token::Type::REAL_OBJECT:
				return ReadReal();
			case Token::Type::TRUE_VALUE:
				return ReadTrue();
			case Token::Type::FALSE_VALUE:
				return ReadFalse();
			case Token::Type::NULL_OBJECT:
				return ReadNull();
			default:
				throw ParseException(offset);
			}
		}

		ObjectStreamHeader Parser::ReadObjectStreamHeader()
		{
			ObjectStreamHeader result;

			auto obj_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto offset_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

			auto obj_number = ObjectFactory::CreateInteger(obj_number_token);
			auto offset = ObjectFactory::CreateInteger(offset_token);

			result.object_number = obj_number->Value();
			result.offset = offset->Value();
			return result;
		}

		ObjectStreamHeaders Parser::ReadObjectStreamHeaders(size_t size)
		{
			ObjectStreamHeaders result;
			result.reserve(size);
			for (decltype(size) i = 0; i < size; ++i) {
				auto item = ReadObjectStreamHeader();
				result.push_back(item);
			}

			return result;
		}

		std::vector<ObjectPtr> Parser::ReadObjectStreamEntries(types::big_uint first, size_t size)
		{
			std::vector<ObjectPtr> result;
			auto headers = ReadObjectStreamHeaders(size);
			for (auto header : headers) {
				seekg(first + header.offset);
				auto obj = ReadDirectObject();
				obj->SetObjectNumber(header.object_number);

				result.push_back(obj);
			}

			return result;
		}

		ObjectPtr Parser::ReadDirectObject(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			return ReadDirectObject();
		}

		ObjectPtr Parser::PeekDirectObject()
		{
			auto position = tellg();
			auto obj = ReadDirectObject();
			seekg(position);

			return obj;
		}

#pragma endregion

#pragma region Tokens

		TokenPtr Parser::ReadTokenSkip()
		{
			for (;;) {
				auto token = ReadToken();
				if (token->GetType() == Token::Type::EOL)
					continue;

				return token;
			}
		}

		TokenPtr Parser::PeekTokenSkip()
		{
			auto position = tellg();
			bool rewind = false;
			for (;;) {
				auto token = PeekToken();
				if (token->GetType() == Token::Type::EOL) {
					ReadToken();
					rewind = true;
					continue;
				}

				if (rewind) {
					seekg(position);
				}

				return token;
			}
		}

		Token::Type Parser::PeekTokenTypeSkip()
		{
			auto token = PeekTokenSkip();
			return token->GetType();
		}

		TokenPtr Parser::ReadTokenWithTypeSkip(Token::Type type)
		{
			auto offset = tellg();
			for (;;) {
				auto token = ReadToken();

				if (token->GetType() == type)
					return token;

				if (token->GetType() == Token::Type::EOL)
					continue;

				throw ParseException(offset);
			}
		}

#pragma endregion

#pragma region Xref

		XrefEntryBasePtr Parser::ReadTableEntry(types::big_uint objNumber)
		{
			auto offset_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto generation_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

			auto offset = ObjectFactory::CreateInteger(offset_token);
			auto gen_number = ObjectFactory::CreateInteger(generation_token);

			auto peeked_token = PeekTokenSkip();
			if (*peeked_token->Value() == "n") {
				ReadTokenSkip();
				XrefUsedEntryPtr result(objNumber, gen_number->SafeConvert<types::ushort>(), offset->Value());
				result->SetFile(_file);
				return result;
			}

			if (*peeked_token->Value() == "f") {
				ReadTokenSkip();
				XrefFreeEntryPtr result(objNumber, gen_number->SafeConvert<types::ushort>(), offset->Value());
				result->SetFile(_file);
				return result;
			}

			throw ParseException(tellg());
		}

		XrefTablePtr Parser::ReadXrefTable()
		{
			XrefTablePtr table;

			ReadTokenWithTypeSkip(Token::Type::XREF_MARKER);
			while (PeekTokenTypeSkip() != Token::Type::TRAILER) {
				auto revision_base_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
				auto size_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

				auto revision_base = std::stoull(revision_base_token->Value()->ToString());
				auto size = std::stoull(size_token->Value()->ToString());

				for (decltype(size) i = 0; i < size; ++i) {
					auto entry = ReadTableEntry(SafeAddition<types::big_uint>(revision_base, i));
					table->Add(entry);
				}
			}

			ReadTokenWithTypeSkip(Token::Type::TRAILER);
			auto trailer_dictionary = ReadDirectObjectWithType<DictionaryObjectPtr>();

			table->SetTrailerDictionary(trailer_dictionary);
			table->SetFile(_file);
			return table;
		}

		XrefStreamPtr Parser::ReadXrefStream()
		{
			XrefStreamPtr result;

			auto indirect = ReadIndirectObject();
			auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(indirect);

			// Get stream object data
			auto header = stream->GetHeader();

			auto fields = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::W);
			auto size = header->FindAs<IntegerObjectPtr>(constant::Name::Size);

			assert(fields->Size() == 3);
			ArrayObjectPtr<IntegerObjectPtr> index = { IntegerObjectPtr(0), size };
			if (header->Contains(constant::Name::Index)) {
				index = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::Index);
			}

			auto index_size = index->Size();
			assert(index_size % 2 == 0);

			auto body = stream->GetBodyDecoded();

			auto field1_size = fields->At(0);
			auto field2_size = fields->At(1);
			auto field3_size = fields->At(2);

			// Iterate over entries
			auto it = body.begin();
			for (unsigned int i = 0; i < index_size; i += 2) {

				auto subsection_index = index->At(i);
				auto subsection_size = index->At(i + 1);

				for (auto idx = 0; idx < *subsection_size; idx++) {

					IntegerObject field1;
					for (int j = 0; j < *field1_size; ++j) {
						field1 = (field1 << 8) + (*it & 0xff);
						it++;
					}

					assert(field1 == 0 || field1 == 1 || field1 == 2);

					IntegerObject field2;
					for (int j = 0; j < *field2_size; ++j) {
						field2 = (field2 << 8) + (*it & 0xff);
						it++;
					}

					IntegerObject field3;
					for (int j = 0; j < *field3_size; ++j) {
						field3 = (field3 << 8) + (*it & 0xff);
						it++;
					}

					switch (field1) {
					case 0:
					{
						XrefFreeEntryPtr entry(*subsection_index + idx, field3.SafeConvert<types::ushort>(), field2);
						entry->SetFile(_file);
						result->Add(entry);
						break;
					}
					case 1:
					{
						XrefUsedEntryPtr entry(*subsection_index + idx, field3.SafeConvert<types::ushort>(), field2);
						entry->SetFile(_file);
						result->Add(entry);
						break;
					}
					case 2:
					{
						XrefCompressedEntryPtr entry(*subsection_index + idx, static_cast<types::ushort>(0), field2, field3.SafeConvert<size_t>());
						entry->SetFile(_file);
						result->Add(entry);
						break;
					}
					default:
						LOG_ERROR(_file) << "Unrecognized data found in xref stream data";
						break;
					}
				}
			}

			result->SetFile(_file);
			result->SetTrailerDictionary(header);
			return result;
		}

		XrefBasePtr Parser::ReadXref(void)
		{
			if (PeekTokenTypeSkip() == Token::Type::XREF_MARKER) {
				return ReadXrefTable();
			}

			return ReadXrefStream();
		}

		XrefBasePtr Parser::ReadXref(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			return ReadXref();
		}

#pragma endregion


#pragma region Content streams

		contents::GenericOperationCollection Parser::ReadContentStreamOperations(void)
		{
			contents::GenericOperationCollection result;
			while (PeekTokenTypeSkip() != Token::Type::END_OF_INPUT) {
				auto operation = ReadContentStreamOperation();
				result.push_back(operation);
			}

			return result;
		}

		contents::OperationGenericPtr Parser::ReadContentStreamOperation(void)
		{
			std::vector<ObjectPtr> operands;
			while (IsOperand(PeekTokenTypeSkip())) {
				auto operand = ReadOperand();
				operands.push_back(operand);
			}

			auto oper = ReadOperator();
			return contents::OperationGenericPtr(operands, oper);
		}

		contents::OperatorBasePtr Parser::ReadOperator()
		{
			auto token = ReadTokenSkip();
			switch (token->GetType())
			{
			case Token::Type::LINE_WIDTH:
				return contents::LineWidthOperatorPtr();
			case Token::Type::LINE_CAP:
				return contents::LineCapOperatorPtr();
			case Token::Type::LINE_JOIN:
				return contents::LineJoinOperatorPtr();
			case Token::Type::MITER_LIMIT:
				return contents::MiterLimitOperatorPtr();
			case Token::Type::DASH_PATTERN:
				return contents::DashPatternOperatorPtr();
			case Token::Type::COLOR_RENDERING_INTENT:
				return contents::ColorRenderingIntentOperatorPtr();
			case Token::Type::FLATNESS:
				return contents::FlatnessOperatorPtr();
			case Token::Type::GRAPHICS_STATE:
				return contents::GraphicsStateOperatorPtr();
			case Token::Type::SAVE_GRAPHICS_STATE:
				return contents::SaveGraphicsStateOperatorPtr();
			case Token::Type::RESTORE_GRAPHIC_STATE:
				return contents::RestoreGraphicsStateOperatorPtr();
			case Token::Type::TRANSFORMATION_MATRIX:
				return contents::TransformationMatrixOperatorPtr();
			case Token::Type::BEGIN_SUBPATH:
				return contents::BeginSubpathOperatorPtr();
			case Token::Type::LINE:
				return contents::LineOperatorPtr();
			case Token::Type::FULL_CURVE:
				return contents::FullCurveOperatorPtr();
			case Token::Type::FINAL_CURVE:
				return contents::FinalCurveOperatorPtr();
			case Token::Type::INITIAL_CURVE:
				return contents::InitialCurveOperatorPtr();
			case Token::Type::CLOSE_SUBPATH:
				return contents::CloseSubpathOperatorPtr();
			case Token::Type::RECTANGLE:
				return contents::RectangleOperatorPtr();
			case Token::Type::STROKE:
				return contents::StrokeOperatorPtr();
			case Token::Type::CLOSE_AND_STROKE:
				return contents::CloseAndStrokeOperatorPtr();
			case Token::Type::FILL_PATH_NONZERO:
				return contents::FillPathNonzeroOperatorPtr();
			case Token::Type::FILL_PATH_COMPATIBILITY:
				return contents::FillPathCompatibilityOperatorPtr();
			case Token::Type::FILL_PATH_EVEN_ODD:
				return contents::FillPathEvenOddOperatorPtr();
			case Token::Type::FILL_STROKE_NONZERO:
				return contents::FillStrokeNonzeroOperatorPtr();
			case Token::Type::FILL_STROKE_EVEN_ODD:
				return contents::FillStrokeEvenOddOperatorPtr();
			case Token::Type::CLOSE_FILL_STROKE_NONZERO:
				return contents::CloseFillStrokeNonzeroOperatorPtr();
			case Token::Type::CLOSE_FILL_STROKE_EVEN_ODD:
				return contents::CloseFillStrokeEvenOddOperatorPtr();
			case Token::Type::END_PATH:
				return contents::EndPathOperatorPtr();
			case Token::Type::CLIP_PATH_NONZERO:
				return contents::ClipPathNonzeroOperatorPtr();
			case Token::Type::CLIP_PATH_EVEN_ODD:
				return contents::ClipPathEvenOddOperatorPtr();
			case Token::Type::BEGIN_TEXT:
				return contents::BeginTextOperatorPtr();
			case Token::Type::END_TEXT:
				return contents::EndTextOperatorPtr();
			case Token::Type::CHARACTER_SPACING:
				return contents::CharacterSpacingOperatorPtr();
			case Token::Type::WORD_SPACING:
				return contents::WordSpacingOperatorPtr();
			case Token::Type::HORIZONTAL_SCALING:
				return contents::HorizontalScalingOperatorPtr();
			case Token::Type::LEADING:
				return contents::LeadingOperatorPtr();
			case Token::Type::TEXT_FONT:
				return contents::TextFontOperatorPtr();
			case Token::Type::TEXT_RENDERING_MODE:
				return contents::TextRenderingModeOperatorPtr();
			case Token::Type::TEXT_RISE:
				return contents::TextRiseOperatorPtr();
			case Token::Type::TEXT_TRANSLATE:
				return contents::TextTranslateOperatorPtr();
			case Token::Type::TEXT_TRANSLATE_LEADING:
				return contents::TextTranslateLeadingOperatorPtr();
			case Token::Type::TEXT_MATRIX:
				return contents::TextMatrixOperatorPtr();
			case Token::Type::TEXT_NEXT_LINE:
				return contents::TextNextLineOperatorPtr();
			case Token::Type::TEXT_SHOW:
				return contents::TextShowOperatorPtr();
			case Token::Type::TEXT_SHOW_ARRAY:
				return contents::TextShowArrayOperatorPtr();
			case Token::Type::TEXT_NEXT_LINE_SHOW:
				return contents::TextNextLineShowOperatorPtr();
			case Token::Type::TEXT_NEXT_LINE_SHOW_SPACING:
				return contents::TextNextLineOperatorPtr();
			case Token::Type::SET_CHAR_WIDTH:
				return contents::SetCharWidthOperatorPtr();
			case Token::Type::SET_CACHE_DEVICE:
				return contents::SetCacheDeviceOperatorPtr();
			case Token::Type::COLOR_SPACE_STROKE:
				return contents::ColorSpaceStrokeOperatorPtr();
			case Token::Type::COLOR_SPACE_NONSTROKE:
				return contents::ColorSpaceNonstrokeOperatorPtr();
			case Token::Type::SET_COLOR_STROKE:
				return contents::SetColorStrokeOperatorPtr();
			case Token::Type::SET_COLOR_STROKE_EXTENDED:
				return contents::SetColorStrokeExtendedOperatorPtr();
			case Token::Type::SET_COLOR_NONSTROKE:
				return contents::SetColorNonstrokeOperatorPtr();
			case Token::Type::SET_COLOR_NONSTROKE_EXTENDED:
				return contents::SetColorNonstrokeExtendedOperatorPtr();
			case Token::Type::SET_STROKING_COLOR_SPACE_GRAY:
				return contents::SetNonstrokingColorSpaceGrayOperatorPtr();
			case Token::Type::SET_NONSTROKING_COLOR_SPACE_GRAY:
				return contents::SetNonstrokingColorSpaceGrayOperatorPtr();
			case Token::Type::SET_STROKING_COLOR_SPACE_RGB:
				return contents::SetStrokingColorSpaceRGBOperatorPtr();
			case Token::Type::SET_NONSTROKING_COLOR_SPACE_RGB:
				return contents::SetNonstrokingColorSpaceRGBOperatorPtr();
			case Token::Type::SET_STROKING_COLOR_SPACE_CMYK:
				return contents::SetStrokingColorSpaceCMYKOperatorPtr();
			case Token::Type::SET_NONSTROKING_COLOR_SPACE_CMYK:
				return contents::SetNonstrokingColorSpaceCMYKOperatorPtr();
			case Token::Type::SHADING_PAINT:
				return contents::ShadingPaintOperatorPtr();
			case Token::Type::BEGIN_INLINE_IMAGE_OBJECT:
				return contents::BeginInlineImageObjectOperatorPtr();
			case Token::Type::BEGIN_INLINE_IMAGE_DATA:
				throw NotSupportedException("Begin inline image data operator is not yet supported");
				return contents::BeginInlineImageDataOperatorPtr();
			case Token::Type::END_INLINE_IMAGE_OBJECT:
				return contents::EndInlineImageObjectOperatorPtr();
			case Token::Type::INVOKE_X_OBJECT:
				return contents::InvokeXObjectOperatorPtr();
			case Token::Type::DEFINE_MARKED_CONTENT_POINT:
				return contents::DefineMarkedContentPointOperatorPtr();
			case Token::Type::DEFINE_MARKED_CONTENT_POINT_WITH_PROPERTY_LIST:
				return contents::DefineMarkedContentPointWithPropertyListOperatorPtr();
			case Token::Type::BEGIN_MARKED_CONTENT_SEQUENCE:
				return contents::BeginMarkedContentSequenceOperatorPtr();
			case Token::Type::BEGIN_MARKED_CONTENT_SEQUENCE_WITH_PROPERTY_LIST:
				return contents::BeginMarkedContentSequenceWithPropertyListOperatorPtr();
			case Token::Type::END_MARKED_CONTENT_SEQUENCE:
				return contents::EndMarkedContentSequenceOperatorPtr();
			case Token::Type::BEGIN_COMPATIBILITY_SECTION:
				return contents::BeginCompatibilitySectionOperatorPtr();
			case Token::Type::END_COMPATIBILITY_SECTION:
				return contents::EndCompatibilitySectionOperatorPtr();
			default:
				return contents::UnknownOperatorPtr(token->Value());
			}
		}

		bool Parser::IsOperand(Token::Type type)
		{
			switch (type)
			{
			case Token::Type::DICTIONARY_BEGIN:
			case Token::Type::INTEGER_OBJECT:
			case Token::Type::ARRAY_BEGIN:
			case Token::Type::NAME_OBJECT:
			case Token::Type::HEXADECIMAL_STRING:
			case Token::Type::LITERAL_STRING:
			case Token::Type::REAL_OBJECT:
			case Token::Type::TRUE_VALUE:
			case Token::Type::FALSE_VALUE:
			case Token::Type::NULL_OBJECT:
				return true;
			}

			return false;
		}

		ObjectPtr Parser::ReadOperand()
		{
			auto offset = tellg();
			switch (PeekTokenTypeSkip())
			{
			case Token::Type::DICTIONARY_BEGIN:
				return ReadDictionary();
			case Token::Type::INTEGER_OBJECT:
				return ReadInteger();
			case Token::Type::ARRAY_BEGIN:
				return ReadArray();
			case Token::Type::NAME_OBJECT:
				return ReadName();
			case Token::Type::HEXADECIMAL_STRING:
				return ReadHexadecimalString();
			case Token::Type::LITERAL_STRING:
				return ReadLiteralString();
			case Token::Type::REAL_OBJECT:
				return ReadReal();
			case Token::Type::TRUE_VALUE:
				return ReadTrue();
			case Token::Type::FALSE_VALUE:
				return ReadFalse();
			case Token::Type::NULL_OBJECT:
				return ReadNull();
			default:
				throw ParseException(offset);
			}
		}

#pragma endregion

		HeaderPtr Parser::ReadHeader(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			return ReadHeader();
		}

		HeaderPtr Parser::ReadHeader(void)
		{
			HeaderPtr result;

			std::smatch sm;
			std::regex header_regex("%PDF-1\\.([0-7]).*");
			std::string line;

			for (;;) {
				auto data = readline();
				line = data->ToString();

				if (std::regex_match(line, sm, header_regex))
					break;
			}

			switch (stoi(sm[1]))
			{
			case 0:
				result->SetVersion(Version::PDF10); break;
			case 1:
				result->SetVersion(Version::PDF11); break;
			case 2:
				result->SetVersion(Version::PDF12); break;
			case 3:
				result->SetVersion(Version::PDF13); break;
			case 4:
				result->SetVersion(Version::PDF14); break;
			case 5:
				result->SetVersion(Version::PDF15); break;
			case 6:
				result->SetVersion(Version::PDF16); break;
			case 7:
				result->SetVersion(Version::PDF17); break;
			default:
				assert(!"If happens, error in regular expression");
			}

			return result;
		}

	}
}
