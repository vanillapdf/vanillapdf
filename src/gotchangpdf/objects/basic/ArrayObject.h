#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "Fwd.h"
#include "Object.h"
#include "IArrayObject.h"

#include "boost/intrusive_ptr.hpp"

#include <vector>

namespace gotchangpdf
{
	class ArrayObject : public Object, public IArrayObject
	{
	public:
		ArrayObject();

		virtual long Size(void) const override;
		virtual IObject* At(long at) const override;

		friend lexical::Parser& operator>> (lexical::Parser& s, ArrayObject& o);

	private:
		std::vector<boost::intrusive_ptr<Object>> _list;

		friend void ::boost::intrusive_ptr_add_ref(ArrayObject*);
		friend void ::boost::intrusive_ptr_release(ArrayObject*);
	};
}

#endif /* _ARRAY_OBJECT_H */
