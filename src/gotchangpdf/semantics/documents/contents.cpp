#include "precompiled.h"
#include "contents.h"

#include "content_stream_grammar.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace syntax;

		Contents::Contents(StreamObjectPtr obj)
		{
			ContentStreamPtr stream(obj);
			_contents.push_back(obj);
		}

		Contents::Contents(ArrayObjectPtr<IndirectObjectReferencePtr> obj)
		{
			for (auto& ref : *obj) {
				auto item = ref->GetReferencedObjectAs<StreamObjectPtr>();
				_contents.push_back(item);
			}
		}

		class ConvertOperationVisitor : public boost::static_visitor<ContentOperationPtr>
		{
		public:
			ConvertOperationVisitor(contents::OperationGenericPtr oper) : _oper(oper) {}

			ContentOperationPtr operator()(const contents::UnknownOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::LineWidthOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::LineCapOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::LineJoinOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::MiterLimitOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::DashPatternOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::ColorRenderingIntentOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::FlatnessOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::GraphicsStateOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SaveGraphicsStateOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::RestoreGraphicsStateOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TransformationMatrixOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::BeginSubpathOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::LineOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::FullCurveOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::FinalCurveOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::InitialCurveOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::CloseSubpathOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::RectangleOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::StrokeOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::CloseAndStrokeOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::FillPathNonzeroOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::FillPathCompatibilityOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::FillPathEvenOddOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::FillStrokeNonzeroOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::FillStrokeEvenOddOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::CloseFillStrokeNonzeroOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::CloseFillStrokeEvenOddOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::EndPathOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::ClipPathNonzeroOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::ClipPathEvenOddOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::BeginTextOperator& obj) { assert(_oper->GetOperandsSize() == 0); return BeginTextOperationPtr(); }
			ContentOperationPtr operator()(const contents::EndTextOperator& obj) { assert(_oper->GetOperandsSize() == 0); return EndTextOperationPtr(); }
			ContentOperationPtr operator()(const contents::CharacterSpacingOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::WordSpacingOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::HorizontalScalingOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::LeadingOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TextFontOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TextRenderingModeOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TextRiseOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TextTranslateOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TextTranslateLeadingOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TextMatrixOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TextNextLineOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TextShowOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TextShowArrayOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TextNextLineShowOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::TextNextLineShowSpacingOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetCharWidthOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetCacheDeviceOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::ColorSpaceStrokeOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::ColorSpaceNonstrokeOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetColorStrokeOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetColorStrokeExtendedOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetColorNonstrokeOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetColorNonstrokeExtendedOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetStrokingColorSpaceGrayOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetNonstrokingColorSpaceGrayOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetStrokingColorSpaceRGBOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetNonstrokingColorSpaceRGBOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetStrokingColorSpaceCMYKOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::SetNonstrokingColorSpaceCMYKOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::ShadingPaintOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::BeginInlineImageObjectOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::BeginInlineImageDataOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::EndInlineImageObjectOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::InvokeXObjectOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::DefineMarkedContentPointOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::DefineMarkedContentPointWithPropertyListOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::BeginMarkedContentSequenceOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::BeginMarkedContentSequenceWithPropertyListOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::EndMarkedContentSequenceOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::BeginCompatibilitySectionOperator& obj) { return _oper; }
			ContentOperationPtr operator()(const contents::EndCompatibilitySectionOperator& obj) { return _oper; }

		private:
			contents::OperationGenericPtr _oper;
		};

		ContentInstructionCollection Contents::Instructions(void) const
		{
			if (!_instructions.empty())
				return _instructions;

			ContentOperationCollection ops;

			for (auto item : _contents) {
				auto operations = item->Operations();
				auto size = operations.size();
				ops.reserve(ops.size() + size);
				for (unsigned int i = 0; i < size; ++i) {
					auto op = operations.at(i);
					auto oper = op->GetOperator();

					ConvertOperationVisitor convert_visitor(op);
					auto converted = oper.apply_visitor(convert_visitor);
					ops.push_back(converted);
				}
			}

			// visitors
			contents::IsTypeVisitor<BeginTextOperationPtr> is_begin_text;
			contents::IsTypeVisitor<EndTextOperationPtr> is_end_text;

			ContentInstructionCollection result;
			for (auto it = ops.begin(); it != ops.end(); ++it) {
				if (it->apply_visitor(is_begin_text)) {

					auto last = std::find_if(it + 1, ops.end(), [it, is_end_text](const decltype(it)::value_type& item) {
						return item.apply_visitor(is_end_text);
					});

					assert(ops.end() != last && "End of current Text Object was not found");
					if (ops.end() == last) {
						result.push_back(*it);
						continue;
					}

					// Construct the collection
					ContentOperationCollection text_object_data(it, last);

					// Erase Begin and End of Text Object, because they are not part of object
					text_object_data.erase(text_object_data.begin());
					text_object_data.pop_back();

					ContentObjectPtr text_object = TextObjectPtr(text_object_data);
					result.push_back(text_object);
					it = last;
				}
				else {
					// unknown
					result.push_back(*it);
				}
			}

			_instructions = result;
			return result;
		}

		types::uinteger Contents::GetInstructionsSize(void) const
		{
			auto ops = Instructions();
			return ops.size();
		}

		ContentInstructionPtr Contents::GetInstructionAt(types::uinteger at) const
		{
			auto ops = Instructions();
			return ops.at(at);
		}
	}
}

