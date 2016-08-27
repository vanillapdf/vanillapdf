#ifndef _PARSER_INTERFACE_H
#define _PARSER_INTERFACE_H

#include "syntax_fwd.h"
#include "constants.h"
#include "object_stream_header.h"
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

			virtual ObjectPtr ReadDirectObject(void) = 0;
			virtual ObjectPtr ReadDirectObject(types::stream_offset offset) = 0;

			virtual std::vector<ObjectPtr> ReadObjectStreamEntries(types::big_uint first, size_t size) = 0;
			virtual contents::BaseInstructionCollectionPtr ReadContentStreamInstructions(void) = 0;

			virtual ~IParser() {}
		};
	}
}

#endif /* _PARSER_INTERFACE_H */
