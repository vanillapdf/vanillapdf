#include "precompiled.h"

#include "contents/content_stream_parser.h"
#include "contents/content_stream_operations.h"
#include "contents/content_stream_operation_generic.h"
#include "contents/content_stream_operators.h"
#include "contents/content_stream_objects.h"
#include "contents/content_utils.h"

#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/character.h"
#include "utils/license_info.h"

namespace vanillapdf {
namespace contents {

using namespace syntax;

ContentStreamParser::ContentStreamParser(WeakReference<File> file, IInputStreamPtr stream)
	: ParserBase(file, stream) {
	_dictionary = make_unique<ContentStreamTokenDictionary>();
	_dictionary->Initialize();
}

BaseInstructionCollectionPtr ContentStreamParser::ReadInstructions(void) {
	BaseInstructionCollectionPtr result;

	std::list<InstructionBasePtr> instructions;
	while (PeekTokenTypeSkip() != Token::Type::END_OF_INPUT) {
		auto operation = ReadInstruction();
		operation->SetInitialized();
		instructions.push_back(operation);
	}

	result->reserve(instructions.size());
	result->assign(instructions.begin(), instructions.end());

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

			if (value->GetObjectType() == Object::Type::Null) {
				continue;
			}

			auto containable_ptr = dynamic_cast<ContainableObject*>(value.get());
			if (nullptr == containable_ptr) {
				throw ConversionExceptionFactory<ContainableObject>::Construct(value);
			}

			image_dictionary->Insert(name, ContainableObjectPtr(containable_ptr));
			continue;
		}

		if (token_type == Token::Type::BEGIN_INLINE_IMAGE_DATA) {
			break;
		}

		assert(!"Unknown data in inline image dictionary");
	}

	// read operation begin image data
	auto inline_image_data_op = ReadOperation();
	if (inline_image_data_op->GetOperationType() != OperationBase::Type::BeginInlineImageData) {
		assert(!"Invalid operation after inline image dictionary");
	}

	// read data
	BufferPtr image_data;

	int stage = 0;
	unsigned char stored_whitespace = 0;
	for (;;) {
		auto current_meta = m_stream->Get();
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

	return make_deferred<InlineImageObject>(image_dictionary, image_data);
}

InstructionBasePtr ContentStreamParser::ReadInstruction(void) {
	auto operation = ReadOperation();

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

			auto text_operation = ReadOperation();
			if (text_operation->GetOperationType() == OperationBase::Type::EndText) {
				break;
			}

			text_operations.push_back(text_operation);
		}

		return make_deferred<TextObject>(text_operations);
	}

	return operation;
}

OperationBasePtr ContentStreamParser::ReadOperation(void) {
	std::vector<syntax::ObjectPtr> operands;
	while (IsOperand(PeekTokenTypeSkip())) {
		auto operand = ReadOperand();
		operand->SetEncryptionExempted();
		operand->SetInitialized();
		operands.push_back(operand);
	}

	return ReadOperatorReturnOperation(operands);
}

