#ifndef _SPIRIT_PARSER_H
#define _SPIRIT_PARSER_H

#include "syntax_fwd.h"
#include "xref.h"
#include "raw_stream.h"
#include "direct_object.h"
#include "object_visitors.h"
#include "object_stream_header.h"
#include "content_stream_operations.h"

#include <vector>
#include <memory>

namespace gotchangpdf
{
	namespace syntax
	{
		class SpiritParser : public Stream
		{
		public:
			SpiritParser(std::weak_ptr<File> file, CharacterSource & stream);
			SpiritParser(const SpiritParser & other);

			template<typename T>
			T ReadDirectObjectWithType(types::stream_offset offset)
			{
				auto direct = ReadDirectObject(offset);
				ObjectVisitor<T> visitor;
				auto converted = direct.apply_visitor(visitor);
				return converted;
			}

			template<typename T>
			T ReadDirectObjectWithType(void)
			{
				auto direct = ReadDirectObject();
				ObjectVisitor<T> visitor;
				auto converted = direct.apply_visitor(visitor);
				return converted;
			}

			std::vector<DirectObject> ReadObjectStreamEntries(types::integer first, types::integer size);
			ObjectStreamHeaders ReadObjectStreamHeaders(types::integer size);
			contents::OperationCollection ReadContentStreamOperations(void);

			Xref ReadXref(void);
			Xref ReadXref(types::stream_offset offset);

			DirectObject ReadDirectObject(void);
			DirectObject ReadDirectObject(types::stream_offset offset);

			types::integer ReadLastXrefOffset();

			std::weak_ptr<File> file(void) const;

		private:
			class Impl;
			std::shared_ptr<Impl> _impl;
		};
	}
}

#endif /* _SPIRIT_PARSER_H */
