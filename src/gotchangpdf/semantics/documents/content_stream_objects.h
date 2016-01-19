#ifndef _CONTENT_STREAM_OBJECTS_H
#define _CONTENT_STREAM_OBJECTS_H

#include "semantics_fwd.h"
#include "content_stream_instruction_base.h"

#include <vector>

namespace gotchangpdf
{
	namespace semantics
	{
		namespace contents
		{
			class ContentObjectBase : public syntax::contents::InstructionBase
			{
			public:
				enum class Type
				{
					TextObject
				};

				virtual Type GetType(void) const _NOEXCEPT = 0;
				virtual syntax::contents::InstructionBase::Type GetInstructionType(void) const _NOEXCEPT override { return InstructionBase::Type::Object; }
			};

			class TextObject : public ContentObjectBase
			{
			public:
				TextObject(syntax::contents::BaseOperationCollection ops) : _operations(ops) {}

				virtual Type GetType(void) const _NOEXCEPT override { return Type::TextObject; }

				types::uinteger GetOperationsSize(void) const { return _operations.size(); }
				syntax::contents::OperationBasePtr GetOperationAt(types::uinteger at) const { return _operations.at(at); }

			private:
				syntax::contents::BaseOperationCollection _operations;
			};
		}
	}
}

#endif /* _CONTENT_STREAM_OBJECTS_H */
