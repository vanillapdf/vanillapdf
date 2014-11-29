#ifndef _REAL_OBJECT_H
#define _REAL_OBJECT_H

#include "fwd.h"
#include "constants.h"
#include "object.h"
#include "containerable.h"

namespace gotchangpdf
{
	class RealObject : public Object, public ParentContainer<ContainerPtr>
	{
	public:
		typedef types::real value_type;

	public:
		RealObject();
		explicit RealObject(double value);
		explicit RealObject(const lexical::Token& token);

		virtual inline Object::Type GetType(void) const override { return Object::Type::RealObject; }

		//virtual Object* Clone(void) const  override { return new RealObject(static_cast<RealObject const&>(*this)); };

	//private:
	public:
		types::real _value = 0;
	};

	typedef Deferred<RealObject> RealObjectPtr;
}

#endif /* _REAL_OBJECT_H */
