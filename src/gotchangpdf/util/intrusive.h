#ifndef _INTRUSIVE_H
#define _INTRUSIVE_H

#include "boost/intrusive_ptr.hpp"
#include "boost/smart_ptr/intrusive_ref_counter.hpp"

namespace boost
{
	/*
	template<typename T>
	void intrusive_ptr_add_ref(T* obj)
	{
		++obj->_intrusive_ref_count;
	}

	template<typename T>
	void intrusive_ptr_release(T* obj)
	{
		if (--obj->_intrusive_ref_count == 0)
			delete obj;
	}
	*/
}

#endif /* _INTRUSIVE_H */
