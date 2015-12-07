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
			namespace sync = syntax::contents;
			class ContentObjectBase : public syntax::contents::InstructionBase
			{
			public:
				enum class Type
				{
					TextObject
				};

				virtual Type GetType(void) const _NOEXCEPT = 0;
				inline virtual syntax::contents::InstructionBase::Type GetInstructionType(void) const _NOEXCEPT override { return InstructionBase::Type::Object; }
			};

			class TextObject : public ContentObjectBase
			{
			public:
				TextObject(sync::BaseOperationCollection ops) : _operations(ops) {}

				inline virtual Type GetType(void) const _NOEXCEPT override { return Type::TextObject; }

				types::uinteger GetOperationsSize(void) const { return _operations.size(); }
				sync::OperationBasePtr GetOperationAt(types::uinteger at) const { return _operations.at(at); }

			private:
				sync::BaseOperationCollection _operations;
			};
		}
	}
}

#endif /* _CONTENT_STREAM_OBJECTS_H */
