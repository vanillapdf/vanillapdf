#ifndef _SYNTAX_FWD_H
#define _SYNTAX_FWD_H

namespace gotchangpdf
{
	template <typename T>
	class Deferred;

	template <typename T>
	class DeferredContainer;

	template <typename T>
	class DeferredIterator;

	class Buffer;
	using BufferPtr = DeferredContainer<Buffer>;

	// Exceptions
	class ExceptionBase;
	class GeneralException;

	class IEncryptionKey;

	namespace syntax
	{
		// Exceptions
		class ConversionException;
		class FileDisposedException;
		class InvalidObjectTypeException;

		// Filters
		class FilterBase; using FilterBasePtr = Deferred<FilterBase>;		
		class ASCII85DecodeFilter; using ASCII85DecodeFilterPtr = Deferred<ASCII85DecodeFilter>;
		class ASCIIHexDecodeFilter; using ASCIIHexDecodeFilterPtr = Deferred<ASCIIHexDecodeFilter>;
		class FlateDecodeFilter; using FlateDecodeFilterPtr = Deferred<FlateDecodeFilter>;

		// Files
		class File;
		class FileHolder; using FileHolderPtr = Deferred<FileHolder>;
		class Header;

		class XrefBase; using XrefBasePtr = Deferred<XrefBase>;
		class XrefSubsection;
		class XrefTable;
		class XrefStream;
		class XrefChain;

		class XrefEntryBase;
		class XrefNullEntry;
		class XrefUsedEntry;
		class XrefFreeEntry;
		class XrefCompressedEntry;

		using HeaderPtr = Deferred<Header>;

		using XrefTablePtr = Deferred<XrefTable>;
		using XrefStreamPtr = Deferred<XrefStream>;

		using XrefChainPtr = Deferred<XrefChain>;
		using XrefEntryBasePtr = Deferred<XrefEntryBase>;
		using XrefNullEntryPtr = Deferred<XrefNullEntry>;
		using XrefFreeEntryPtr = Deferred<XrefFreeEntry>;
		using XrefUsedEntryPtr = Deferred<XrefUsedEntry>;
		using XrefCompressedEntryPtr = Deferred<XrefCompressedEntry>;

		class Token; using TokenPtr = Deferred<Token>;

		// Streams
		class BaseStream;
		class Stream;
		class ReverseStream;

		// Objects
		class ObjectUtils;
		class Object;
		class ContainableObject;

		template <typename KeyT, typename ValueT, typename MapT /*= std::map<KeyT, ValueT>*/>
		class DictionaryObjectBase;

		template <typename T>
		class ArrayObject;

		class MixedArrayObject;
		class BooleanObject;
		class DictionaryObject;
		class IndirectObjectReference;
		class IntegerObject;
		class NameObject;
		class NullObject;
		class NumericObject;
		class Object;
		class RealObject;
		class StreamObject;
		class StringObjectBase;
		class HexadecimalStringObject;
		class LiteralStringObject;

		template <typename T>
		using ArrayObjectPtr = Deferred<ArrayObject<T>>;

		template <typename KeyT, typename ValueT, typename MapT /*= std::map<KeyT, ValueT>*/>
		using DictionaryObjectBasePtr = Deferred<DictionaryObjectBase<KeyT, ValueT, MapT>>;

		using DictionaryObjectPtr = Deferred<DictionaryObject>;
		using MixedArrayObjectPtr = Deferred<MixedArrayObject>;

		using ObjectPtr = Deferred<Object>;
		using ContainableObjectPtr = Deferred<ContainableObject>;
		using NameObjectPtr = Deferred<NameObject>;
		using BooleanObjectPtr = Deferred<BooleanObject>;
		using IndirectObjectReferencePtr = Deferred<IndirectObjectReference>;
		using IntegerObjectPtr = Deferred<IntegerObject>;
		using NullObjectPtr = Deferred<NullObject>;
		using RealObjectPtr = Deferred<RealObject>;
		using StreamObjectPtr = Deferred<StreamObject>;
		using StringObjectPtr = Deferred<StringObjectBase>;
		using LiteralStringObjectPtr = Deferred<LiteralStringObject>;
		using HexadecimalStringObjectPtr = Deferred<HexadecimalStringObject>;

