#ifndef _SPIRIT_PARSER_H
#define _SPIRIT_PARSER_H

#include "syntax_fwd.h"
#include "parser_interface.h"
#include "xref.h"
#include "raw_stream.h"
#include "object_stream_header.h"
#include "content_stream_operation_generic.h"

#include <vector>
#include <memory>

namespace gotchangpdf
{
	namespace syntax
	{
		class SpiritParser : public Stream, public IParser
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

			virtual std::vector<ObjectPtr> ReadObjectStreamEntries(types::integer first, types::integer size) override;
			contents::GenericOperationCollection ReadContentStreamOperations(void);

			virtual XrefBasePtr ReadXref(void) override;
			virtual XrefBasePtr ReadXref(types::stream_offset offset) override;

			virtual ObjectPtr ReadDirectObject(void) override;
			virtual ObjectPtr ReadDirectObject(types::stream_offset offset) override;

			types::integer ReadLastXrefOffset();

			std::weak_ptr<File> GetFile(void) const;

		private:
			class Impl;
			std::shared_ptr<Impl> _impl;

			ObjectStreamHeaders ReadObjectStreamHeaders(types::integer size);
		};
	}
}

#endif /* _SPIRIT_PARSER_H */
