#include "precompiled.h"
#include "content_utils.h"

#include "content_stream_operations.h"

namespace gotchangpdf
{
	namespace semantics
	{
		namespace contents
		{
			namespace sync = syntax::contents;
			sync::OperationBasePtr gotchangpdf::semantics::contents::ContentUtils::ConvertGenericOperation(sync::OperationGenericPtr obj)
			{
				auto oper = obj->GetOperator();
				auto type = oper->GetType();
				switch (type)
				{
				case sync::OperatorBase::Type::Unknown:
					return obj;
				case sync::OperatorBase::Type::LineWidth:
					return obj;
				case sync::OperatorBase::Type::LineCap:
					return obj;
				case sync::OperatorBase::Type::LineJoin:
					return obj;
				case sync::OperatorBase::Type::MiterLimit:
					return obj;
				case sync::OperatorBase::Type::DashPattern:
					return obj;
				case sync::OperatorBase::Type::ColorRenderingIntent:
					return obj;
				case sync::OperatorBase::Type::Flatness:
					return obj;
				case sync::OperatorBase::Type::GraphicsState:
					return obj;
				case sync::OperatorBase::Type::SaveGraphicsState:
					return obj;
				case sync::OperatorBase::Type::RestoreGraphicsState:
					return obj;
				case sync::OperatorBase::Type::TransformationMatrix:
					return obj;
				case sync::OperatorBase::Type::BeginSubpath:
					return obj;
				case sync::OperatorBase::Type::Line:
					return obj;
				case sync::OperatorBase::Type::FullCurve:
					return obj;
				case sync::OperatorBase::Type::FinalCurve:
					return obj;
				case sync::OperatorBase::Type::InitialCurve:
					return obj;
				case sync::OperatorBase::Type::CloseSubpath:
					return obj;
				case sync::OperatorBase::Type::Rectangle:
					return obj;
				case sync::OperatorBase::Type::Stroke:
					return obj;
				case sync::OperatorBase::Type::CloseAndStroke:
					return obj;
				case sync::OperatorBase::Type::FillPathNonzero:
					return obj;
				case sync::OperatorBase::Type::FillPathCompatibility:
					return obj;
				case sync::OperatorBase::Type::FillPathEvenOdd:
					return obj;
				case sync::OperatorBase::Type::FillStrokeNonzero:
					return obj;
				case sync::OperatorBase::Type::FillStrokeEvenOdd:
					return obj;
				case sync::OperatorBase::Type::CloseFillStrokeNonzero:
					return obj;
				case sync::OperatorBase::Type::CloseFillStrokeEvenOdd:
					return obj;
				case sync::OperatorBase::Type::EndPath:
					return obj;
				case sync::OperatorBase::Type::ClipPathNonzero:
					return obj;
				case sync::OperatorBase::Type::ClipPathEvenOdd:
					return obj;
				case sync::OperatorBase::Type::BeginText:
					assert(obj->GetOperandsSize() == 0);
					return OperationBeginTextPtr();
				case sync::OperatorBase::Type::EndText:
					assert(obj->GetOperandsSize() == 0);
					return OperationEndTextPtr();
				case sync::OperatorBase::Type::CharacterSpacing:
					return obj;
				case sync::OperatorBase::Type::WordSpacing:
					return obj;
				case sync::OperatorBase::Type::HorizontalScaling:
					return obj;
				case sync::OperatorBase::Type::Leading:
					return obj;
				case sync::OperatorBase::Type::TextFont:
					return obj;
				case sync::OperatorBase::Type::TextRenderingMode:
					return obj;
				case sync::OperatorBase::Type::TextRise:
					return obj;
				case sync::OperatorBase::Type::TextTranslate:
					return obj;
				case sync::OperatorBase::Type::TextTranslateLeading:
					return obj;
				case sync::OperatorBase::Type::TextMatrix:
					return obj;
				case sync::OperatorBase::Type::TextNextLine:
					return obj;
				case sync::OperatorBase::Type::TextShow:
				{
					auto operands = obj->GetOperands();
					if (1 != operands.size())
						return obj;

					auto item = operands.at(0);
					if (!syntax::ObjectUtils::IsType<syntax::StringObjectPtr>(item))
						return obj;

					auto str = syntax::ObjectUtils::ConvertTo<syntax::StringObjectPtr>(item);
					return OperationTextShowPtr(str);
				}
				case sync::OperatorBase::Type::TextShowArray:
					return obj;
				case sync::OperatorBase::Type::TextNextLineShow:
					return obj;
				case sync::OperatorBase::Type::TextNextLineShowSpacing:
					return obj;
				case sync::OperatorBase::Type::SetCharWidth:
					return obj;
				case sync::OperatorBase::Type::SetCacheDevice:
					return obj;
				case sync::OperatorBase::Type::ColorSpaceStroke:
					return obj;
				case sync::OperatorBase::Type::ColorSpaceNonstroke:
					return obj;
				case sync::OperatorBase::Type::SetColorStroke:
					return obj;
				case sync::OperatorBase::Type::SetColorStrokeExtended:
					return obj;
				case sync::OperatorBase::Type::SetColorNonstroke:
					return obj;
				case sync::OperatorBase::Type::SetColorNonstrokeExtended:
					return obj;
				case sync::OperatorBase::Type::SetStrokingColorSpaceGray:
					return obj;
				case sync::OperatorBase::Type::SetNonstrokingColorSpaceGray:
					return obj;
				case sync::OperatorBase::Type::SetStrokingColorSpaceRGB:
					return obj;
				case sync::OperatorBase::Type::SetNonstrokingColorSpaceRGB:
					return obj;
				case sync::OperatorBase::Type::SetStrokingColorSpaceCMYK:
					return obj;
				case sync::OperatorBase::Type::SetNonstrokingColorSpaceCMYK:
					return obj;
				case sync::OperatorBase::Type::ShadingPaint:
					return obj;
				case sync::OperatorBase::Type::BeginInlineImageObject:
					return obj;
				case sync::OperatorBase::Type::BeginInlineImageData:
					return obj;
				case sync::OperatorBase::Type::EndInlineImageObject:
					return obj;
				case sync::OperatorBase::Type::InvokeXObject:
					return obj;
				case sync::OperatorBase::Type::DefineMarkedContentPoint:
					return obj;
				case sync::OperatorBase::Type::DefineMarkedContentPointWithPropertyList:
					return obj;
				case sync::OperatorBase::Type::BeginMarkedContentSequence:
					return obj;
				case sync::OperatorBase::Type::BeginMarkedContentSequenceWithPropertyList:
					return obj;
				case sync::OperatorBase::Type::EndMarkedContentSequence:
					return obj;
				case sync::OperatorBase::Type::BeginCompatibilitySection:
					return obj;
				case sync::OperatorBase::Type::EndCompatibilitySection:
					return obj;
				default:
					assert(!"Unrecognized operator type");
					return obj;
				}
			}
		}
	}
}
