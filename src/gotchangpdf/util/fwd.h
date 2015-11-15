#ifndef _FWD_H
#define _FWD_H

namespace gotchangpdf
{
	template <typename T>
	struct Deferred;

	template <typename T>
	struct DeferredContainer;

	template <typename T>
	struct DeferredIterator;

	template <typename T>
	class SmartPtr;

	template <typename T>
	class ArrayObject;

	class MixedArrayObject;
	class BooleanObject;
	class DictionaryObject;
	class FunctionObject;
	class IndirectObjectReference;
	class IntegerObject;
	class NameObject;
	class NullObject;
	class NumericObject;
	class Object;
	class RealObject;
	class StreamObject;

	class StringObject;
	class HexadecimalStringObject;
	class LiteralStringObject;

	class Date;
	class NameTree;
	class NumberTree;
	class Rectangle;
	class Tree;

	class Buffer;

	class Containable;

	namespace documents
	{
		class Document;
		class Catalog;
		class PageNodeBase;
		class PageTree;
		class PageTreeNode;
		class PageObject;

		class ResourceDictionary;
		class ContentStream;
		class Contents;

		class Rectangle;
	}

	namespace exceptions
	{
		class Exception;
		class InvalidObjectTypeException;
	}

	namespace files
	{
		class File;
		class Header;

		class XrefBase;
		class XrefSubsection;
		class XrefTable;
		class XrefStream;
		class XrefChain;

		class XrefEntryBase;
		class XrefNullEntry;
		class XrefUsedEntry;
		class XrefFreeEntry;
		class XrefCompressedEntry;
	}

	namespace lexical
	{
		class SpiritParser;
		struct ObjectStreamHeader;

		class EmptyOperator;

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

		typedef Deferred<EmptyOperator> EmptyOperatorPtr;

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

	namespace filters
	{
		class Filter;
		class FlateDecodeFilter;
	}

	namespace raw
	{
		class BaseStream;
		class Stream;
		class ReverseStream;
	}

	using FlateDecodeFilterPtr = Deferred<filters::FlateDecodeFilter>;

	using BufferPtr = DeferredContainer<Buffer>;

	template <typename T>
	using ArrayObjectPtr = Deferred<ArrayObject<T>>;

	using DictionaryObjectPtr = Deferred<DictionaryObject>;
	using MixedArrayObjectPtr = Deferred<MixedArrayObject>;

	using NameObjectPtr = Deferred<NameObject>;
	using BooleanObjectPtr = Deferred<BooleanObject>;
	using FunctionObjectPtr = Deferred<FunctionObject>;
	using IndirectObjectReferencePtr = Deferred<IndirectObjectReference>;
	using IntegerObjectPtr = Deferred<IntegerObject>;
	using NullObjectPtr = Deferred<NullObject>;
	using RealObjectPtr = Deferred<RealObject>;
	using StreamObjectPtr = Deferred<StreamObject>;
	using LiteralStringObjectPtr = Deferred<LiteralStringObject>;
	using HexadecimalStringObjectPtr = Deferred<HexadecimalStringObject>;

	using XrefChainPtr = Deferred<files::XrefChain>;
	using XrefEntryBasePtr = Deferred<files::XrefEntryBase>;
	using XrefNullEntryPtr = Deferred<files::XrefNullEntry>;
	using XrefFreeEntryPtr = Deferred<files::XrefFreeEntry>;
	using XrefUsedEntryPtr = Deferred<files::XrefUsedEntry>;
	using XrefCompressedEntryPtr = Deferred<files::XrefCompressedEntry>;

	using HeaderPtr = Deferred<files::Header>;

	using XrefSubsectionPtr = Deferred<files::XrefSubsection>;
	using XrefTablePtr = Deferred<files::XrefTable>;
	using XrefStreamPtr = Deferred<files::XrefStream>;

	using PageTreePtr = Deferred<documents::PageTree>;
	using PageTreeNodePtr = Deferred<documents::PageTreeNode>;
	using PageObjectPtr = Deferred<documents::PageObject>;
	using ResourceDictionaryPtr = Deferred<documents::ResourceDictionary>;
	using ContentStreamPtr = Deferred<documents::ContentStream>;
	using RectanglePtr = Deferred<documents::Rectangle>;
	using CatalogPtr = Deferred<documents::Catalog>;

	using ContentsPtr = Deferred<documents::Contents>;
}

#endif /* _FWD_H */
