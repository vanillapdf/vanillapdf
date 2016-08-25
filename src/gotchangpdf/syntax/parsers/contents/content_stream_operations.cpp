#include "precompiled.h"
#include "content_stream_operations.h"
#include "content_stream_operators.h"

#include <sstream>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			OperationBeginText::OperationBeginText(const std::vector<ObjectPtr>& operands)
			{
				assert(operands.size() == 0); operands;
			}

			OperationEndText::OperationEndText(const std::vector<ObjectPtr>& operands)
			{
				assert(operands.size() == 0); operands;
			}

			OperationBeginInlineImageObject::OperationBeginInlineImageObject(const std::vector<ObjectPtr>& operands)
			{
				assert(operands.size() == 0); operands;
			}

			OperationBeginInlineImageData::OperationBeginInlineImageData(const std::vector<ObjectPtr>& operands)
			{
				assert(operands.size() == 0); operands;
			}

			OperationEndInlineImageObject::OperationEndInlineImageObject(const std::vector<ObjectPtr>& operands)
			{
				assert(operands.size() == 0); operands;
			}

			OperationTextShow::OperationTextShow(const std::vector<ObjectPtr>& operands)
			{
				if (1 != operands.size()) {
					assert(!"Text show operation has invalid arguments");
					return;
				}

				auto item = operands.at(0);
				if (!syntax::ObjectUtils::IsType<syntax::StringObjectPtr>(item)) {
					assert(!"Text show operation has invalid arguments");
					return;
				}

				_str = syntax::ObjectUtils::ConvertTo<syntax::StringObjectPtr>(item);
			}

			std::string OperationBeginText::ToPdf() const
			{
				BeginTextOperatorPtr op;
				BufferPtr buffer = op->Value();
				return buffer->ToString();
			}

			std::string OperationEndText::ToPdf() const
			{
				EndTextOperatorPtr op;
				BufferPtr buffer = op->Value();
				return buffer->ToString();
			}

			std::string OperationTextShow::ToPdf() const
			{
				std::stringstream ss;

				TextShowOperatorPtr op;
				ss << op->Value();
				ss << " ";
				ss << _str->ToPdf();

				return ss.str();
			}

			std::string OperationBeginInlineImageObject::ToPdf() const
			{
				BeginInlineImageObjectOperatorPtr op;
				BufferPtr buffer = op->Value();
				return buffer->ToString();
			}

			std::string OperationBeginInlineImageData::ToPdf() const
			{
				BeginInlineImageDataOperatorPtr op;
				BufferPtr buffer = op->Value();
				return buffer->ToString();
			}

			std::string OperationEndInlineImageObject::ToPdf() const
			{
				EndInlineImageObjectOperatorPtr op;
				BufferPtr buffer = op->Value();
				return buffer->ToString();
			}
		}
	}
}
