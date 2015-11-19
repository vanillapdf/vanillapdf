#ifndef _CONTENT_STREAM_OPERATIONS_H
#define _CONTENT_STREAM_OPERATIONS_H

#include "fwd.h"
#include "unknown_interface.h"

#include <vector>
#include <boost/variant/variant.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		class OperatorBase : public IUnknown
		{
		public:
			enum class Type
			{
				Unknown = 0,
				LineWidth,
				LineCap,
				LineJoin,
				MiterLimit,
				DashPattern,
				ColorRenderingIntent,
				Flatness,
				GraphicsState,
				SaveGraphicsState,
				RestoreGraphicsState,
				TransformationMatrix,
				BeginSubpath,
				Line,
				FullCurve,
				FinalCurve,
				InitialCurve,
				CloseSubpath,
				Rectangle,
				Stroke,
				CloseAndStroke,
				FillPathNonzero,
				FillPathCompatibility,
				FillPathEvenOdd,
				FillStrokeNonzero,
				FillStrokeEvenOdd,
				CloseFillStrokeNonzero,
				CloseFillStrokeEvenOdd,
				EndPath,
				ClipPathNonzero,
				ClipPathEvenOdd,
				BeginText,
				EndText,
				CharacterSpacing,
				WordSpacing,
				HorizontalScaling,
				Leading,
				TextFont,
				TextRenderingMode,
				TextRise,
				TextTranslate,
				TextTranslateLeading,
				TextMatrix,
				TextNextLine,
				TextShow,
				TextShowArray,
				TextNextLineShow,
				TextNextLineShowSpacing,
				SetCharWidth,
				SetCacheDevice,
				ColorSpaceStroke,
				ColorSpaceNonstroke,
				SetColorStroke,
				SetColorStrokeExtended,
				SetColorNonstroke,
				SetColorNonstrokeExtended,
				SetStrokingColorSpaceGray,
				SetNonstrokingColorSpaceGray,
				SetStrokingColorSpaceRGB,
				SetNonstrokingColorSpaceRGB,
				SetStrokingColorSpaceCMYK,
				SetNonstrokingColorSpaceCMYK,
				ShadingPaint,
				BeginInlineImageObject,
				BeginInlineImageData,
				EndInlineImageObject,
				InvokeXObject,
				DefineMarkedContentPoint,
				DefineMarkedContentPointWithPropertyList,
				BeginMarkedContentSequence,
				BeginMarkedContentSequenceWithPropertyList,
				EndMarkedContentSequence,
				BeginCompatibilitySection,
				EndCompatibilitySection
			};
		};

		class UnknownOperator : public OperatorBase {};

		// General graphics state
		class LineWidthOperator : public OperatorBase {};				//w
		class LineCapOperator : public OperatorBase {};					//J
		class LineJoinOperator : public OperatorBase {};				//j
		class MiterLimitOperator : public OperatorBase {};				//M
		class DashPatternOperator : public OperatorBase {};				//d
		class ColorRenderingIntentOperator : public OperatorBase {};	//ri
		class FlatnessOperator : public OperatorBase {};				//i
		class GraphicsStateOperator : public OperatorBase {};			//gs

		// Special graphics state
		class SaveGraphicsStateOperator : public OperatorBase {};		//q
		class RestoreGraphicsStateOperator : public OperatorBase {};	//Q
		class TransformationMatrixOperator : public OperatorBase {};	//cm

		// Path construction
		class BeginSubpathOperator : public OperatorBase {};		//m
		class LineOperator : public OperatorBase {};				//l
		class FullCurveOperator : public OperatorBase {};			//c
		class FinalCurveOperator : public OperatorBase {};			//v
		class InitialCurveOperator : public OperatorBase {};		//y
		class CloseSubpathOperator : public OperatorBase {};		//h
		class RectangleOperator : public OperatorBase {};			//re

		// Path painting
		class StrokeOperator : public OperatorBase {};					//S
		class CloseAndStrokeOperator : public OperatorBase {};			//s
		class FillPathNonzeroOperator : public OperatorBase {};			//f
		class FillPathCompatibilityOperator : public OperatorBase {};	//F
		class FillPathEvenOddOperator : public OperatorBase {};			//f*
		class FillStrokeNonzeroOperator : public OperatorBase {};		//B
		class FillStrokeEvenOddOperator : public OperatorBase {};		//B*
		class CloseFillStrokeNonzeroOperator : public OperatorBase {};	//b
		class CloseFillStrokeEvenOddOperator : public OperatorBase {};	//b*
		class EndPathOperator : public OperatorBase {};					//n

		// Clipping paths
		class ClipPathNonzeroOperator : public OperatorBase {};		//W
		class ClipPathEvenOddOperator : public OperatorBase {};		//W*

		// Text objects
		class BeginTextOperator : public OperatorBase {};			//BT
		class EndTextOperator : public OperatorBase {};				//ET

		// Text state
		class CharacterSpacingOperator : public OperatorBase {};	//Tc
		class WordSpacingOperator : public OperatorBase {};			//Tw
		class HorizontalScalingOperator : public OperatorBase {};	//Tz
		class LeadingOperator : public OperatorBase {};				//TL
		class TextFontOperator : public OperatorBase {};			//Tf
		class TextRenderingModeOperator : public OperatorBase {};	//Tr
		class TextRiseOperator : public OperatorBase {};			//Ts

		// Text positioning
		class TextTranslateOperator : public OperatorBase {};			//Td
		class TextTranslateLeadingOperator : public OperatorBase {};	//TD
		class TextMatrixOperator : public OperatorBase {};				//Tm
		class TextNextLineOperator : public OperatorBase {};			//T*

		// Text showing
		class TextShowOperator : public OperatorBase {};					//Tj
		class TextShowArrayOperator : public OperatorBase {};				//TJ
		class TextNextLineShowOperator : public OperatorBase {};			//'
		class TextNextLineShowSpacingOperator : public OperatorBase {};		//"

		// Type 3 fonts
		class SetCharWidthOperator : public OperatorBase {};		//d0
		class SetCacheDeviceOperator : public OperatorBase {};		//d1

		// Color
		class ColorSpaceStrokeOperator : public OperatorBase {};					//CS
		class ColorSpaceNonstrokeOperator : public OperatorBase {};					//cs
		class SetColorStrokeOperator : public OperatorBase {};						//SC
		class SetColorStrokeExtendedOperator : public OperatorBase {};				//SCN
		class SetColorNonstrokeOperator : public OperatorBase {};					//sc
		class SetColorNonstrokeExtendedOperator : public OperatorBase {};			//scn
		class SetStrokingColorSpaceGrayOperator : public OperatorBase {};			//G
		class SetNonstrokingColorSpaceGrayOperator : public OperatorBase {};		//g
		class SetStrokingColorSpaceRGBOperator : public OperatorBase {};			//RG
		class SetNonstrokingColorSpaceRGBOperator : public OperatorBase {};			//rg
		class SetStrokingColorSpaceCMYKOperator : public OperatorBase {};			//K
		class SetNonstrokingColorSpaceCMYKOperator : public OperatorBase {};		//k

		// Shading patterns
		class ShadingPaintOperator : public OperatorBase {};				//sh

		// Inline images
		class BeginInlineImageObjectOperator : public OperatorBase {};		//BI
		class BeginInlineImageDataOperator : public OperatorBase {};		//ID
		class EndInlineImageObjectOperator : public OperatorBase {};		//EI

		// XObjects
		class InvokeXObjectOperator : public OperatorBase {};								//Do

		// Marked content
		class DefineMarkedContentPointOperator : public OperatorBase {};					//MP
		class DefineMarkedContentPointWithPropertyListOperator : public OperatorBase {};	//DP
		class BeginMarkedContentSequenceOperator : public OperatorBase {};					//BMC
		class BeginMarkedContentSequenceWithPropertyListOperator : public OperatorBase {};	//BDC
		class EndMarkedContentSequenceOperator : public OperatorBase {};					//EMC

		// Compatibility
		class BeginCompatibilitySectionOperator : public OperatorBase {};					//BX
		class EndCompatibilitySectionOperator : public OperatorBase {};						//EX

		typedef boost::variant<
			EmptyOperatorPtr,
			LineWidthOperatorPtr,
			LineCapOperatorPtr,
			LineJoinOperatorPtr,
			MiterLimitOperatorPtr,
			DashPatternOperatorPtr,
			ColorRenderingIntentOperatorPtr,
			FlatnessOperatorPtr,
			GraphicsStateOperatorPtr,
			SaveGraphicsStateOperatorPtr,
			RestoreGraphicsStateOperatorPtr,
			TransformationMatrixOperatorPtr,
			BeginSubpathOperatorPtr,
			LineOperatorPtr,
			FullCurveOperatorPtr,
			FinalCurveOperatorPtr,
			InitialCurveOperatorPtr,
			CloseSubpathOperatorPtr,
			RectangleOperatorPtr,
			StrokeOperatorPtr,
			CloseAndStrokeOperatorPtr,
			FillPathNonzeroOperatorPtr,
			FillPathCompatibilityOperatorPtr,
			FillPathEvenOddOperatorPtr,
			FillStrokeNonzeroOperatorPtr,
			FillStrokeEvenOddOperatorPtr,
			CloseFillStrokeNonzeroOperatorPtr,
			CloseFillStrokeEvenOddOperatorPtr,
			EndPathOperatorPtr,
			ClipPathNonzeroOperatorPtr,
			ClipPathEvenOddOperatorPtr,
			BeginTextOperatorPtr,
			EndTextOperatorPtr,
			CharacterSpacingOperatorPtr,
			WordSpacingOperatorPtr,
			HorizontalScalingOperatorPtr,
			LeadingOperatorPtr,
			TextFontOperatorPtr,
			TextRenderingModeOperatorPtr,
			TextRiseOperatorPtr,
			TextTranslateOperatorPtr,
			TextTranslateLeadingOperatorPtr,
			TextMatrixOperatorPtr,
			TextNextLineOperatorPtr,
			TextShowOperatorPtr,
			TextShowArrayOperatorPtr,
			TextNextLineShowOperatorPtr,
			TextNextLineShowSpacingOperatorPtr,
			SetCharWidthOperatorPtr,
			SetCacheDeviceOperatorPtr,
			ColorSpaceStrokeOperatorPtr,
			ColorSpaceNonstrokeOperatorPtr,
			SetColorStrokeOperatorPtr,
			SetColorStrokeExtendedOperatorPtr,
			SetColorNonstrokeOperatorPtr,
			SetColorNonstrokeExtendedOperatorPtr,
			SetStrokingColorSpaceGrayOperatorPtr,
			SetNonstrokingColorSpaceGrayOperatorPtr,
			SetStrokingColorSpaceRGBOperatorPtr,
			SetNonstrokingColorSpaceRGBOperatorPtr,
			SetStrokingColorSpaceCMYKOperatorPtr,
			SetNonstrokingColorSpaceCMYKOperatorPtr,
			ShadingPaintOperatorPtr,
			BeginInlineImageObjectOperatorPtr,
			BeginInlineImageDataOperatorPtr,
			EndInlineImageObjectOperatorPtr,
			InvokeXObjectOperatorPtr,
			DefineMarkedContentPointOperatorPtr,
			DefineMarkedContentPointWithPropertyListOperatorPtr,
			BeginMarkedContentSequenceOperatorPtr,
			BeginMarkedContentSequenceWithPropertyListOperatorPtr,
			EndMarkedContentSequenceOperatorPtr,
			BeginCompatibilitySectionOperatorPtr,
			EndCompatibilitySectionOperatorPtr
		> ContentStreamOperator;

		typedef boost::variant<
			LineWidthOperatorPtr,
			LineCapOperatorPtr,
			LineJoinOperatorPtr,
			MiterLimitOperatorPtr,
			DashPatternOperatorPtr,
			ColorRenderingIntentOperatorPtr,
			FlatnessOperatorPtr,
			GraphicsStateOperatorPtr
		> GeneralGraphicsStateOperators;

		typedef boost::variant<
			SaveGraphicsStateOperatorPtr,
			RestoreGraphicsStateOperatorPtr,
			TransformationMatrixOperatorPtr
		> SpecialGraphicsStateOperators;

		typedef boost::variant<
			BeginSubpathOperatorPtr,
			LineOperatorPtr,
			FullCurveOperatorPtr,
			FinalCurveOperatorPtr,
			InitialCurveOperatorPtr,
			CloseSubpathOperatorPtr,
			RectangleOperatorPtr
		> PathConstructionOperators;

		typedef boost::variant<
			StrokeOperatorPtr,
			CloseAndStrokeOperatorPtr,
			FillPathNonzeroOperatorPtr,
			FillPathCompatibilityOperatorPtr,
			FillPathEvenOddOperatorPtr,
			FillStrokeNonzeroOperatorPtr,
			FillStrokeEvenOddOperatorPtr,
			CloseFillStrokeNonzeroOperatorPtr,
			CloseFillStrokeEvenOddOperatorPtr,
			EndPathOperatorPtr
		> PathPaintingOperators;

		typedef boost::variant<
			ClipPathNonzeroOperatorPtr,
			ClipPathEvenOddOperatorPtr
		> ClippingPathOperators;

		typedef boost::variant<
			BeginTextOperatorPtr,
			EndTextOperatorPtr
		> TextObjectOperators;

		typedef boost::variant<
			CharacterSpacingOperatorPtr,
			WordSpacingOperatorPtr,
			HorizontalScalingOperatorPtr,
			LeadingOperatorPtr,
			TextFontOperatorPtr,
			TextRenderingModeOperatorPtr,
			TextRiseOperatorPtr
		> TextStateOperators;

		typedef boost::variant<
			TextTranslateOperatorPtr,
			TextTranslateLeadingOperatorPtr,
			TextMatrixOperatorPtr,
			TextNextLineOperatorPtr
		> TextPositioningOperators;

		typedef boost::variant<
			TextShowOperatorPtr,
			TextShowArrayOperatorPtr,
			TextNextLineShowOperatorPtr,
			TextNextLineShowSpacingOperatorPtr
		> TextShowingOperators;

		typedef boost::variant<
			SetCharWidthOperatorPtr,
			SetCacheDeviceOperatorPtr
		> Type3FontOperators;

		typedef boost::variant<
			ColorSpaceStrokeOperatorPtr,
			ColorSpaceNonstrokeOperatorPtr,
			SetColorStrokeOperatorPtr,
			SetColorStrokeExtendedOperatorPtr,
			SetColorNonstrokeOperatorPtr,
			SetColorNonstrokeExtendedOperatorPtr,
			SetStrokingColorSpaceGrayOperatorPtr,
			SetNonstrokingColorSpaceGrayOperatorPtr,
			SetStrokingColorSpaceRGBOperatorPtr,
			SetNonstrokingColorSpaceRGBOperatorPtr,
			SetStrokingColorSpaceCMYKOperatorPtr,
			SetNonstrokingColorSpaceCMYKOperatorPtr
		> ColorOperators;

		typedef boost::variant<
			ShadingPaintOperatorPtr
		> ShadingPatternOperators;

		typedef boost::variant<
			BeginInlineImageObjectOperatorPtr,
			BeginInlineImageDataOperatorPtr,
			EndInlineImageObjectOperatorPtr
		> InlineImageOperators;

		typedef boost::variant<
			InvokeXObjectOperatorPtr
		> XObjectOperators;

		typedef boost::variant<
			DefineMarkedContentPointOperatorPtr,
			DefineMarkedContentPointWithPropertyListOperatorPtr,
			BeginMarkedContentSequenceOperatorPtr,
			BeginMarkedContentSequenceWithPropertyListOperatorPtr,
			EndMarkedContentSequenceOperatorPtr
		> MarkedContentOperators;

		typedef boost::variant<
			BeginCompatibilitySectionOperatorPtr,
			EndCompatibilitySectionOperatorPtr
		> CompatibilityOperators;

		template <typename T>
		class IsContentStreamOperatorTypeVisitor : public boost::static_visitor<bool>
		{
		public:
			inline bool operator()(const T&) const { return true; }

			template <typename U>
			inline bool operator()(const U&) const { return false; }
		};

		class ContentStreamOperatorTypeVisitor : public boost::static_visitor<OperatorBase::Type>
		{
		public:
			inline OperatorBase::Type operator()(const UnknownOperatorPtr&) const { return OperatorBase::Type::Unknown; }
			inline OperatorBase::Type operator()(const LineWidthOperatorPtr&) const { return OperatorBase::Type::LineWidth; }
			inline OperatorBase::Type operator()(const LineCapOperatorPtr&) const { return OperatorBase::Type::LineCap; }
			inline OperatorBase::Type operator()(const LineJoinOperatorPtr&) const { return OperatorBase::Type::LineJoin; }
			inline OperatorBase::Type operator()(const MiterLimitOperatorPtr&) const { return OperatorBase::Type::MiterLimit; }
			inline OperatorBase::Type operator()(const DashPatternOperatorPtr&) const { return OperatorBase::Type::DashPattern; }
			inline OperatorBase::Type operator()(const ColorRenderingIntentOperatorPtr&) const { return OperatorBase::Type::ColorRenderingIntent; }
			inline OperatorBase::Type operator()(const FlatnessOperatorPtr&) const { return OperatorBase::Type::Flatness; }
			inline OperatorBase::Type operator()(const GraphicsStateOperatorPtr&) const { return OperatorBase::Type::GraphicsState; }
			inline OperatorBase::Type operator()(const SaveGraphicsStateOperatorPtr&) const { return OperatorBase::Type::SaveGraphicsState; }
			inline OperatorBase::Type operator()(const RestoreGraphicsStateOperatorPtr&) const { return OperatorBase::Type::RestoreGraphicsState; }
			inline OperatorBase::Type operator()(const TransformationMatrixOperatorPtr&) const { return OperatorBase::Type::TransformationMatrix; }
			inline OperatorBase::Type operator()(const BeginSubpathOperatorPtr&) const { return OperatorBase::Type::BeginSubpath; }
			inline OperatorBase::Type operator()(const LineOperatorPtr&) const { return OperatorBase::Type::Line; }
			inline OperatorBase::Type operator()(const FullCurveOperatorPtr&) const { return OperatorBase::Type::FullCurve; }
			inline OperatorBase::Type operator()(const FinalCurveOperatorPtr&) const { return OperatorBase::Type::FinalCurve; }
			inline OperatorBase::Type operator()(const InitialCurveOperatorPtr&) const { return OperatorBase::Type::InitialCurve; }
			inline OperatorBase::Type operator()(const CloseSubpathOperatorPtr&) const { return OperatorBase::Type::CloseSubpath; }
			inline OperatorBase::Type operator()(const RectangleOperatorPtr&) const { return OperatorBase::Type::Rectangle; }
			inline OperatorBase::Type operator()(const StrokeOperatorPtr&) const { return OperatorBase::Type::Stroke; }
			inline OperatorBase::Type operator()(const CloseAndStrokeOperatorPtr&) const { return OperatorBase::Type::CloseAndStroke; }
			inline OperatorBase::Type operator()(const FillPathNonzeroOperatorPtr&) const { return OperatorBase::Type::FillPathNonzero; }
			inline OperatorBase::Type operator()(const FillPathCompatibilityOperatorPtr&) const { return OperatorBase::Type::FillPathCompatibility; }
			inline OperatorBase::Type operator()(const FillPathEvenOddOperatorPtr&) const { return OperatorBase::Type::FillPathEvenOdd; }
			inline OperatorBase::Type operator()(const FillStrokeNonzeroOperatorPtr&) const { return OperatorBase::Type::FillStrokeNonzero; }
			inline OperatorBase::Type operator()(const FillStrokeEvenOddOperatorPtr&) const { return OperatorBase::Type::FillStrokeEvenOdd; }
			inline OperatorBase::Type operator()(const CloseFillStrokeNonzeroOperatorPtr&) const { return OperatorBase::Type::CloseFillStrokeNonzero; }
			inline OperatorBase::Type operator()(const CloseFillStrokeEvenOddOperatorPtr&) const { return OperatorBase::Type::CloseFillStrokeEvenOdd; }
			inline OperatorBase::Type operator()(const EndPathOperatorPtr&) const { return OperatorBase::Type::EndPath; }
			inline OperatorBase::Type operator()(const ClipPathNonzeroOperatorPtr&) const { return OperatorBase::Type::ClipPathNonzero; }
			inline OperatorBase::Type operator()(const ClipPathEvenOddOperatorPtr&) const { return OperatorBase::Type::ClipPathEvenOdd; }
			inline OperatorBase::Type operator()(const BeginTextOperatorPtr&) const { return OperatorBase::Type::BeginText; }
			inline OperatorBase::Type operator()(const EndTextOperatorPtr&) const { return OperatorBase::Type::EndText; }
			inline OperatorBase::Type operator()(const CharacterSpacingOperatorPtr&) const { return OperatorBase::Type::CharacterSpacing; }
			inline OperatorBase::Type operator()(const WordSpacingOperatorPtr&) const { return OperatorBase::Type::WordSpacing; }
			inline OperatorBase::Type operator()(const HorizontalScalingOperatorPtr&) const { return OperatorBase::Type::HorizontalScaling; }
			inline OperatorBase::Type operator()(const LeadingOperatorPtr&) const { return OperatorBase::Type::Leading; }
			inline OperatorBase::Type operator()(const TextFontOperatorPtr&) const { return OperatorBase::Type::TextFont; }
			inline OperatorBase::Type operator()(const TextRenderingModeOperatorPtr&) const { return OperatorBase::Type::TextRenderingMode; }
			inline OperatorBase::Type operator()(const TextRiseOperatorPtr&) const { return OperatorBase::Type::TextRise; }
			inline OperatorBase::Type operator()(const TextTranslateOperatorPtr&) const { return OperatorBase::Type::TextTranslate; }
			inline OperatorBase::Type operator()(const TextTranslateLeadingOperatorPtr&) const { return OperatorBase::Type::TextTranslateLeading; }
			inline OperatorBase::Type operator()(const TextMatrixOperatorPtr&) const { return OperatorBase::Type::TextMatrix; }
			inline OperatorBase::Type operator()(const TextNextLineOperatorPtr&) const { return OperatorBase::Type::TextNextLine; }
			inline OperatorBase::Type operator()(const TextShowOperatorPtr&) const { return OperatorBase::Type::TextShow; }
			inline OperatorBase::Type operator()(const TextShowArrayOperatorPtr&) const { return OperatorBase::Type::TextShowArray; }
			inline OperatorBase::Type operator()(const TextNextLineShowOperatorPtr&) const { return OperatorBase::Type::TextNextLineShow; }
			inline OperatorBase::Type operator()(const TextNextLineShowSpacingOperatorPtr&) const { return OperatorBase::Type::TextNextLineShowSpacing; }
			inline OperatorBase::Type operator()(const SetCharWidthOperatorPtr&) const { return OperatorBase::Type::SetCharWidth; }
			inline OperatorBase::Type operator()(const SetCacheDeviceOperatorPtr&) const { return OperatorBase::Type::SetCacheDevice; }
			inline OperatorBase::Type operator()(const ColorSpaceStrokeOperatorPtr&) const { return OperatorBase::Type::ColorSpaceStroke; }
			inline OperatorBase::Type operator()(const ColorSpaceNonstrokeOperatorPtr&) const { return OperatorBase::Type::ColorSpaceNonstroke; }
			inline OperatorBase::Type operator()(const SetColorStrokeOperatorPtr&) const { return OperatorBase::Type::SetColorStroke; }
			inline OperatorBase::Type operator()(const SetColorStrokeExtendedOperatorPtr&) const { return OperatorBase::Type::SetColorStrokeExtended; }
			inline OperatorBase::Type operator()(const SetColorNonstrokeOperatorPtr&) const { return OperatorBase::Type::SetColorNonstroke; }
			inline OperatorBase::Type operator()(const SetColorNonstrokeExtendedOperatorPtr&) const { return OperatorBase::Type::SetColorNonstrokeExtended; }
			inline OperatorBase::Type operator()(const SetStrokingColorSpaceGrayOperatorPtr&) const { return OperatorBase::Type::SetStrokingColorSpaceGray; }
			inline OperatorBase::Type operator()(const SetNonstrokingColorSpaceGrayOperatorPtr&) const { return OperatorBase::Type::SetNonstrokingColorSpaceGray; }
			inline OperatorBase::Type operator()(const SetStrokingColorSpaceRGBOperatorPtr&) const { return OperatorBase::Type::SetStrokingColorSpaceRGB; }
			inline OperatorBase::Type operator()(const SetNonstrokingColorSpaceRGBOperatorPtr&) const { return OperatorBase::Type::SetNonstrokingColorSpaceRGB; }
			inline OperatorBase::Type operator()(const SetStrokingColorSpaceCMYKOperatorPtr&) const { return OperatorBase::Type::SetStrokingColorSpaceCMYK; }
			inline OperatorBase::Type operator()(const SetNonstrokingColorSpaceCMYKOperatorPtr&) const { return OperatorBase::Type::SetNonstrokingColorSpaceCMYK; }
			inline OperatorBase::Type operator()(const ShadingPaintOperatorPtr&) const { return OperatorBase::Type::ShadingPaint; }
			inline OperatorBase::Type operator()(const BeginInlineImageObjectOperatorPtr&) const { return OperatorBase::Type::BeginInlineImageObject; }
			inline OperatorBase::Type operator()(const BeginInlineImageDataOperatorPtr&) const { return OperatorBase::Type::BeginInlineImageData; }
			inline OperatorBase::Type operator()(const EndInlineImageObjectOperatorPtr&) const { return OperatorBase::Type::EndInlineImageObject; }
			inline OperatorBase::Type operator()(const InvokeXObjectOperatorPtr&) const { return OperatorBase::Type::InvokeXObject; }
			inline OperatorBase::Type operator()(const DefineMarkedContentPointOperatorPtr&) const { return OperatorBase::Type::DefineMarkedContentPoint; }
			inline OperatorBase::Type operator()(const DefineMarkedContentPointWithPropertyListOperatorPtr&) const { return OperatorBase::Type::DefineMarkedContentPointWithPropertyList; }
			inline OperatorBase::Type operator()(const BeginMarkedContentSequenceOperatorPtr&) const { return OperatorBase::Type::BeginMarkedContentSequence; }
			inline OperatorBase::Type operator()(const BeginMarkedContentSequenceWithPropertyListOperatorPtr&) const { return OperatorBase::Type::BeginMarkedContentSequenceWithPropertyList; }
			inline OperatorBase::Type operator()(const EndMarkedContentSequenceOperatorPtr&) const { return OperatorBase::Type::EndMarkedContentSequence; }
			inline OperatorBase::Type operator()(const BeginCompatibilitySectionOperatorPtr&) const { return OperatorBase::Type::BeginCompatibilitySection; }
			inline OperatorBase::Type operator()(const EndCompatibilitySectionOperatorPtr&) const { return OperatorBase::Type::EndCompatibilitySection; }
		};

		typedef boost::variant <
			// first is null object
			NullObjectPtr,
			MixedArrayObjectPtr,
			NameObjectPtr,
			DictionaryObjectPtr,
			FunctionObjectPtr,
			BooleanObjectPtr,
			IntegerObjectPtr,
			RealObjectPtr,
			LiteralStringObjectPtr,
			HexadecimalStringObjectPtr
		> ContentStreamOperand;

		typedef std::pair<std::vector<ContentStreamOperand>, ContentStreamOperator> ContentStreamOperation;
		typedef std::vector<ContentStreamOperation> ContentStreamOperationCollection;
	}
}

#endif /* _CONTENT_STREAM_OPERATIONS_H */