		// Parsers
		struct ObjectStreamHeader;

		namespace contents
		{
			// instructions
			class InstructionBase; using InstructionBasePtr = Deferred<InstructionBase>;

			// basic operations
			class OperationBase; using OperationBasePtr = Deferred<OperationBase>;
			class OperationGeneric; using OperationGenericPtr = Deferred<OperationGeneric>;

			// specialized operations
			class OperationBeginText; using OperationBeginTextPtr = Deferred<OperationBeginText>;
			class OperationEndText; using OperationEndTextPtr = Deferred<OperationEndText>;
			class OperationTextShow; using OperationTextShowPtr = Deferred<OperationTextShow>;
			class OperationBeginInlineImageObject; using OperationBeginInlineImageObjectPtr = Deferred<OperationBeginInlineImageObject>;
			class OperationBeginInlineImageData; using OperationBeginInlineImageDataPtr = Deferred<OperationBeginInlineImageData>;
			class OperationEndInlineImageObject; using OperationEndInlineImageObjectPtr = Deferred<OperationEndInlineImageObject>;

			// content objects
			class ContentObjectBase; using ContentObjectBasePtr = Deferred<ContentObjectBase>;
			class TextObject; using TextObjectPtr = Deferred<TextObject>;
			class InlineImageObject; using InlineImageObjectPtr = Deferred<InlineImageObject>;

			class OperatorBase;
			class UnknownOperator;

			class LineWidthOperator;
			class LineCapOperator;
			class LineJoinOperator;
			class MiterLimitOperator;
			class DashPatternOperator;
			class ColorRenderingIntentOperator;
			class FlatnessOperator;
			class GraphicsStateOperator;

			class SaveGraphicsStateOperator;
			class RestoreGraphicsStateOperator;
			class TransformationMatrixOperator;

			class BeginSubpathOperator;
			class LineOperator;
			class FullCurveOperator;
			class FinalCurveOperator;
			class InitialCurveOperator;
			class CloseSubpathOperator;
			class RectangleOperator;

			class StrokeOperator;
			class CloseAndStrokeOperator;
			class FillPathNonzeroOperator;
			class FillPathCompatibilityOperator;
			class FillPathEvenOddOperator;
			class FillStrokeNonzeroOperator;
			class FillStrokeEvenOddOperator;
			class CloseFillStrokeNonzeroOperator;
			class CloseFillStrokeEvenOddOperator;
			class EndPathOperator;

			class ClipPathNonzeroOperator;
			class ClipPathEvenOddOperator;

			class BeginTextOperator;
			class EndTextOperator;

			class CharacterSpacingOperator;
			class WordSpacingOperator;
			class HorizontalScalingOperator;
			class LeadingOperator;
			class TextFontOperator;
			class TextRenderingModeOperator;
			class TextRiseOperator;

			class TextTranslateOperator;
			class TextTranslateLeadingOperator;
			class TextMatrixOperator;
			class TextNextLineOperator;
			class TextShowOperator;
			class TextShowArrayOperator;
			class TextNextLineShowOperator;
			class TextNextLineShowSpacingOperator;

			class SetCharWidthOperator;
			class SetCacheDeviceOperator;

			class ColorSpaceStrokeOperator;
			class ColorSpaceNonstrokeOperator;
			class SetColorStrokeOperator;
			class SetColorStrokeExtendedOperator;
			class SetColorNonstrokeOperator;
			class SetColorNonstrokeExtendedOperator;
			class SetStrokingColorSpaceGrayOperator;
			class SetNonstrokingColorSpaceGrayOperator;
			class SetStrokingColorSpaceRGBOperator;
			class SetNonstrokingColorSpaceRGBOperator;
			class SetStrokingColorSpaceCMYKOperator;
			class SetNonstrokingColorSpaceCMYKOperator;

			class ShadingPaintOperator;

			class BeginInlineImageObjectOperator;
			class BeginInlineImageDataOperator;
			class EndInlineImageObjectOperator;

