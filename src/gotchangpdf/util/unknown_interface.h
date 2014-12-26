#ifndef _UNKNOWN_INTERFACE_H
#define _UNKNOWN_INTERFACE_H

#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace gotchangpdf
{
	class IUnknown : public boost::intrusive_ref_counter<IUnknown>
	{
	public:
		inline void AddRef() const { boost::sp_adl_block::intrusive_ptr_add_ref(this); }
		inline void Release() const { boost::sp_adl_block::intrusive_ptr_release(this); }
	};
}

#endif /* _UNKNOWN_INTERFACE_H */
