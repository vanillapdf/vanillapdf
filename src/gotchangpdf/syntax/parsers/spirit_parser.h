#ifndef _SPIRIT_PARSER_H
#define _SPIRIT_PARSER_H

#include "syntax_fwd.h"
#include "xref.h"
#include "raw_stream.h"
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
				return ObjectUtils::ConvertTo<T>(direct);
			}

			template<typename T>
			T ReadDirectObjectWithType(void)
			{
				auto direct = ReadDirectObject();
				return ObjectUtils::ConvertTo<T>(direct);
			}

			std::vector<ObjectPtr> ReadObjectStreamEntries(types::integer first, types::integer size);
			ObjectStreamHeaders ReadObjectStreamHeaders(types::integer size);
			contents::GenericOperationCollection ReadContentStreamOperations(void);

			XrefBasePtr ReadXref(void);
			XrefBasePtr ReadXref(types::stream_offset offset);

			ObjectPtr ReadDirectObject(void);
			ObjectPtr ReadDirectObject(types::stream_offset offset);

			types::integer ReadLastXrefOffset();

			std::weak_ptr<File> file(void) const;

		private:
			class Impl;
			std::shared_ptr<Impl> _impl;
		};
	}
}

#endif /* _SPIRIT_PARSER_H */
