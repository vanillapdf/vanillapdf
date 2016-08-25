#ifndef _CONTENT_STREAM_OPERATIONS_H
#define _CONTENT_STREAM_OPERATIONS_H

#include "syntax_fwd.h"
#include "content_stream_operation_base.h"
#include "string_object.h"

#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			class OperationBeginText : public OperationBase
			{
			public:
				explicit OperationBeginText(const std::vector<ObjectPtr>& operands)
				{
					assert(operands.size() == 0); operands;
				}

				virtual Type GetOperationType(void) const noexcept override { return Type::BeginText; }
			};

			class OperationEndText : public OperationBase
			{
			public:
				explicit OperationEndText(const std::vector<ObjectPtr>& operands)
				{
					assert(operands.size() == 0); operands;
				}

				virtual Type GetOperationType(void) const noexcept override { return Type::EndText; }
			};

			class OperationTextShow : public OperationBase
			{
			public:
				explicit OperationTextShow(const std::vector<ObjectPtr>& operands)
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


				StringObjectPtr Value() const { return _str; }
				virtual Type GetOperationType(void) const noexcept override { return Type::TextShow; }

			private:
				StringObjectPtr _str;
			};

			class OperationBeginInlineImageObject : public OperationBase
			{
			public:
				explicit OperationBeginInlineImageObject(const std::vector<ObjectPtr>& operands)
				{
					assert(operands.size() == 0); operands;
				}

				virtual Type GetOperationType(void) const noexcept override { return Type::BeginInlineImageObject; }
			};

			class OperationBeginInlineImageData : public OperationBase
			{
			public:
				explicit OperationBeginInlineImageData(const std::vector<ObjectPtr>& operands)
				{
					assert(operands.size() == 0); operands;
				}

				virtual Type GetOperationType(void) const noexcept override { return Type::BeginInlineImageData; }
			};

			class OperationEndInlineImageObject : public OperationBase
			{
			public:
				explicit OperationEndInlineImageObject(const std::vector<ObjectPtr>& operands)
				{
					assert(operands.size() == 0); operands;
				}

				virtual Type GetOperationType(void) const noexcept override { return Type::EndInlineImageObject; }
			};
		}
	}
}

#endif /* _CONTENT_STREAM_OPERATIONS_H */
