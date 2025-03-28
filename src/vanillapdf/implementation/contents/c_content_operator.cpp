#include "precompiled.h"

#include "contents/content_stream_operators.h"

#include "vanillapdf/contents/c_content_operator.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::contents;

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperator_GetOperatorType(ContentOperatorHandle* handle, ContentOperatorType* result) {
    OperatorBase* obj = reinterpret_cast<OperatorBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    switch (obj->GetOperatorType()) {
        case OperatorBase::Type::Unknown:
            *result = ContentOperatorType_Unknown; break;
        case OperatorBase::Type::LineWidth:
            *result = ContentOperatorType_LineWidth; break;
        case OperatorBase::Type::LineCap:
            *result = ContentOperatorType_LineCap; break;
        case OperatorBase::Type::LineJoin:
            *result = ContentOperatorType_LineJoin; break;
        case OperatorBase::Type::MiterLimit:
            *result = ContentOperatorType_MiterLimit; break;
        case OperatorBase::Type::DashPattern:
            *result = ContentOperatorType_DashPattern; break;
        case OperatorBase::Type::ColorRenderingIntent:
            *result = ContentOperatorType_ColorRenderingIntent; break;
        case OperatorBase::Type::Flatness:
            *result = ContentOperatorType_Flatness; break;
        case OperatorBase::Type::GraphicsState:
            *result = ContentOperatorType_GraphicsState; break;
        case OperatorBase::Type::SaveGraphicsState:
            *result = ContentOperatorType_SaveGraphicsState; break;
        case OperatorBase::Type::RestoreGraphicsState:
            *result = ContentOperatorType_RestoreGraphicsState; break;
        case OperatorBase::Type::TransformationMatrix:
            *result = ContentOperatorType_TransformationMatrix; break;
        case OperatorBase::Type::BeginSubpath:
            *result = ContentOperatorType_BeginSubpath; break;
        case OperatorBase::Type::Line:
            *result = ContentOperatorType_Line; break;
        case OperatorBase::Type::FullCurve:
            *result = ContentOperatorType_FullCurve; break;
        case OperatorBase::Type::FinalCurve:
            *result = ContentOperatorType_FinalCurve; break;
        case OperatorBase::Type::InitialCurve:
            *result = ContentOperatorType_InitialCurve; break;
        case OperatorBase::Type::CloseSubpath:
            *result = ContentOperatorType_CloseSubpath; break;
        case OperatorBase::Type::Rectangle:
            *result = ContentOperatorType_Rectangle; break;
        case OperatorBase::Type::Stroke:
            *result = ContentOperatorType_Stroke; break;
        case OperatorBase::Type::CloseAndStroke:
            *result = ContentOperatorType_CloseAndStroke; break;
        case OperatorBase::Type::FillPathNonzero:
            *result = ContentOperatorType_FillPathNonzero; break;
        case OperatorBase::Type::FillPathCompatibility:
            *result = ContentOperatorType_FillPathCompatibility; break;
        case OperatorBase::Type::FillPathEvenOdd:
            *result = ContentOperatorType_FillPathEvenOdd; break;
        case OperatorBase::Type::FillStrokeNonzero:
            *result = ContentOperatorType_FillStrokeNonzero; break;
        case OperatorBase::Type::FillStrokeEvenOdd:
            *result = ContentOperatorType_FillStrokeEvenOdd; break;
        case OperatorBase::Type::CloseFillStrokeNonzero:
            *result = ContentOperatorType_CloseFillStrokeNonzero; break;
        case OperatorBase::Type::CloseFillStrokeEvenOdd:
            *result = ContentOperatorType_CloseFillStrokeEvenOdd; break;
        case OperatorBase::Type::EndPath:
            *result = ContentOperatorType_EndPath; break;
        case OperatorBase::Type::ClipPathNonzero:
            *result = ContentOperatorType_ClipPathNonzero; break;
        case OperatorBase::Type::ClipPathEvenOdd:
            *result = ContentOperatorType_ClipPathEvenOdd; break;
        case OperatorBase::Type::BeginText:
            *result = ContentOperatorType_BeginText; break;
        case OperatorBase::Type::EndText:
            *result = ContentOperatorType_EndText; break;
        case OperatorBase::Type::CharacterSpacing:
            *result = ContentOperatorType_CharacterSpacing; break;
        case OperatorBase::Type::WordSpacing:
            *result = ContentOperatorType_WordSpacing; break;
        case OperatorBase::Type::HorizontalScaling:
            *result = ContentOperatorType_HorizontalScaling; break;
        case OperatorBase::Type::Leading:
            *result = ContentOperatorType_Leading; break;
        case OperatorBase::Type::TextFont:
            *result = ContentOperatorType_TextFont; break;
        case OperatorBase::Type::TextRenderingMode:
            *result = ContentOperatorType_TextRenderingMode; break;
        case OperatorBase::Type::TextRise:
            *result = ContentOperatorType_TextRise; break;
        case OperatorBase::Type::TextTranslate:
            *result = ContentOperatorType_TextTranslate; break;
        case OperatorBase::Type::TextTranslateLeading:
            *result = ContentOperatorType_TextTranslateLeading; break;
        case OperatorBase::Type::TextMatrix:
            *result = ContentOperatorType_TextMatrix; break;
        case OperatorBase::Type::TextNextLine:
            *result = ContentOperatorType_TextNextLine; break;
        case OperatorBase::Type::TextShow:
            *result = ContentOperatorType_TextShow; break;
        case OperatorBase::Type::TextShowArray:
            *result = ContentOperatorType_TextShowArray; break;
        case OperatorBase::Type::TextNextLineShow:
            *result = ContentOperatorType_TextNextLineShow; break;
        case OperatorBase::Type::TextNextLineShowSpacing:
            *result = ContentOperatorType_TextNextLineShowSpacing; break;
        case OperatorBase::Type::SetCharWidth:
            *result = ContentOperatorType_SetCharWidth; break;
        case OperatorBase::Type::SetCacheDevice:
            *result = ContentOperatorType_SetCacheDevice; break;
        case OperatorBase::Type::SetStrokingColorSpaceName:
            *result = ContentOperatorType_SetStrokingColorSpaceName; break;
        case OperatorBase::Type::SetNonstrokingColorSpaceName:
            *result = ContentOperatorType_SetNonstrokingColorSpaceName; break;
        case OperatorBase::Type::SetStrokingColorSpaceDevice:
            *result = ContentOperatorType_SetStrokingColorSpaceDevice; break;
        case OperatorBase::Type::SetStrokingColorSpaceDeviceExtended:
            *result = ContentOperatorType_SetStrokingColorSpaceDeviceExtended; break;
        case OperatorBase::Type::SetNonstrokingColorSpaceDevice:
            *result = ContentOperatorType_SetNonstrokingColorSpaceDevice; break;
        case OperatorBase::Type::SetNonstrokingColorSpaceDeviceExtended:
            *result = ContentOperatorType_SetNonstrokingColorSpaceDeviceExtended; break;
        case OperatorBase::Type::SetStrokingColorSpaceGray:
            *result = ContentOperatorType_SetStrokingColorSpaceGray; break;
        case OperatorBase::Type::SetNonstrokingColorSpaceGray:
            *result = ContentOperatorType_SetNonstrokingColorSpaceGray; break;
        case OperatorBase::Type::SetStrokingColorSpaceRGB:
            *result = ContentOperatorType_SetStrokingColorSpaceRGB; break;
        case OperatorBase::Type::SetNonstrokingColorSpaceRGB:
            *result = ContentOperatorType_SetNonstrokingColorSpaceRGB; break;
        case OperatorBase::Type::SetStrokingColorSpaceCMYK:
            *result = ContentOperatorType_SetStrokingColorSpaceCMYK; break;
        case OperatorBase::Type::SetNonstrokingColorSpaceCMYK:
            *result = ContentOperatorType_SetNonstrokingColorSpaceCMYK; break;
        case OperatorBase::Type::ShadingPaint:
            *result = ContentOperatorType_ShadingPaint; break;
        case OperatorBase::Type::BeginInlineImageObject:
            *result = ContentOperatorType_BeginInlineImageObject; break;
        case OperatorBase::Type::BeginInlineImageData:
            *result = ContentOperatorType_BeginInlineImageData; break;
        case OperatorBase::Type::EndInlineImageObject:
            *result = ContentOperatorType_EndInlineImageObject; break;
        case OperatorBase::Type::InvokeXObject:
            *result = ContentOperatorType_InvokeXObject; break;
        case OperatorBase::Type::DefineMarkedContentPoint:
            *result = ContentOperatorType_DefineMarkedContentPoint; break;
        case OperatorBase::Type::DefineMarkedContentPointWithPropertyList:
            *result = ContentOperatorType_DefineMarkedContentPointWithPropertyList; break;
        case OperatorBase::Type::BeginMarkedContentSequence:
            *result = ContentOperatorType_BeginMarkedContentSequence; break;
        case OperatorBase::Type::BeginMarkedContentSequenceWithPropertyList:
            *result = ContentOperatorType_BeginMarkedContentSequenceWithPropertyList; break;
        case OperatorBase::Type::EndMarkedContentSequence:
            *result = ContentOperatorType_EndMarkedContentSequence; break;
        case OperatorBase::Type::BeginCompatibilitySection:
            *result = ContentOperatorType_BeginCompatibilitySection; break;
        case OperatorBase::Type::EndCompatibilitySection:
            *result = ContentOperatorType_EndCompatibilitySection; break;
        default:
            assert(!"Unknown enum value");
            return VANILLAPDF_ERROR_GENERAL;
    }

    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperator_GetValue(ContentOperatorHandle* handle, BufferHandle** result) {
    OperatorBase* obj = reinterpret_cast<OperatorBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto buffer = obj->Value();
        auto ptr = buffer.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    }
    CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperator_ToUnknown(ContentOperatorHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<OperatorBase, IUnknown, ContentOperatorHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperator_FromUnknown(IUnknownHandle* handle, ContentOperatorHandle** result) {
    return SafeObjectConvert<IUnknown, OperatorBase, IUnknownHandle, ContentOperatorHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperator_Release(ContentOperatorHandle* handle) {
    return ObjectRelease<OperatorBase, ContentOperatorHandle>(handle);
}
