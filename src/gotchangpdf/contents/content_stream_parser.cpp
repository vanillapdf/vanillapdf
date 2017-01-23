#include "precompiled.h"

#include "contents/content_stream_parser.h"
#include "contents/content_stream_operations.h"
#include "contents/content_stream_operation_generic.h"
#include "contents/content_stream_operators.h"
#include "contents/content_stream_objects.h"
#include "contents/content_utils.h"

#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/character.h"

namespace gotchangpdf {
namespace contents {

using namespace syntax;

ContentStreamParser::ContentStreamParser(WeakReference<File> file, CharacterSource & stream)
	: ParserBase(file, stream) {
	_dictionary = make_unique<ContentStreamTokenDictionary>();
	_dictionary->Initialize();
}

BaseInstructionCollectionPtr ContentStreamParser::ReadContentStreamInstructions(void) {
	BaseInstructionCollectionPtr result;
	while (PeekTokenTypeSkip() != Token::Type::END_OF_INPUT) {
		auto operation = ReadContentStreamInstruction();
		operation->SetInitialized();
		result->push_back(operation);
	}

	return result;
}

InlineImageObjectPtr ContentStreamParser::ReadInlineImageObject(void) {
	// read dictionary
	DictionaryObjectPtr image_dictionary;

	for (;;) {
		auto token_type = PeekTokenTypeSkip();
		if (token_type == Token::Type::NAME_OBJECT) {
			auto name = ReadName();
			auto value = ReadDirectObject();

			if (value->GetType() == Object::Type::Null)
				continue;

			auto containable_ptr = dynamic_cast<ContainableObject*>(value.get());
			if (nullptr == containable_ptr)
				throw ConversionExceptionFactory<ContainableObject>::Construct(value);

			image_dictionary->Insert(name, ContainableObjectPtr(containable_ptr));
			continue;
		}

		if (token_type == Token::Type::BEGIN_INLINE_IMAGE_DATA) {
			break;
		}

		assert(!"Unknown data in inline image dictionary");
	}

	// read operation begin image data
	auto inline_image_data_op = ReadContentStreamOperation();
	if (inline_image_data_op->GetOperationType() != OperationBase::Type::BeginInlineImageData) {
		assert(!"Invalid operation after inline image dictionary");
	}

	// read data
	BufferPtr image_data;

	int stage = 0;
	unsigned char stored_whitespace = 0;
	for (;;) {
		auto current_meta = get();
		auto current = ValueConvertUtils::SafeConvert<unsigned char>(current_meta);

		if (stage == 0 && IsWhiteSpace(current)) {
			stage = 1;
			stored_whitespace = current;
			continue;
		}

		if (stage == 1) {
			if (current == 'E') {
				stage = 2;
				continue;
			}

			if (IsWhiteSpace(current)) {
				image_data->push_back(stored_whitespace);
				stored_whitespace = current;
				continue;
			}

			// reset stage
			stage = 0;

			image_data->push_back(stored_whitespace);
			image_data->push_back(current);
			continue;
		}

		if (stage == 2) {
			if (current == 'I') {
				stage = 3;
				continue;
			}

			// reset stage
			stage = 0;

			// pop stored data
			image_data->push_back(stored_whitespace);
			image_data->push_back('E');
			image_data->push_back(current);
			continue;
		}

		if (stage == 3) {
			if (IsWhiteSpace(current)) {
				// verify data
				break;
			}

			// reset stage
			stage = 0;

			// pop stored data
			image_data->push_back(stored_whitespace);
			image_data->push_back('E');
			image_data->push_back('I');
			image_data->push_back(current);
			continue;
		}

		image_data->push_back(current);
	}

	return InlineImageObjectPtr(image_dictionary, image_data);
}

InstructionBasePtr ContentStreamParser::ReadContentStreamInstruction(void) {
	auto operation = ReadContentStreamOperation();

	if (operation->GetOperationType() == OperationBase::Type::EndText) {
		// This seems, that someone is trying to parse content stream,
		// that is part of multiple streams
		// Please be sure, that entry source data are all concatenated into
		// stream and not parsed partially
		assert(!"Found EndText operation without begin");
	}

	if (operation->GetOperationType() == OperationBase::Type::BeginInlineImageObject) {
		return ReadInlineImageObject();
	}

	if (operation->GetOperationType() == OperationBase::Type::BeginText) {
		BaseOperationCollection text_operations;

		for (;;) {
			if (PeekTokenTypeSkip() == Token::Type::END_OF_INPUT) {
				// This seems, that someone is trying to parse content stream,
				// that is part of multiple streams
				// Please be sure, that entry source data are all concatenated into
				// stream and not parsed partially
				assert(!"Found BeginText operation without end"); break;
			}

			auto text_operation = ReadContentStreamOperation();
			if (text_operation->GetOperationType() == OperationBase::Type::EndText) {
				break;
			}

			text_operations.push_back(text_operation);
		}

		return TextObjectPtr(text_operations);
	}

	return operation;
}

OperationBasePtr ContentStreamParser::ReadContentStreamOperation(void) {
	std::vector<ObjectPtr> operands;
	while (IsOperand(PeekTokenTypeSkip())) {
		auto operand = ReadOperand();
		operand->SetEncryptionExempted();
		operand->SetInitialized();
		operands.push_back(operand);
	}

	return ReadOperatorReturnOperation(operands);
}

OperationBasePtr ContentStreamParser::ReadOperatorReturnOperation(const std::vector<ObjectPtr>& operands) {
	auto token = ReadTokenSkip();
	switch (token->GetType()) {
		case Token::Type::LINE_WIDTH:
			return OperationGenericPtr(operands, LineWidthOperatorPtr());
		case Token::Type::LINE_CAP:
			return OperationGenericPtr(operands, LineCapOperatorPtr());
		case Token::Type::LINE_JOIN:
			return OperationGenericPtr(operands, LineJoinOperatorPtr());
		case Token::Type::MITER_LIMIT:
			return OperationGenericPtr(operands, MiterLimitOperatorPtr());
		case Token::Type::DASH_PATTERN:
			return OperationGenericPtr(operands, DashPatternOperatorPtr());
		case Token::Type::COLOR_RENDERING_INTENT:
			return OperationGenericPtr(operands, ColorRenderingIntentOperatorPtr());
		case Token::Type::FLATNESS:
			return OperationGenericPtr(operands, FlatnessOperatorPtr());
		case Token::Type::GRAPHICS_STATE:
			return OperationGenericPtr(operands, GraphicsStateOperatorPtr());
		case Token::Type::SAVE_GRAPHICS_STATE:
			return OperationGenericPtr(operands, SaveGraphicsStateOperatorPtr());
		case Token::Type::RESTORE_GRAPHIC_STATE:
			return OperationGenericPtr(operands, RestoreGraphicsStateOperatorPtr());
		case Token::Type::TRANSFORMATION_MATRIX:
			return OperationGenericPtr(operands, TransformationMatrixOperatorPtr());
		case Token::Type::BEGIN_SUBPATH:
			return OperationGenericPtr(operands, BeginSubpathOperatorPtr());
		case Token::Type::LINE:
			return OperationGenericPtr(operands, LineOperatorPtr());
		case Token::Type::FULL_CURVE:
			return OperationGenericPtr(operands, FullCurveOperatorPtr());
		case Token::Type::FINAL_CURVE:
			return OperationGenericPtr(operands, FinalCurveOperatorPtr());
		case Token::Type::INITIAL_CURVE:
			return OperationGenericPtr(operands, InitialCurveOperatorPtr());
		case Token::Type::CLOSE_SUBPATH:
			return OperationGenericPtr(operands, CloseSubpathOperatorPtr());
		case Token::Type::RECTANGLE:
			return OperationGenericPtr(operands, RectangleOperatorPtr());
		case Token::Type::STROKE:
			return OperationGenericPtr(operands, StrokeOperatorPtr());
		case Token::Type::CLOSE_AND_STROKE:
			return OperationGenericPtr(operands, CloseAndStrokeOperatorPtr());
		case Token::Type::FILL_PATH_NONZERO:
			return OperationGenericPtr(operands, FillPathNonzeroOperatorPtr());
		case Token::Type::FILL_PATH_COMPATIBILITY:
			return OperationGenericPtr(operands, FillPathCompatibilityOperatorPtr());
		case Token::Type::FILL_PATH_EVEN_ODD:
			return OperationGenericPtr(operands, FillPathEvenOddOperatorPtr());
		case Token::Type::FILL_STROKE_NONZERO:
			return OperationGenericPtr(operands, FillStrokeNonzeroOperatorPtr());
		case Token::Type::FILL_STROKE_EVEN_ODD:
			return OperationGenericPtr(operands, FillStrokeEvenOddOperatorPtr());
		case Token::Type::CLOSE_FILL_STROKE_NONZERO:
			return OperationGenericPtr(operands, CloseFillStrokeNonzeroOperatorPtr());
		case Token::Type::CLOSE_FILL_STROKE_EVEN_ODD:
			return OperationGenericPtr(operands, CloseFillStrokeEvenOddOperatorPtr());
		case Token::Type::END_PATH:
			return OperationGenericPtr(operands, EndPathOperatorPtr());
		case Token::Type::CLIP_PATH_NONZERO:
			return OperationGenericPtr(operands, ClipPathNonzeroOperatorPtr());
		case Token::Type::CLIP_PATH_EVEN_ODD:
			return OperationGenericPtr(operands, ClipPathEvenOddOperatorPtr());
		case Token::Type::BEGIN_TEXT:
			return OperationBeginTextPtr(operands);
		case Token::Type::END_TEXT:
			return OperationEndTextPtr(operands);
		case Token::Type::CHARACTER_SPACING:
			return OperationGenericPtr(operands, CharacterSpacingOperatorPtr());
		case Token::Type::WORD_SPACING:
			return OperationGenericPtr(operands, WordSpacingOperatorPtr());
		case Token::Type::HORIZONTAL_SCALING:
			return OperationGenericPtr(operands, HorizontalScalingOperatorPtr());
		case Token::Type::LEADING:
			return OperationGenericPtr(operands, LeadingOperatorPtr());
		case Token::Type::TEXT_FONT:
			return OperationTextFontPtr(operands);
		case Token::Type::TEXT_RENDERING_MODE:
			return OperationGenericPtr(operands, TextRenderingModeOperatorPtr());
		case Token::Type::TEXT_RISE:
			return OperationGenericPtr(operands, TextRiseOperatorPtr());
		case Token::Type::TEXT_TRANSLATE:
			return OperationGenericPtr(operands, TextTranslateOperatorPtr());
		case Token::Type::TEXT_TRANSLATE_LEADING:
			return OperationGenericPtr(operands, TextTranslateLeadingOperatorPtr());
		case Token::Type::TEXT_MATRIX:
			return OperationGenericPtr(operands, TextMatrixOperatorPtr());
		case Token::Type::TEXT_NEXT_LINE:
			return OperationGenericPtr(operands, TextNextLineOperatorPtr());
		case Token::Type::TEXT_SHOW:
			return OperationTextShowPtr(operands);
		case Token::Type::TEXT_SHOW_ARRAY:
			return OperationTextShowArrayPtr(operands);
		case Token::Type::TEXT_NEXT_LINE_SHOW:
			return OperationGenericPtr(operands, TextNextLineShowOperatorPtr());
		case Token::Type::TEXT_NEXT_LINE_SHOW_SPACING:
			return OperationGenericPtr(operands, TextNextLineShowSpacingOperatorPtr());
		case Token::Type::SET_CHAR_WIDTH:
			return OperationGenericPtr(operands, SetCharWidthOperatorPtr());
		case Token::Type::SET_CACHE_DEVICE:
			return OperationGenericPtr(operands, SetCacheDeviceOperatorPtr());
		case Token::Type::COLOR_SPACE_STROKE:
			return OperationGenericPtr(operands, ColorSpaceStrokeOperatorPtr());
		case Token::Type::COLOR_SPACE_NONSTROKE:
			return OperationGenericPtr(operands, ColorSpaceNonstrokeOperatorPtr());
		case Token::Type::SET_COLOR_STROKE:
			return OperationGenericPtr(operands, SetColorStrokeOperatorPtr());
		case Token::Type::SET_COLOR_STROKE_EXTENDED:
			return OperationGenericPtr(operands, SetColorStrokeExtendedOperatorPtr());
		case Token::Type::SET_COLOR_NONSTROKE:
			return OperationGenericPtr(operands, SetColorNonstrokeOperatorPtr());
		case Token::Type::SET_COLOR_NONSTROKE_EXTENDED:
			return OperationGenericPtr(operands, SetColorNonstrokeExtendedOperatorPtr());
		case Token::Type::SET_STROKING_COLOR_SPACE_GRAY:
			return OperationGenericPtr(operands, SetStrokingColorSpaceGrayOperatorPtr());
		case Token::Type::SET_NONSTROKING_COLOR_SPACE_GRAY:
			return OperationGenericPtr(operands, SetNonstrokingColorSpaceGrayOperatorPtr());
		case Token::Type::SET_STROKING_COLOR_SPACE_RGB:
			return OperationGenericPtr(operands, SetStrokingColorSpaceRGBOperatorPtr());
		case Token::Type::SET_NONSTROKING_COLOR_SPACE_RGB:
			return OperationGenericPtr(operands, SetNonstrokingColorSpaceRGBOperatorPtr());
		case Token::Type::SET_STROKING_COLOR_SPACE_CMYK:
			return OperationGenericPtr(operands, SetStrokingColorSpaceCMYKOperatorPtr());
		case Token::Type::SET_NONSTROKING_COLOR_SPACE_CMYK:
			return OperationGenericPtr(operands, SetNonstrokingColorSpaceCMYKOperatorPtr());
		case Token::Type::SHADING_PAINT:
			return OperationGenericPtr(operands, ShadingPaintOperatorPtr());
		case Token::Type::BEGIN_INLINE_IMAGE_OBJECT:
			return OperationBeginInlineImageObjectPtr(operands);
		case Token::Type::BEGIN_INLINE_IMAGE_DATA:
			return OperationBeginInlineImageDataPtr(operands);
		case Token::Type::END_INLINE_IMAGE_OBJECT:
			return OperationEndInlineImageObjectPtr(operands);
		case Token::Type::INVOKE_X_OBJECT:
			return OperationGenericPtr(operands, InvokeXObjectOperatorPtr());
		case Token::Type::DEFINE_MARKED_CONTENT_POINT:
			return OperationGenericPtr(operands, DefineMarkedContentPointOperatorPtr());
		case Token::Type::DEFINE_MARKED_CONTENT_POINT_WITH_PROPERTY_LIST:
			return OperationGenericPtr(operands, DefineMarkedContentPointWithPropertyListOperatorPtr());
		case Token::Type::BEGIN_MARKED_CONTENT_SEQUENCE:
			return OperationGenericPtr(operands, BeginMarkedContentSequenceOperatorPtr());
		case Token::Type::BEGIN_MARKED_CONTENT_SEQUENCE_WITH_PROPERTY_LIST:
			return OperationGenericPtr(operands, BeginMarkedContentSequenceWithPropertyListOperatorPtr());
		case Token::Type::END_MARKED_CONTENT_SEQUENCE:
			return OperationGenericPtr(operands, EndMarkedContentSequenceOperatorPtr());
		case Token::Type::BEGIN_COMPATIBILITY_SECTION:
			return OperationGenericPtr(operands, BeginCompatibilitySectionOperatorPtr());
		case Token::Type::END_COMPATIBILITY_SECTION:
			return OperationGenericPtr(operands, EndCompatibilitySectionOperatorPtr());
		default:
			return OperationGenericPtr(operands, UnknownOperatorPtr(token->Value()));
	}
}

bool ContentStreamParser::IsOperand(Token::Type type) {
	switch (type) {
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

ObjectPtr ContentStreamParser::ReadOperand() {
	auto offset = GetPosition();
	switch (PeekTokenTypeSkip()) {
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

} // contents
} // gotchangpdf