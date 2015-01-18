#ifndef _REAL_OBJECT_H
#define _REAL_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	class RealObject : public Containable, public Object
	{
	public:
		typedef types::real value_type;

	public:
		RealObject();
		explicit RealObject(double value);

		virtual inline Object::Type GetType(void) const override { return Object::Type::Real; }

	//private:
	public:
		types::real _value = 0;
	};
}

#endif /* _REAL_OBJECT_H */
