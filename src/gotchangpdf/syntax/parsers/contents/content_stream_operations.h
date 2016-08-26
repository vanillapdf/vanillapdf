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
				explicit OperationBeginText(const std::vector<ObjectPtr>& operands);

				virtual Type GetOperationType(void) const noexcept override { return Type::BeginText; }
				virtual std::string ToPdf() const override;
			};

			class OperationEndText : public OperationBase
			{
			public:
				explicit OperationEndText(const std::vector<ObjectPtr>& operands);

				virtual Type GetOperationType(void) const noexcept override { return Type::EndText; }
				virtual std::string ToPdf() const override;
			};

			class OperationTextShow : public OperationBase, public IModifyObserver
			{
			public:
				explicit OperationTextShow(const std::vector<ObjectPtr>& operands);

				virtual Type GetOperationType(void) const noexcept override { return Type::TextShow; }
				virtual std::string ToPdf() const override;
				StringObjectPtr GetValue() const { return _str; }
				void SetValue(StringObjectPtr value) { _str = value; OnChanged(); }

				virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

				~OperationTextShow();

			private:
				StringObjectPtr _str;
			};

			class OperationTextShowArray : public OperationBase, public IModifyObserver
			{
			public:
				explicit OperationTextShowArray(const std::vector<ObjectPtr>& operands);

				virtual Type GetOperationType(void) const noexcept override { return Type::TextShowArray; }
				virtual std::string ToPdf() const override;
				MixedArrayObjectPtr GetValue() const { return m_items; }
				void SetValue(MixedArrayObjectPtr value) { m_items = value; OnChanged(); }

				virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

				~OperationTextShowArray();

			private:
				MixedArrayObjectPtr m_items;
			};

			class OperationBeginInlineImageObject : public OperationBase
			{
			public:
				explicit OperationBeginInlineImageObject(const std::vector<ObjectPtr>& operands);
				virtual Type GetOperationType(void) const noexcept override { return Type::BeginInlineImageObject; }
				virtual std::string ToPdf() const override;
			};

			class OperationBeginInlineImageData : public OperationBase
			{
			public:
				explicit OperationBeginInlineImageData(const std::vector<ObjectPtr>& operands);
				virtual Type GetOperationType(void) const noexcept override { return Type::BeginInlineImageData; }
				virtual std::string ToPdf() const override;
			};

			class OperationEndInlineImageObject : public OperationBase
			{
			public:
				explicit OperationEndInlineImageObject(const std::vector<ObjectPtr>& operands);
				virtual Type GetOperationType(void) const noexcept override { return Type::EndInlineImageObject; }
				virtual std::string ToPdf() const override;
			};
		}
	}
}

#endif /* _CONTENT_STREAM_OPERATIONS_H */
