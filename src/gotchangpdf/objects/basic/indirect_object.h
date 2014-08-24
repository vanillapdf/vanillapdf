#ifndef _INDIRECT_OBJECT_H
#define _INDIRECT_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "constants.h"
#include "smart_ptr.h"
#include "version.h"

#include <memory>

namespace gotchangpdf
{
	class IndirectObject : public Object, public RequireVersion<Version::PDF12>
	{
	public:
		IndirectObject(std::shared_ptr<files::File> file, int objNumber, int genNumber, streamOffsetValueType offset = std::_BADOFF);
		IndirectObject(const IndirectObject& other);

		void SetObject(SmartPtr<Object> ref);
		SmartPtr<Object> GetObject() const;

		template <typename T>
		inline SmartPtr<T> GetObjectAs() const { return dynamic_wrapper_cast<T>(GetObject()); }

		void SetOffset(streamOffsetValueType offset);
		streamOffsetValueType GetOffset() const;

		virtual inline Object::Type GetType(void) const override { return Object::Type::IndirectObject; }

		/* IIndirectObject */
		//virtual IObject* GetIObject() const override;
		//virtual streamOffsetValueType GetOffset() const override;

	private:
		std::shared_ptr<files::File> _file;
		int _objNumber;
		int _genNumber;
		streamOffsetValueType _offset;

		mutable SmartPtr<Object> _reference;
	};
}

#endif /* _INDIRECT_OBJECT_H */
