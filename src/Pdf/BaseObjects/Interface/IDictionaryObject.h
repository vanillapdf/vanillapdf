#ifndef _I_DICTIONARY_OBJECT_H
#define _I_DICTIONARY_OBJECT_H

#include "Interface/IObject.h"
#include "Interface/INameObject.h"

#include "boost/intrusive_ptr.hpp"

namespace Pdf
{
	class IDictionaryObject
	{
	public:
		virtual IObject* IObjectFind(const INameObject& name) const = 0;
		virtual IObject* IObjectFind(const char* name, int len) const = 0;
	};
}

#endif /* _I_DICTIONARY_OBJECT_H */
