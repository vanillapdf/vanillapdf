#ifndef _INDIRECT_OBJECT_H
#define _INDIRECT_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "i_indirect_object.h"
#include "object_reference_wrapper.h"
#include "version.h"

#include <memory>

namespace gotchangpdf
{
	class IndirectObject : public Object, public IIndirectObject, public RequireVersion<Version::PDF12>
	{
	public:
		IndirectObject(std::shared_ptr<files::File> file, int objNumber, int genNumber, std::streamoff offset = std::_BADOFF);
		IndirectObject(const IndirectObject& other);

		void SetObject(ObjectReferenceWrapper<Object> ref);
		ObjectReferenceWrapper<Object> GetObject() const;
		void SetOffset(streamOffsetValueType offset);

		/* IIndirectObject */
		virtual IObject* GetIObject() const override;
		virtual streamOffsetValueType GetOffset() const override;

	private:
		std::shared_ptr<files::File> _file;
		int _objNumber;
		int _genNumber;
		streamOffsetValueType _offset;

		mutable ObjectReferenceWrapper<Object> _reference;

		friend void ::boost::intrusive_ptr_add_ref(IndirectObject*);
		friend void ::boost::intrusive_ptr_release(IndirectObject*);
	};
}

#endif /* _INDIRECT_OBJECT_H */
