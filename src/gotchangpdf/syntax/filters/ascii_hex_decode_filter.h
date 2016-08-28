#ifndef _ASCII_HEX_DECODE_FILTER_H
#define _ASCII_HEX_DECODE_FILTER_H

#include "filter.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class ASCIIHexDecodeFilter : public FilterBase
		{
		public:
			virtual Type GetType(void) const noexcept override { return FilterBase::Type::ASCIIHexDecode; }

			virtual BufferPtr Encode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;
			virtual BufferPtr Decode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;

			virtual BufferPtr Encode(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;
			virtual BufferPtr Decode(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;
		};
	}
}

#endif /* _ASCII_HEX_DECODE_FILTER_H */
