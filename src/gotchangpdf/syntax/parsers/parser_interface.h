#ifndef _PARSER_INTERFACE_H
#define _PARSER_INTERFACE_H

#include "syntax_fwd.h"
#include "constants.h"
#include "object_stream_entry.h"
#include "content_stream_instruction_base.h"

#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		class IParser
		{
		public:
			virtual XrefBasePtr ReadXref(void) = 0;
			virtual XrefBasePtr ReadXref(types::stream_offset offset) = 0;

			virtual ObjectStreamEntries ReadObjectStreamEntries(types::big_uint first, size_t size) = 0;

			virtual ~IParser() {}
		};

		class IReverseParser
		{
		public:
			virtual types::stream_offset ReadLastXrefOffset() = 0;

			virtual ~IReverseParser() {}
		};

		class IContentStreamParser
		{
		public:
			virtual contents::BaseInstructionCollectionPtr ReadContentStreamInstructions(void) = 0;

			virtual ~IContentStreamParser() {}
		};
	}
}

#endif /* _PARSER_INTERFACE_H */
