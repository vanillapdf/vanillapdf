#ifndef _I_BUFFER_H
#define _I_BUFFER_H

#include "i_object.h"
#include "Character.h"

namespace gotchangpdf
{
	class IBuffer
	{
	public:
		virtual Character At(long at) const = 0;
		virtual int Size() const = 0;

		virtual bool operator==(const IBuffer& other) const = 0;
		virtual bool operator<(const IBuffer& other) const = 0;
		virtual Character operator[](unsigned int i) const = 0;
	};
}

#endif /* _I_BUFFER_H */
