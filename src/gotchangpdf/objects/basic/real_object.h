#ifndef _REAL_OBJECT_H
#define _REAL_OBJECT_H

#include "fwd.h"
#include "numeric_object.h"

namespace gotchangpdf
{
	class RealObject : public NumericObject
	{
	public:
		RealObject();
		explicit RealObject(double value);
		explicit RealObject(const lexical::Token& token);

		//virtual Object* Clone(void) const  override { return new RealObject(static_cast<RealObject const&>(*this)); };

	private:
		double _value;

		template <typename T>
		friend void ::boost::intrusive_ptr_add_ref(T*);

		template <typename T>
		friend void ::boost::intrusive_ptr_release(T*);
	};
}

#endif /* _REAL_OBJECT_H */
