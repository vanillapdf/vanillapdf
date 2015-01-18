#ifndef _SPIRIT_PARSER_H
#define _SPIRIT_PARSER_H

#include "fwd.h"
#include "lexical_stream.h"
#include "direct_object.h"

#include <memory>

namespace gotchangpdf
{
	namespace lexical
	{
		class SpiritParser : public lexical::Stream
		{
		public:
			SpiritParser(files::File * file, CharacterSource & stream);
			SpiritParser(const SpiritParser & other);

			template<typename T>
			T ReadDirectObjectWithType(types::stream_offset offset) { return DirectObjectGetAs<T>(ReadDirectObject(offset)); }

			DirectObject ReadDirectObject(types::stream_offset offset);
			//DirectObject ReadIndirectObject(types::stream_offset offset);

			files::File * file(void) const;

		private:
			class Impl;
			std::shared_ptr<Impl> _impl;
		};
	}
}

#endif /* _SPIRIT_PARSER_H */
