#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "Fwd.h"
#include "IndirectObject.h"

#include "boost/intrusive_ptr.hpp"

#include <memory>

namespace gotchangpdf
{
	class IndirectObjectReference : public Object
	{
	public:
		explicit IndirectObjectReference(std::shared_ptr<files::File> file);
		IndirectObjectReference(std::shared_ptr<files::File> file, int obj_number, int gen_number);

		boost::intrusive_ptr<IndirectObject> GetReference() const;

	private:
		std::shared_ptr<files::File> _file;
		mutable boost::intrusive_ptr<IndirectObject> _reference;

		int _obj_number, _gen_number;

		friend void ::boost::intrusive_ptr_add_ref(IndirectObjectReference*);
		friend void ::boost::intrusive_ptr_release(IndirectObjectReference*);
	};
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