			class InvokeXObjectOperator;

			class DefineMarkedContentPointOperator;
			class DefineMarkedContentPointWithPropertyListOperator;
			class BeginMarkedContentSequenceOperator;
			class BeginMarkedContentSequenceWithPropertyListOperator;
			class EndMarkedContentSequenceOperator;

			class BeginCompatibilitySectionOperator;
			class EndCompatibilitySectionOperator;

			typedef Deferred<OperationBase> OperationBasePtr;
			typedef Deferred<OperationGeneric> OperationGenericPtr;

			typedef Deferred<OperatorBase> OperatorBasePtr;
			typedef Deferred<UnknownOperator> UnknownOperatorPtr;

			typedef Deferred<LineWidthOperator> LineWidthOperatorPtr;
			typedef Deferred<LineCapOperator> LineCapOperatorPtr;
			typedef Deferred<LineJoinOperator> LineJoinOperatorPtr;
			typedef Deferred<MiterLimitOperator> MiterLimitOperatorPtr;
			typedef Deferred<DashPatternOperator> DashPatternOperatorPtr;
			typedef Deferred<ColorRenderingIntentOperator> ColorRenderingIntentOperatorPtr;
			typedef Deferred<FlatnessOperator> FlatnessOperatorPtr;
			typedef Deferred<GraphicsStateOperator> GraphicsStateOperatorPtr;

			typedef Deferred<SaveGraphicsStateOperator> SaveGraphicsStateOperatorPtr;
			typedef Deferred<RestoreGraphicsStateOperator> RestoreGraphicsStateOperatorPtr;
			typedef Deferred<TransformationMatrixOperator> TransformationMatrixOperatorPtr;

			typedef Deferred<BeginSubpathOperator> BeginSubpathOperatorPtr;
			typedef Deferred<LineOperator> LineOperatorPtr;
			typedef Deferred<FullCurveOperator> FullCurveOperatorPtr;
			typedef Deferred<FinalCurveOperator> FinalCurveOperatorPtr;
			typedef Deferred<InitialCurveOperator> InitialCurveOperatorPtr;
			typedef Deferred<CloseSubpathOperator> CloseSubpathOperatorPtr;
			typedef Deferred<RectangleOperator> RectangleOperatorPtr;

			typedef Deferred<StrokeOperator> StrokeOperatorPtr;
			typedef Deferred<CloseAndStrokeOperator> CloseAndStrokeOperatorPtr;
			typedef Deferred<FillPathNonzeroOperator> FillPathNonzeroOperatorPtr;
			typedef Deferred<FillPathCompatibilityOperator> FillPathCompatibilityOperatorPtr;
			typedef Deferred<FillPathEvenOddOperator> FillPathEvenOddOperatorPtr;
			typedef Deferred<FillStrokeNonzeroOperator> FillStrokeNonzeroOperatorPtr;
			typedef Deferred<FillStrokeEvenOddOperator> FillStrokeEvenOddOperatorPtr;
			typedef Deferred<CloseFillStrokeNonzeroOperator> CloseFillStrokeNonzeroOperatorPtr;
			typedef Deferred<CloseFillStrokeEvenOddOperator> CloseFillStrokeEvenOddOperatorPtr;
			typedef Deferred<EndPathOperator> EndPathOperatorPtr;

			typedef Deferred<ClipPathNonzeroOperator> ClipPathNonzeroOperatorPtr;
			typedef Deferred<ClipPathEvenOddOperator> ClipPathEvenOddOperatorPtr;

			typedef Deferred<BeginTextOperator> BeginTextOperatorPtr;
			typedef Deferred<EndTextOperator> EndTextOperatorPtr;

			typedef Deferred<CharacterSpacingOperator> CharacterSpacingOperatorPtr;
			typedef Deferred<WordSpacingOperator> WordSpacingOperatorPtr;
			typedef Deferred<HorizontalScalingOperator> HorizontalScalingOperatorPtr;
			typedef Deferred<LeadingOperator> LeadingOperatorPtr;
			typedef Deferred<TextFontOperator> TextFontOperatorPtr;
			typedef Deferred<TextRenderingModeOperator> TextRenderingModeOperatorPtr;
			typedef Deferred<TextRiseOperator> TextRiseOperatorPtr;