OperationBasePtr ContentStreamParser::ReadOperatorReturnOperation(const std::vector<syntax::ObjectPtr>& operands) {
	auto token = ReadTokenSkip();
	switch (token->GetType()) {
		case Token::Type::LINE_WIDTH:
			return make_deferred<OperationGeneric>(operands, LineWidthOperatorPtr());
		case Token::Type::LINE_CAP:
			return make_deferred<OperationGeneric>(operands, LineCapOperatorPtr());
		case Token::Type::LINE_JOIN:
			return make_deferred<OperationGeneric>(operands, LineJoinOperatorPtr());
		case Token::Type::MITER_LIMIT:
			return make_deferred<OperationGeneric>(operands, MiterLimitOperatorPtr());
		case Token::Type::DASH_PATTERN:
			return make_deferred<OperationGeneric>(operands, DashPatternOperatorPtr());
		case Token::Type::COLOR_RENDERING_INTENT:
			return make_deferred<OperationGeneric>(operands, ColorRenderingIntentOperatorPtr());
		case Token::Type::FLATNESS:
			return make_deferred<OperationGeneric>(operands, FlatnessOperatorPtr());
		case Token::Type::GRAPHICS_STATE:
			return make_deferred<OperationGeneric>(operands, GraphicsStateOperatorPtr());
		case Token::Type::SAVE_GRAPHICS_STATE:
			return make_deferred<OperationGeneric>(operands, SaveGraphicsStateOperatorPtr());
		case Token::Type::RESTORE_GRAPHIC_STATE:
			return make_deferred<OperationGeneric>(operands, RestoreGraphicsStateOperatorPtr());
		case Token::Type::TRANSFORMATION_MATRIX:
			return make_deferred<OperationGeneric>(operands, TransformationMatrixOperatorPtr());
		case Token::Type::BEGIN_SUBPATH:
			return make_deferred<OperationGeneric>(operands, BeginSubpathOperatorPtr());
		case Token::Type::LINE:
			return make_deferred<OperationGeneric>(operands, LineOperatorPtr());
		case Token::Type::FULL_CURVE:
			return make_deferred<OperationGeneric>(operands, FullCurveOperatorPtr());
		case Token::Type::FINAL_CURVE:
			return make_deferred<OperationGeneric>(operands, FinalCurveOperatorPtr());
		case Token::Type::INITIAL_CURVE:
			return make_deferred<OperationGeneric>(operands, InitialCurveOperatorPtr());
		case Token::Type::CLOSE_SUBPATH:
			return make_deferred<OperationGeneric>(operands, CloseSubpathOperatorPtr());
		case Token::Type::RECTANGLE:
			return make_deferred<OperationGeneric>(operands, RectangleOperatorPtr());
		case Token::Type::STROKE:
			return make_deferred<OperationGeneric>(operands, StrokeOperatorPtr());
		case Token::Type::CLOSE_AND_STROKE:
			return make_deferred<OperationGeneric>(operands, CloseAndStrokeOperatorPtr());
		case Token::Type::FILL_PATH_NONZERO:
			return make_deferred<OperationGeneric>(operands, FillPathNonzeroOperatorPtr());
		case Token::Type::FILL_PATH_COMPATIBILITY:
			return make_deferred<OperationGeneric>(operands, FillPathCompatibilityOperatorPtr());
		case Token::Type::FILL_PATH_EVEN_ODD:
			return make_deferred<OperationGeneric>(operands, FillPathEvenOddOperatorPtr());
		case Token::Type::FILL_STROKE_NONZERO:
			return make_deferred<OperationGeneric>(operands, FillStrokeNonzeroOperatorPtr());
		case Token::Type::FILL_STROKE_EVEN_ODD:
			return make_deferred<OperationGeneric>(operands, FillStrokeEvenOddOperatorPtr());
		case Token::Type::CLOSE_FILL_STROKE_NONZERO:
			return make_deferred<OperationGeneric>(operands, CloseFillStrokeNonzeroOperatorPtr());
		case Token::Type::CLOSE_FILL_STROKE_EVEN_ODD:
			return make_deferred<OperationGeneric>(operands, CloseFillStrokeEvenOddOperatorPtr());
		case Token::Type::END_PATH:
			return make_deferred<OperationGeneric>(operands, EndPathOperatorPtr());
		case Token::Type::CLIP_PATH_NONZERO:
			return make_deferred<OperationGeneric>(operands, ClipPathNonzeroOperatorPtr());
		case Token::Type::CLIP_PATH_EVEN_ODD:
			return make_deferred<OperationGeneric>(operands, ClipPathEvenOddOperatorPtr());
		case Token::Type::BEGIN_TEXT:
			return make_deferred<OperationBeginText>(operands);
		case Token::Type::END_TEXT:
			return make_deferred<OperationEndText>(operands);
		case Token::Type::CHARACTER_SPACING:
			return make_deferred<OperationGeneric>(operands, CharacterSpacingOperatorPtr());
		case Token::Type::WORD_SPACING:
			return make_deferred<OperationGeneric>(operands, WordSpacingOperatorPtr());
		case Token::Type::HORIZONTAL_SCALING:
			return make_deferred<OperationGeneric>(operands, HorizontalScalingOperatorPtr());
		case Token::Type::LEADING:
			return make_deferred<OperationGeneric>(operands, LeadingOperatorPtr());
		case Token::Type::TEXT_FONT:
			return make_deferred<OperationTextFont>(operands);
		case Token::Type::TEXT_RENDERING_MODE:
			return make_deferred<OperationGeneric>(operands, TextRenderingModeOperatorPtr());
		case Token::Type::TEXT_RISE:
			return make_deferred<OperationGeneric>(operands, TextRiseOperatorPtr());
		case Token::Type::TEXT_TRANSLATE:
			return make_deferred<OperationGeneric>(operands, TextTranslateOperatorPtr());
		case Token::Type::TEXT_TRANSLATE_LEADING:
			return make_deferred<OperationGeneric>(operands, TextTranslateLeadingOperatorPtr());
		case Token::Type::TEXT_MATRIX:
			return make_deferred<OperationGeneric>(operands, TextMatrixOperatorPtr());
		case Token::Type::TEXT_NEXT_LINE:
			return make_deferred<OperationGeneric>(operands, TextNextLineOperatorPtr());
		case Token::Type::TEXT_SHOW:
			return make_deferred<OperationTextShow>(operands);
		case Token::Type::TEXT_SHOW_ARRAY:
			return make_deferred<OperationTextShowArray>(operands);
		case Token::Type::TEXT_NEXT_LINE_SHOW:
			return make_deferred<OperationGeneric>(operands, TextNextLineShowOperatorPtr());
		case Token::Type::TEXT_NEXT_LINE_SHOW_SPACING:
			return make_deferred<OperationGeneric>(operands, TextNextLineShowSpacingOperatorPtr());
		case Token::Type::SET_CHAR_WIDTH:
			return make_deferred<OperationGeneric>(operands, SetCharWidthOperatorPtr());
		case Token::Type::SET_CACHE_DEVICE:
			return make_deferred<OperationGeneric>(operands, SetCacheDeviceOperatorPtr());
		case Token::Type::COLOR_SPACE_STROKE:
			return make_deferred<OperationGeneric>(operands, ColorSpaceStrokeOperatorPtr());
		case Token::Type::COLOR_SPACE_NONSTROKE:
			return make_deferred<OperationGeneric>(operands, ColorSpaceNonstrokeOperatorPtr());
		case Token::Type::SET_COLOR_STROKE:
			return make_deferred<OperationGeneric>(operands, SetColorStrokeOperatorPtr());
		case Token::Type::SET_COLOR_STROKE_EXTENDED:
			return make_deferred<OperationGeneric>(operands, SetColorStrokeExtendedOperatorPtr());
		case Token::Type::SET_COLOR_NONSTROKE:
			return make_deferred<OperationGeneric>(operands, SetColorNonstrokeOperatorPtr());
		case Token::Type::SET_COLOR_NONSTROKE_EXTENDED:
			return make_deferred<OperationGeneric>(operands, SetColorNonstrokeExtendedOperatorPtr());
		case Token::Type::SET_STROKING_COLOR_SPACE_GRAY:
			return make_deferred<OperationGeneric>(operands, SetStrokingColorSpaceGrayOperatorPtr());
		case Token::Type::SET_NONSTROKING_COLOR_SPACE_GRAY:
			return make_deferred<OperationGeneric>(operands, SetNonstrokingColorSpaceGrayOperatorPtr());
		case Token::Type::SET_STROKING_COLOR_SPACE_RGB:
			return make_deferred<OperationGeneric>(operands, SetStrokingColorSpaceRGBOperatorPtr());
		case Token::Type::SET_NONSTROKING_COLOR_SPACE_RGB:
			return make_deferred<OperationGeneric>(operands, SetNonstrokingColorSpaceRGBOperatorPtr());
		case Token::Type::SET_STROKING_COLOR_SPACE_CMYK:
			return make_deferred<OperationGeneric>(operands, SetStrokingColorSpaceCMYKOperatorPtr());
		case Token::Type::SET_NONSTROKING_COLOR_SPACE_CMYK:
			return make_deferred<OperationGeneric>(operands, SetNonstrokingColorSpaceCMYKOperatorPtr());
		case Token::Type::SHADING_PAINT:
			return make_deferred<OperationGeneric>(operands, ShadingPaintOperatorPtr());
		case Token::Type::BEGIN_INLINE_IMAGE_OBJECT:
			return make_deferred<OperationBeginInlineImageObject>(operands);
		case Token::Type::BEGIN_INLINE_IMAGE_DATA:
			return make_deferred<OperationBeginInlineImageData>(operands);
		case Token::Type::END_INLINE_IMAGE_OBJECT:
			return make_deferred<OperationEndInlineImageObject>(operands);
		case Token::Type::INVOKE_X_OBJECT:
			return make_deferred<OperationGeneric>(operands, InvokeXObjectOperatorPtr());
		case Token::Type::DEFINE_MARKED_CONTENT_POINT:
			return make_deferred<OperationGeneric>(operands, DefineMarkedContentPointOperatorPtr());
		case Token::Type::DEFINE_MARKED_CONTENT_POINT_WITH_PROPERTY_LIST:
			return make_deferred<OperationGeneric>(operands, DefineMarkedContentPointWithPropertyListOperatorPtr());
		case Token::Type::BEGIN_MARKED_CONTENT_SEQUENCE:
			return make_deferred<OperationGeneric>(operands, BeginMarkedContentSequenceOperatorPtr());
		case Token::Type::BEGIN_MARKED_CONTENT_SEQUENCE_WITH_PROPERTY_LIST:
			return make_deferred<OperationGeneric>(operands, BeginMarkedContentSequenceWithPropertyListOperatorPtr());
		case Token::Type::END_MARKED_CONTENT_SEQUENCE:
			return make_deferred<OperationGeneric>(operands, EndMarkedContentSequenceOperatorPtr());
		case Token::Type::BEGIN_COMPATIBILITY_SECTION:
			return make_deferred<OperationGeneric>(operands, BeginCompatibilitySectionOperatorPtr());
		case Token::Type::END_COMPATIBILITY_SECTION:
			return make_deferred<OperationGeneric>(operands, EndCompatibilitySectionOperatorPtr());
		default:
			return make_deferred<OperationGeneric>(operands, make_deferred<UnknownOperator>(token->Value()));
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
		default:
			return false;
	}
}

ObjectPtr ContentStreamParser::ReadOperand() {
	auto offset = m_stream->GetInputPosition();
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
} // vanillapdf
