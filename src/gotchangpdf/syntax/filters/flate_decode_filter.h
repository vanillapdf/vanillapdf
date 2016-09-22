#ifndef _FLATE_DECODE_FILTER_H
#define _FLATE_DECODE_FILTER_H

#include "filter.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class FlateDecodeFilter : public FilterBase
		{
		public:
			virtual Type GetType(void) const noexcept override { return FilterBase::Type::FlateDecode; }

			virtual BufferPtr Encode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;
			virtual BufferPtr Decode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;

			virtual BufferPtr Encode(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;
			virtual BufferPtr Decode(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;

			BufferPtr ApplyPredictor(BufferPtr src, DictionaryObjectPtr parameters) const;
			BufferPtr ApplyPredictor(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters) const;
		};
	}
}

#endif /* _FLATE_DECODE_FILTER_H */
