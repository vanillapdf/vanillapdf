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

		class ContentUtils
		{
		public:
			static contents::OperationBasePtr ConvertGenericOperation(contents::OperationGenericPtr obj)
			{
				auto type = obj->GetOperationType();
				switch (type)
				{
				case contents::OperationBase::Type::Generic:
					return obj;
				case contents::OperationBase::Type::LineWidth:
					return obj;
				case contents::OperationBase::Type::LineCap:
					return obj;
				case contents::OperationBase::Type::LineJoin:
					return obj;
				case contents::OperationBase::Type::MiterLimit:
					return obj;
				case contents::OperationBase::Type::DashPattern:
					return obj;
				case contents::OperationBase::Type::ColorRenderingIntent:
					return obj;
				case contents::OperationBase::Type::Flatness:
					return obj;
				case contents::OperationBase::Type::GraphicsState:
					return obj;
				case contents::OperationBase::Type::SaveGraphicsState:
					return obj;
				case contents::OperationBase::Type::RestoreGraphicsState:
					return obj;
				case contents::OperationBase::Type::TransformationMatrix:
					return obj;
				case contents::OperationBase::Type::BeginSubpath:
					return obj;
				case contents::OperationBase::Type::Line:
					return obj;
				case contents::OperationBase::Type::FullCurve:
					return obj;
				case contents::OperationBase::Type::FinalCurve:
					return obj;
				case contents::OperationBase::Type::InitialCurve:
					return obj;
				case contents::OperationBase::Type::CloseSubpath:
					return obj;
				case contents::OperationBase::Type::Rectangle:
					return obj;
				case contents::OperationBase::Type::Stroke:
					return obj;
				case contents::OperationBase::Type::CloseAndStroke:
					return obj;
				case contents::OperationBase::Type::FillPathNonzero:
					return obj;
				case contents::OperationBase::Type::FillPathCompatibility:
					return obj;
				case contents::OperationBase::Type::FillPathEvenOdd:
					return obj;
				case contents::OperationBase::Type::FillStrokeNonzero:
					return obj;
				case contents::OperationBase::Type::FillStrokeEvenOdd:
					return obj;
				case contents::OperationBase::Type::CloseFillStrokeNonzero:
					return obj;
				case contents::OperationBase::Type::CloseFillStrokeEvenOdd:
					return obj;
				case contents::OperationBase::Type::EndPath:
					return obj;
				case contents::OperationBase::Type::ClipPathNonzero:
					return obj;
				case contents::OperationBase::Type::ClipPathEvenOdd:
					return obj;
				case contents::OperationBase::Type::BeginText:
					assert(obj->GetOperandsSize() == 0);
					return BeginTextOperationPtr();
				case contents::OperationBase::Type::EndText:
					assert(obj->GetOperandsSize() == 0);
					return EndTextOperationPtr();
				case contents::OperationBase::Type::CharacterSpacing:
					return obj;
				case contents::OperationBase::Type::WordSpacing:
					return obj;
				case contents::OperationBase::Type::HorizontalScaling:
					return obj;
				case contents::OperationBase::Type::Leading:
					return obj;
				case contents::OperationBase::Type::TextFont:
					return obj;
				case contents::OperationBase::Type::TextRenderingMode:
					return obj;
				case contents::OperationBase::Type::TextRise:
					return obj;
				case contents::OperationBase::Type::TextTranslate:
					return obj;
				case contents::OperationBase::Type::TextTranslateLeading:
					return obj;
				case contents::OperationBase::Type::TextMatrix:
					return obj;
				case contents::OperationBase::Type::TextNextLine:
					return obj;
				case contents::OperationBase::Type::TextShow:
					return obj;
				case contents::OperationBase::Type::TextShowArray:
					return obj;
				case contents::OperationBase::Type::TextNextLineShow:
					return obj;
				case contents::OperationBase::Type::TextNextLineShowSpacing:
					return obj;
				case contents::OperationBase::Type::SetCharWidth:
					return obj;
				case contents::OperationBase::Type::SetCacheDevice:
					return obj;
				case contents::OperationBase::Type::ColorSpaceStroke:
					return obj;
				case contents::OperationBase::Type::ColorSpaceNonstroke:
					return obj;
				case contents::OperationBase::Type::SetColorStroke:
					return obj;
				case contents::OperationBase::Type::SetColorStrokeExtended:
					return obj;
				case contents::OperationBase::Type::SetColorNonstroke:
					return obj;
				case contents::OperationBase::Type::SetColorNonstrokeExtended:
					return obj;
				case contents::OperationBase::Type::SetStrokingColorSpaceGray:
					return obj;
				case contents::OperationBase::Type::SetNonstrokingColorSpaceGray:
					return obj;
				case contents::OperationBase::Type::SetStrokingColorSpaceRGB:
					return obj;
				case contents::OperationBase::Type::SetNonstrokingColorSpaceRGB:
					return obj;
				case contents::OperationBase::Type::SetStrokingColorSpaceCMYK:
					return obj;
				case contents::OperationBase::Type::SetNonstrokingColorSpaceCMYK:
					return obj;
				case contents::OperationBase::Type::ShadingPaint:
					return obj;
				case contents::OperationBase::Type::BeginInlineImageObject:
					return obj;
				case contents::OperationBase::Type::BeginInlineImageData:
					return obj;
				case contents::OperationBase::Type::EndInlineImageObject:
					return obj;
				case contents::OperationBase::Type::InvokeXObject:
					return obj;
				case contents::OperationBase::Type::DefineMarkedContentPoint:
					return obj;
				case contents::OperationBase::Type::DefineMarkedContentPointWithPropertyList:
					return obj;
				case contents::OperationBase::Type::BeginMarkedContentSequence:
					return obj;
				case contents::OperationBase::Type::BeginMarkedContentSequenceWithPropertyList:
					return obj;
				case contents::OperationBase::Type::EndMarkedContentSequence:
					return obj;
				case contents::OperationBase::Type::BeginCompatibilitySection:
					return obj;
				case contents::OperationBase::Type::EndCompatibilitySection:
					return obj;
				default:
					throw Exception("Unknown operation type");
				}
			}

			template <typename T>
			static T ConvertTo(const contents::OperationBasePtr& obj)
			{
				auto ptr = obj.Content.get();
				auto converted = dynamic_cast<typename T::value_type *>(ptr);
				if (nullptr == converted)
					throw Exception("Could not convert object to destination type");

				return T(converted);
			}

			template <typename T>
			static bool IsType(const contents::OperationBasePtr& obj)
			{
				auto ptr = obj.Content.get();
				auto converted = dynamic_cast<typename T::value_type *>(ptr);
				return (nullptr != converted);
			}
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

					auto converted = ContentUtils::ConvertGenericOperation(op);
					ops.push_back(converted);
				}
			}

			ContentInstructionCollection result;
			for (auto it = ops.begin(); it != ops.end(); ++it) {
				if (ContentUtils::IsType<BeginTextOperationPtr>(*it)) {
					auto last = std::find_if(it + 1, ops.end(), [it](const decltype(it)::value_type& item) {
						return ContentUtils::IsType<EndTextOperationPtr>(item);
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

		contents::InstructionBasePtr Contents::GetInstructionAt(types::uinteger at) const
		{
			auto ops = Instructions();
			return ops.at(at);
		}
	}
}

