#ifndef _ASCII_FILTER_H
#define _ASCII_FILTER_H

#include "Filter.h"

namespace Pdf
{
	class ASCIIFilter : public Filter
	{
	public:
		enum class Type : unsigned char
		{
			ASCIIHexDecode,
			ASCII85Decode
		};

		//virtual Object* Clone(void) const  override { return new ASCIIFilter(static_cast<ASCIIFilter const&>(*this)); };
	};
}

#endif /* _ASCII_FILTER_H */
