#ifndef _CONTENT_STREAM_INSTRUCTION_BASE_H
#define _CONTENT_STREAM_INSTRUCTION_BASE_H

#include "syntax_fwd.h"
#include "unknown_interface.h"

#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			class InstructionBase : public IUnknown
			{
			public:
				enum class Type
				{
					Object,
					Operation
				};

				virtual Type GetInstructionType(void) const _NOEXCEPT = 0;
			};

			typedef std::vector<syntax::contents::InstructionBasePtr> BaseInstructionCollection;
		}
	}
}

#endif /* _CONTENT_STREAM_INSTRUCTION_BASE_H */