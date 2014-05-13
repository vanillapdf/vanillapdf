#ifndef _INDIRECT_OBJECT_H
#define _INDIRECT_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "constants.h"
#include "object_reference_wrapper.h"
#include "version.h"

#include <memory>

namespace gotchangpdf
{
	class IndirectObject : public Object, public RequireVersion<Version::PDF12>
	{
	public:
		IndirectObject(std::shared_ptr<files::File> file, int objNumber, int genNumber, streamOffsetValueType offset = std::_BADOFF);
		IndirectObject(const IndirectObject& other);

		void SetObject(ObjectReferenceWrapper<Object> ref);
		ObjectReferenceWrapper<Object> GetObject() const;
		void SetOffset(streamOffsetValueType offset);
		streamOffsetValueType GetOffset() const;

		/* IIndirectObject */
		//virtual IObject* GetIObject() const override;
		//virtual streamOffsetValueType GetOffset() const override;

	private:
		std::shared_ptr<files::File> _file;
		int _objNumber;
		int _genNumber;
		streamOffsetValueType _offset;

		mutable ObjectReferenceWrapper<Object> _reference;
	};
}

#endif /* _INDIRECT_OBJECT_H */
