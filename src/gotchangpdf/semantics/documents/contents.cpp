#include "precompiled.h"
#include "contents.h"

#include "content_stream_grammar.h"
#include "content_stream_operation_generic.h"
#include "content_stream_operators.h"
#include "content_stream_objects.h"

namespace gotchangpdf
{
	namespace semantics
	{
		namespace sync = syntax::contents;

		namespace contents
		{
			class ContentUtils
			{
			public:
				static sync::OperationBasePtr ConvertGenericOperation(sync::OperationGenericPtr obj)
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
						throw syntax::Exception("Unknown operation type");
					}
				}

				template <typename T>
				static T ConvertTo(const sync::OperationBasePtr& obj)
				{
					auto ptr = obj.Content.get();
					auto converted = dynamic_cast<typename T::value_type *>(ptr);
					if (nullptr == converted)
						throw Exception("Could not convert object to destination type");

					return T(converted);
				}

				template <typename T>
				static bool IsType(const sync::OperationBasePtr& obj)
				{
					auto ptr = obj.Content.get();
					auto converted = dynamic_cast<typename T::value_type *>(ptr);
					return (nullptr != converted);
				}
			};
		}

		using namespace syntax;
		using namespace contents;

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

		sync::BaseInstructionCollection Contents::Instructions(void) const
		{
			if (!_instructions.empty())
				return _instructions;

			sync::BaseOperationCollection ops;

			for (auto item : _contents) {
				auto operations = item->Operations();
				auto size = operations.size();
				ops.reserve(ops.size() + size);
				for (unsigned int i = 0; i < size; ++i) {
					auto op = operations.at(i);

					auto converted = ContentUtils::ConvertGenericOperation(op);
					ops.push_back(converted);
				}
			}

			sync::BaseInstructionCollection result;
			for (auto it = ops.begin(); it != ops.end(); ++it) {
				if (ContentUtils::IsType<OperationBeginTextPtr>(*it)) {
					auto last = std::find_if(it + 1, ops.end(), [it](const decltype(it)::value_type& item) {
						return ContentUtils::IsType<OperationEndTextPtr>(item);
					});

					assert(ops.end() != last && "End of current Text Object was not found");
					if (ops.end() == last) {
						result.push_back(*it);
						continue;
					}

					// Construct the collection
					sync::BaseOperationCollection text_object_data(it, last);

					// Erase Begin and End of Text Object, because they are not part of object
					text_object_data.erase(text_object_data.begin());
					text_object_data.pop_back();

					TextObjectPtr text_object(text_object_data);
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

		sync::InstructionBasePtr Contents::GetInstructionAt(types::uinteger at) const
		{
			auto ops = Instructions();
			return ops.at(at);
		}
	}
}

