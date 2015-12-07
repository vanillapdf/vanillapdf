#ifndef _CONTENT_STREAM_OPERATIONS_H
#define _CONTENT_STREAM_OPERATIONS_H

#include "semantics_fwd.h"
#include "content_stream.h"

#include <vector>

namespace gotchangpdf
{
	namespace semantics
	{
		namespace contents
		{
			class OperationBeginText : public syntax::contents::OperationBase
			{
			public:
				inline virtual Type GetOperationType(void) const _NOEXCEPT override { return Type::BeginText; }
			};

			class OperationEndText : public syntax::contents::OperationBase
			{
			public:
				inline virtual Type GetOperationType(void) const _NOEXCEPT override { return Type::EndText; }
			};

			class OperationTextShow : public syntax::contents::OperationBase
			{
			public:
				explicit OperationTextShow(syntax::StringObjectPtr str) : _str(str) {}

				inline virtual Type GetOperationType(void) const _NOEXCEPT override { return Type::TextShow; }

				syntax::StringObjectPtr Value() const { return _str; }

			private:
				syntax::StringObjectPtr _str;
			};
		}
	}
}

#endif /* _CONTENT_STREAM_OPERATIONS_H */
