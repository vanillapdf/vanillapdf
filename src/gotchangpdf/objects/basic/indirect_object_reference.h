#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "fwd.h"
#include "indirect_object.h"

#include <memory>

namespace gotchangpdf
{
	class IndirectObjectReference : public Object
	{
	public:
		explicit IndirectObjectReference(std::shared_ptr<files::File> file);
		IndirectObjectReference(std::shared_ptr<files::File> file, int obj_number, int gen_number);

		ObjectReferenceWrapper<IndirectObject> GetReferencedObject() const;

	private:
		std::shared_ptr<files::File> _file;
		mutable ObjectReferenceWrapper<IndirectObject> _reference;

		int _obj_number, _gen_number;
	};
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