			typedef Deferred<TextTranslateOperator> TextTranslateOperatorPtr;
			typedef Deferred<TextTranslateLeadingOperator> TextTranslateLeadingOperatorPtr;
			typedef Deferred<TextMatrixOperator> TextMatrixOperatorPtr;
			typedef Deferred<TextNextLineOperator> TextNextLineOperatorPtr;

			typedef Deferred<TextShowOperator> TextShowOperatorPtr;
			typedef Deferred<TextShowArrayOperator> TextShowArrayOperatorPtr;
			typedef Deferred<TextNextLineShowOperator> TextNextLineShowOperatorPtr;
			typedef Deferred<TextNextLineShowSpacingOperator> TextNextLineShowSpacingOperatorPtr;

			typedef Deferred<SetCharWidthOperator> SetCharWidthOperatorPtr;
			typedef Deferred<SetCacheDeviceOperator> SetCacheDeviceOperatorPtr;

			typedef Deferred<ColorSpaceStrokeOperator> ColorSpaceStrokeOperatorPtr;
			typedef Deferred<ColorSpaceNonstrokeOperator> ColorSpaceNonstrokeOperatorPtr;
			typedef Deferred<SetColorStrokeOperator> SetColorStrokeOperatorPtr;
			typedef Deferred<SetColorStrokeExtendedOperator> SetColorStrokeExtendedOperatorPtr;
			typedef Deferred<SetColorNonstrokeOperator> SetColorNonstrokeOperatorPtr;
			typedef Deferred<SetColorNonstrokeExtendedOperator> SetColorNonstrokeExtendedOperatorPtr;
			typedef Deferred<SetStrokingColorSpaceGrayOperator> SetStrokingColorSpaceGrayOperatorPtr;
			typedef Deferred<SetNonstrokingColorSpaceGrayOperator> SetNonstrokingColorSpaceGrayOperatorPtr;
			typedef Deferred<SetStrokingColorSpaceRGBOperator> SetStrokingColorSpaceRGBOperatorPtr;
			typedef Deferred<SetNonstrokingColorSpaceRGBOperator> SetNonstrokingColorSpaceRGBOperatorPtr;
			typedef Deferred<SetStrokingColorSpaceCMYKOperator> SetStrokingColorSpaceCMYKOperatorPtr;
			typedef Deferred<SetNonstrokingColorSpaceCMYKOperator> SetNonstrokingColorSpaceCMYKOperatorPtr;

			typedef Deferred<ShadingPaintOperator> ShadingPaintOperatorPtr;

			typedef Deferred<BeginInlineImageObjectOperator> BeginInlineImageObjectOperatorPtr;
			typedef Deferred<BeginInlineImageDataOperator> BeginInlineImageDataOperatorPtr;
			typedef Deferred<EndInlineImageObjectOperator> EndInlineImageObjectOperatorPtr;

			typedef Deferred<InvokeXObjectOperator> InvokeXObjectOperatorPtr;

			typedef Deferred<DefineMarkedContentPointOperator> DefineMarkedContentPointOperatorPtr;
			typedef Deferred<DefineMarkedContentPointWithPropertyListOperator> DefineMarkedContentPointWithPropertyListOperatorPtr;
			typedef Deferred<BeginMarkedContentSequenceOperator> BeginMarkedContentSequenceOperatorPtr;
			typedef Deferred<BeginMarkedContentSequenceWithPropertyListOperator> BeginMarkedContentSequenceWithPropertyListOperatorPtr;
			typedef Deferred<EndMarkedContentSequenceOperator> EndMarkedContentSequenceOperatorPtr;

			typedef Deferred<BeginCompatibilitySectionOperator> BeginCompatibilitySectionOperatorPtr;
			typedef Deferred<EndCompatibilitySectionOperator> EndCompatibilitySectionOperatorPtr;
		}
	}
}

#endif /* _SYNTAX_FWD_H */
