#ifndef _FILTER_H
#define _FILTER_H

#include "BaseObjects/Object.h"

namespace Pdf
{
	class Filter : public Object
	{
	public:
		enum class Type : unsigned char
		{
			ASCII,
			Decompression
		};

	protected:
		Type _type;
	};
}

#endif /* _FILTER_H */
