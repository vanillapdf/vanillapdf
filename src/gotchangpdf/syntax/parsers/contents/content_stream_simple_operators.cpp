#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			namespace phoenix = boost::phoenix;

				void convert_Unknown(UnknownOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_LineWidth(LineWidthOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_LineCap(LineCapOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_LineJoin(LineJoinOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_MiterLimit(MiterLimitOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_DashPattern(DashPatternOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_ColorRenderingIntent(ColorRenderingIntentOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_Flatness(FlatnessOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_GraphicsState(GraphicsStateOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SaveGraphicsState(SaveGraphicsStateOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_RestoreGraphicsState(RestoreGraphicsStateOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TransformationMatrix(TransformationMatrixOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_BeginSubpath(BeginSubpathOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_Line(LineOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_FullCurve(FullCurveOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_FinalCurve(FinalCurveOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_InitialCurve(InitialCurveOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_CloseSubpath(CloseSubpathOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_Rectangle(RectangleOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_Stroke(StrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_CloseAndStroke(CloseAndStrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_FillPathNonzero(FillPathNonzeroOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_FillPathCompatibility(FillPathCompatibilityOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_FillPathEvenOdd(FillPathEvenOddOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_FillStrokeNonzero(FillStrokeNonzeroOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_FillStrokeEvenOdd(FillStrokeEvenOddOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_CloseFillStrokeNonzero(CloseFillStrokeNonzeroOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_CloseFillStrokeEvenOdd(CloseFillStrokeEvenOddOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_EndPath(EndPathOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_ClipPathNonzero(ClipPathNonzeroOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_ClipPathEvenOdd(ClipPathEvenOddOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_BeginText(BeginTextOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_EndText(EndTextOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_CharacterSpacing(CharacterSpacingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_WordSpacing(WordSpacingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_HorizontalScaling(HorizontalScalingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_Leading(LeadingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TextFont(TextFontOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TextRenderingMode(TextRenderingModeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TextRise(TextRiseOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TextTranslate(TextTranslateOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TextTranslateLeading(TextTranslateLeadingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TextMatrix(TextMatrixOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TextNextLine(TextNextLineOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TextShow(TextShowOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TextShowArray(TextShowArrayOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TextNextLineShow(TextNextLineShowOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_TextNextLineShowSpacing(TextNextLineShowSpacingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetCharWidth(SetCharWidthOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetCacheDevice(SetCacheDeviceOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_ColorSpaceStroke(ColorSpaceStrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_ColorSpaceNonstroke(ColorSpaceNonstrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetColorStroke(SetColorStrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetColorStrokeExtended(SetColorStrokeExtendedOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetColorNonstroke(SetColorNonstrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetColorNonstrokeExtended(SetColorNonstrokeExtendedOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetStrokingColorSpaceGray(SetStrokingColorSpaceGrayOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetNonstrokingColorSpaceGray(SetNonstrokingColorSpaceGrayOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetStrokingColorSpaceRGB(SetStrokingColorSpaceRGBOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetNonstrokingColorSpaceRGB(SetNonstrokingColorSpaceRGBOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetStrokingColorSpaceCMYK(SetStrokingColorSpaceCMYKOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_SetNonstrokingColorSpaceCMYK(SetNonstrokingColorSpaceCMYKOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_ShadingPaint(ShadingPaintOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_BeginInlineImageObject(BeginInlineImageObjectOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_BeginInlineImageData(BeginInlineImageDataOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_EndInlineImageObject(EndInlineImageObjectOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_InvokeXObject(InvokeXObjectOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_DefineMarkedContentPoint(DefineMarkedContentPointOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_DefineMarkedContentPointWithPropertyList(DefineMarkedContentPointWithPropertyListOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_BeginMarkedContentSequence(BeginMarkedContentSequenceOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_BeginMarkedContentSequenceWithPropertyList(BeginMarkedContentSequenceWithPropertyListOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_EndMarkedContentSequence(EndMarkedContentSequenceOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_BeginCompatibilitySection(BeginCompatibilitySectionOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
				void convert_EndCompatibilitySection(EndCompatibilitySectionOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			ColorGrammar::ColorGrammar() :
				base_type(start, "Color grammar")
			{
				start %=
					color_space_stroke[phoenix::bind(&convert_ColorSpaceStroke, qi::_1, qi::_val)]
					| color_space_nstroke[phoenix::bind(&convert_ColorSpaceNonstroke, qi::_1, qi::_val)]
					| sc_stroke[phoenix::bind(&convert_SetColorStroke, qi::_1, qi::_val)]
					| sc_extended_stroke[phoenix::bind(&convert_SetColorStrokeExtended, qi::_1, qi::_val)]
					| sc_nstroke[phoenix::bind(&convert_SetColorNonstroke, qi::_1, qi::_val)]
					| sc_extended_nstroke[phoenix::bind(&convert_SetColorNonstrokeExtended, qi::_1, qi::_val)]
					| sgray[phoenix::bind(&convert_SetStrokingColorSpaceGray, qi::_1, qi::_val)]
					| nsgray[phoenix::bind(&convert_SetNonstrokingColorSpaceGray, qi::_1, qi::_val)]
					| srgb[phoenix::bind(&convert_SetStrokingColorSpaceRGB, qi::_1, qi::_val)]
					| nsrgb[phoenix::bind(&convert_SetNonstrokingColorSpaceRGB, qi::_1, qi::_val)]
					| scmyk[phoenix::bind(&convert_SetStrokingColorSpaceCMYK, qi::_1, qi::_val)]
					| nscmyk[phoenix::bind(&convert_SetNonstrokingColorSpaceCMYK, qi::_1, qi::_val)];

				color_space_stroke %=
					qi::eps
					>> qi::lit("CS");

				color_space_nstroke %=
					qi::eps
					>> qi::lit("cs");

				sc_stroke %=
					qi::eps
					>> qi::lit("SC");

				sc_extended_stroke %=
					qi::eps
					>> qi::lit("SCN");

				sc_nstroke %=
					qi::eps
					>> qi::lit("sc");

				sc_extended_nstroke %=
					qi::eps
					>> qi::lit("scn");

				sgray %=
					qi::eps
					>> qi::lit("G");

				nsgray %=
					qi::eps
					>> qi::lit("g");

				srgb %=
					qi::eps
					>> qi::lit("RG");

				nsrgb %=
					qi::eps
					>> qi::lit("rg");

				scmyk %=
					qi::eps
					>> qi::lit("K");

				nscmyk %=
					qi::eps
					>> qi::lit("k");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(color_space_stroke);
				BOOST_SPIRIT_DEBUG_NODE(color_space_nstroke);
				BOOST_SPIRIT_DEBUG_NODE(sc_stroke);
				BOOST_SPIRIT_DEBUG_NODE(sc_extended_stroke);
				BOOST_SPIRIT_DEBUG_NODE(sc_nstroke);
				BOOST_SPIRIT_DEBUG_NODE(sc_extended_nstroke);
				BOOST_SPIRIT_DEBUG_NODE(sgray);
				BOOST_SPIRIT_DEBUG_NODE(nsgray);
				BOOST_SPIRIT_DEBUG_NODE(srgb);
				BOOST_SPIRIT_DEBUG_NODE(nsrgb);
				BOOST_SPIRIT_DEBUG_NODE(scmyk);
				BOOST_SPIRIT_DEBUG_NODE(nscmyk);
			}

			ClippingPathGrammar::ClippingPathGrammar() :
				base_type(start, "Clipping path grammar")
			{
				start %=
					non_zero[phoenix::bind(&convert_ClipPathNonzero, qi::_1, qi::_val)]
					| even_odd[phoenix::bind(&convert_ClipPathEvenOdd, qi::_1, qi::_val)];

				non_zero %=
					qi::eps
					>> qi::lit("W");

				even_odd %=
					qi::eps
					>> qi::lit("W*");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(non_zero);
				BOOST_SPIRIT_DEBUG_NODE(even_odd);
			}

			CompatibilityGrammar::CompatibilityGrammar() :
				base_type(start, "Compatibility grammar")
			{
				start %=
					// TODO: enter compatibility section - suppress errors from unknown operators
					begin_compatibility[phoenix::bind(&convert_BeginCompatibilitySection, qi::_1, qi::_val)]
					| end_compatibility[phoenix::bind(&convert_EndCompatibilitySection, qi::_1, qi::_val)];

				begin_compatibility %=
					qi::eps
					>> qi::lit("BX");

				end_compatibility %=
					qi::eps
					>> qi::lit("EX");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(begin_compatibility);
				BOOST_SPIRIT_DEBUG_NODE(end_compatibility);
			}

			GeneralGraphicsStateGrammar::GeneralGraphicsStateGrammar() :
				base_type(start, "General graphics state grammar")
			{
				start %=
					line_width[phoenix::bind(&convert_LineWidth, qi::_1, qi::_val)]
					| line_cap[phoenix::bind(&convert_LineCap, qi::_1, qi::_val)]
					| line_join[phoenix::bind(&convert_LineJoin, qi::_1, qi::_val)]
					| miter_limit[phoenix::bind(&convert_MiterLimit, qi::_1, qi::_val)]
					| dash_pattern[phoenix::bind(&convert_DashPattern, qi::_1, qi::_val)]
					| color_rendering_intent[phoenix::bind(&convert_ColorRenderingIntent, qi::_1, qi::_val)]
					| flatness[phoenix::bind(&convert_Flatness, qi::_1, qi::_val)]
					| graphics_state[phoenix::bind(&convert_GraphicsState, qi::_1, qi::_val)];

				line_width %=
					qi::eps
					>> qi::lit("w");

				line_cap %=
					qi::eps
					>> qi::lit("J");

				line_join %=
					qi::eps
					>> qi::lit("j");

				miter_limit %=
					qi::eps
					>> qi::lit("M");

				dash_pattern %=
					qi::eps
					>> qi::lit("d");

				color_rendering_intent %=
					qi::eps
					>> qi::lit("ri");

				flatness %=
					qi::eps
					>> qi::lit("i");

				graphics_state %=
					qi::eps
					>> qi::lit("gs");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(line_width);
				BOOST_SPIRIT_DEBUG_NODE(line_cap);
				BOOST_SPIRIT_DEBUG_NODE(line_join);
				BOOST_SPIRIT_DEBUG_NODE(miter_limit);
				BOOST_SPIRIT_DEBUG_NODE(dash_pattern);
				BOOST_SPIRIT_DEBUG_NODE(color_rendering_intent);
				BOOST_SPIRIT_DEBUG_NODE(flatness);
				BOOST_SPIRIT_DEBUG_NODE(graphics_state);
			}

			InlineImageGrammar::InlineImageGrammar() :
				base_type(start, "Inline image grammar")
			{
				start %=
					begin_image[phoenix::bind(&convert_BeginInlineImageObject, qi::_1, qi::_val)]
					| begin_data[phoenix::bind(&convert_BeginInlineImageData, qi::_1, qi::_val)]
					| end_image[phoenix::bind(&convert_EndInlineImageObject, qi::_1, qi::_val)];

				begin_image %=
					qi::eps
					>> qi::lit("BI");

				begin_data %=
					qi::eps
					>> qi::lit("ID");

				end_image %=
					qi::eps
					>> qi::lit("EI");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(begin_image);
				BOOST_SPIRIT_DEBUG_NODE(begin_data);
				BOOST_SPIRIT_DEBUG_NODE(end_image);
			}

			MarkedContentGrammar::MarkedContentGrammar() :
				base_type(start, "Marked content grammar")
			{
				start %=
					define[phoenix::bind(&convert_DefineMarkedContentPoint, qi::_1, qi::_val)]
					| define_prop[phoenix::bind(&convert_DefineMarkedContentPointWithPropertyList, qi::_1, qi::_val)]
					| begin[phoenix::bind(&convert_BeginMarkedContentSequence, qi::_1, qi::_val)]
					| begin_prop[phoenix::bind(&convert_BeginMarkedContentSequenceWithPropertyList, qi::_1, qi::_val)]
					| end[phoenix::bind(&convert_EndMarkedContentSequence, qi::_1, qi::_val)];

				define %=
					qi::eps
					>> qi::lit("MP");

				define_prop %=
					qi::eps
					>> qi::lit("DP");

				begin %=
					qi::eps
					>> qi::lit("BMC");

				begin_prop %=
					qi::eps
					>> qi::lit("BDC");

				end %=
					qi::eps
					>> qi::lit("EMC");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(define);
				BOOST_SPIRIT_DEBUG_NODE(define_prop);
				BOOST_SPIRIT_DEBUG_NODE(begin);
				BOOST_SPIRIT_DEBUG_NODE(begin_prop);
				BOOST_SPIRIT_DEBUG_NODE(end);
			}

			PathConstructionGrammar::PathConstructionGrammar() :
				base_type(start, "Path construction grammar")
			{
				start %=
					begin[phoenix::bind(&convert_BeginSubpath, qi::_1, qi::_val)]
					| line[phoenix::bind(&convert_Line, qi::_1, qi::_val)]
					| full_curve[phoenix::bind(&convert_FullCurve, qi::_1, qi::_val)]
					| final_curve[phoenix::bind(&convert_FinalCurve, qi::_1, qi::_val)]
					| initial_curve[phoenix::bind(&convert_InitialCurve, qi::_1, qi::_val)]
					| close[phoenix::bind(&convert_CloseSubpath, qi::_1, qi::_val)]
					| rectangle[phoenix::bind(&convert_Rectangle, qi::_1, qi::_val)];

				begin %=
					qi::eps
					>> qi::lit("m");

				line %=
					qi::eps
					>> qi::lit("l");

				full_curve %=
					qi::eps
					>> qi::lit("c");

				final_curve %=
					qi::eps
					>> qi::lit("v");

				initial_curve %=
					qi::eps
					>> qi::lit("y");

				close %=
					qi::eps
					>> qi::lit("h");

				rectangle %=
					qi::eps
					>> qi::lit("re");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(begin);
				BOOST_SPIRIT_DEBUG_NODE(line);
				BOOST_SPIRIT_DEBUG_NODE(full_curve);
				BOOST_SPIRIT_DEBUG_NODE(final_curve);
				BOOST_SPIRIT_DEBUG_NODE(initial_curve);
				BOOST_SPIRIT_DEBUG_NODE(close);
				BOOST_SPIRIT_DEBUG_NODE(rectangle);
			}

			PathPaintingGrammar::PathPaintingGrammar() :
				base_type(start, "Path painting grammar")
			{
				start %=
					stroke[phoenix::bind(&convert_Stroke, qi::_1, qi::_val)]
					| close_stroke[phoenix::bind(&convert_CloseAndStroke, qi::_1, qi::_val)]
					| fill_nz[phoenix::bind(&convert_FillPathNonzero, qi::_1, qi::_val)]
					| fill_comp[phoenix::bind(&convert_FillPathCompatibility, qi::_1, qi::_val)]
					| fill_eo[phoenix::bind(&convert_FillPathEvenOdd, qi::_1, qi::_val)]
					| fill_stroke_nz[phoenix::bind(&convert_FillStrokeNonzero, qi::_1, qi::_val)]
					| fill_stroke_eo[phoenix::bind(&convert_FillStrokeEvenOdd, qi::_1, qi::_val)]
					| close_fill_stroke_nz[phoenix::bind(&convert_CloseFillStrokeNonzero, qi::_1, qi::_val)]
					| close_fill_stroke_eo[phoenix::bind(&convert_CloseFillStrokeEvenOdd, qi::_1, qi::_val)]
					| end_path[phoenix::bind(&convert_EndPath, qi::_1, qi::_val)];

				stroke %=
					qi::eps
					>> qi::lit("S");

				close_stroke %=
					qi::eps
					>> qi::lit("s");

				fill_nz %=
					qi::eps
					>> qi::lit("f");

				fill_comp %=
					qi::eps
					>> qi::lit("F");

				fill_eo %=
					qi::eps
					>> qi::lit("f*");

				fill_stroke_nz %=
					qi::eps
					>> qi::lit("B");

				fill_stroke_eo %=
					qi::eps
					>> qi::lit("B*");

				close_fill_stroke_nz %=
					qi::eps
					>> qi::lit("b");

				close_fill_stroke_eo %=
					qi::eps
					>> qi::lit("b*");

				end_path %=
					qi::eps
					>> qi::lit("n");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(stroke);
				BOOST_SPIRIT_DEBUG_NODE(close_stroke);
				BOOST_SPIRIT_DEBUG_NODE(fill_nz);
				BOOST_SPIRIT_DEBUG_NODE(fill_comp);
				BOOST_SPIRIT_DEBUG_NODE(fill_eo);
				BOOST_SPIRIT_DEBUG_NODE(fill_stroke_nz);
				BOOST_SPIRIT_DEBUG_NODE(fill_stroke_eo);
				BOOST_SPIRIT_DEBUG_NODE(close_fill_stroke_nz);
				BOOST_SPIRIT_DEBUG_NODE(close_fill_stroke_eo);
				BOOST_SPIRIT_DEBUG_NODE(end_path);
			}

			ShadingPatternGrammar::ShadingPatternGrammar() :
				base_type(start, "Shading pattern grammar")
			{
				start %=
					shading[phoenix::bind(&convert_ShadingPaint, qi::_1, qi::_val)];

				shading %=
					qi::eps
					>> qi::lit("sh");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(shading);
			}

			SpecialGraphicsStateGrammar::SpecialGraphicsStateGrammar() :
				base_type(start, "Special graphics state grammar")
			{
				start %=
					save[phoenix::bind(&convert_SaveGraphicsState, qi::_1, qi::_val)]
					| restore[phoenix::bind(&convert_RestoreGraphicsState, qi::_1, qi::_val)]
					| matrix[phoenix::bind(&convert_TransformationMatrix, qi::_1, qi::_val)];

				save %=
					qi::eps
					>> qi::lit("q");

				restore %=
					qi::eps
					>> qi::lit("Q");

				matrix %=
					qi::eps
					>> qi::lit("cm");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(save);
				BOOST_SPIRIT_DEBUG_NODE(restore);
				BOOST_SPIRIT_DEBUG_NODE(matrix);
			}

			TextObjectGrammar::TextObjectGrammar() :
				base_type(start, "Text object grammar")
			{
				start %=
					begin_text[phoenix::bind(&convert_BeginText, qi::_1, qi::_val)]
					| end_text[phoenix::bind(&convert_EndText, qi::_1, qi::_val)];

				begin_text %=
					qi::eps
					>> qi::lit("BT");

				end_text %=
					qi::eps
					>> qi::lit("ET");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(begin_text);
				BOOST_SPIRIT_DEBUG_NODE(end_text);
			}

			TextPositioningGrammar::TextPositioningGrammar() :
				base_type(start, "Text positioning grammar")
			{
				start %=
					translate[phoenix::bind(&convert_TextTranslate, qi::_1, qi::_val)]
					| translate_leading[phoenix::bind(&convert_TextTranslateLeading, qi::_1, qi::_val)]
					| matrix[phoenix::bind(&convert_TextMatrix, qi::_1, qi::_val)]
					| next_line[phoenix::bind(&convert_TextNextLine, qi::_1, qi::_val)];

				translate %=
					qi::eps
					>> qi::lit("Td");

				translate_leading %=
					qi::eps
					>> qi::lit("TD");

				matrix %=
					qi::eps
					>> qi::lit("Tm");

				next_line %=
					qi::eps
					>> qi::lit("T*");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(translate);
				BOOST_SPIRIT_DEBUG_NODE(translate_leading);
				BOOST_SPIRIT_DEBUG_NODE(matrix);
				BOOST_SPIRIT_DEBUG_NODE(next_line);
			}

			TextShowingGrammar::TextShowingGrammar() :
				base_type(start, "Text showing grammar")
			{
				start %=
					show[phoenix::bind(&convert_TextShow, qi::_1, qi::_val)]
					| show_array[phoenix::bind(&convert_TextShowArray, qi::_1, qi::_val)]
					| next_line_show[phoenix::bind(&convert_TextNextLineShow, qi::_1, qi::_val)]
					| next_line_show_spacing[phoenix::bind(&convert_TextNextLineShowSpacing, qi::_1, qi::_val)];

				show %=
					qi::eps
					>> qi::lit("Tj");

				show_array %=
					qi::eps
					>> qi::lit("TJ");

				next_line_show %=
					qi::eps
					>> qi::lit("'");

				next_line_show_spacing %=
					qi::eps
					>> qi::lit("\"");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(show);
				BOOST_SPIRIT_DEBUG_NODE(show_array);
				BOOST_SPIRIT_DEBUG_NODE(next_line_show);
				BOOST_SPIRIT_DEBUG_NODE(next_line_show_spacing);
			}

			TextStateGrammar::TextStateGrammar() :
				base_type(start, "Text state grammar")
			{
				start %=
					char_spacing[phoenix::bind(&convert_CharacterSpacing, qi::_1, qi::_val)]
					| word_spacing[phoenix::bind(&convert_WordSpacing, qi::_1, qi::_val)]
					| hscaling[phoenix::bind(&convert_HorizontalScaling, qi::_1, qi::_val)]
					| leading[phoenix::bind(&convert_Leading, qi::_1, qi::_val)]
					| font[phoenix::bind(&convert_TextFont, qi::_1, qi::_val)]
					| rendering_mode[phoenix::bind(&convert_TextRenderingMode, qi::_1, qi::_val)]
					| rise[phoenix::bind(&convert_TextRise, qi::_1, qi::_val)];

				char_spacing %=
					qi::eps
					>> qi::lit("Tc");

				word_spacing %=
					qi::eps
					>> qi::lit("Tw");

				hscaling %=
					qi::eps
					>> qi::lit("Tz");

				leading %=
					qi::eps
					>> qi::lit("TL");

				font %=
					qi::eps
					>> qi::lit("Tf");

				rendering_mode %=
					qi::eps
					>> qi::lit("Tr");

				rise %=
					qi::eps
					>> qi::lit("Ts");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(char_spacing);
				BOOST_SPIRIT_DEBUG_NODE(word_spacing);
				BOOST_SPIRIT_DEBUG_NODE(hscaling);
				BOOST_SPIRIT_DEBUG_NODE(leading);
				BOOST_SPIRIT_DEBUG_NODE(font);
				BOOST_SPIRIT_DEBUG_NODE(rendering_mode);
				BOOST_SPIRIT_DEBUG_NODE(rise);
			}

			Type3FontGrammar::Type3FontGrammar() :
				base_type(start, "Type 3 font grammar")
			{
				start %=
					char_width[phoenix::bind(&convert_SetCharWidth, qi::_1, qi::_val)]
					| cache_device[phoenix::bind(&convert_SetCacheDevice, qi::_1, qi::_val)];

				char_width %=
					qi::eps
					>> qi::lit("d0");

				cache_device %=
					qi::eps
					>> qi::lit("d1");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(char_width);
				BOOST_SPIRIT_DEBUG_NODE(cache_device);
			}

			XObjectGrammar::XObjectGrammar() :
				base_type(start, "XObject grammar")
			{
				start %=
					invoke[phoenix::bind(&convert_InvokeXObject, qi::_1, qi::_val)];

				invoke %=
					qi::eps
					>> qi::lit("Do");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(invoke);
			}
		}
	}
}
