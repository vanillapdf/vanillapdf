#ifndef _INDIRECT_OBJECT_H
#define _INDIRECT_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "constants.h"
#include "smart_ptr.h"
#include "version.h"

namespace gotchangpdf
{
	class IndirectObject : public Object, public RequireVersion<Version::PDF12>
	{
	public:
		IndirectObject(files::File * file, int objNumber, int genNumber,
			streamOffsetValueType offset = std::_BADOFF);
		IndirectObject(const IndirectObject& other);

		void SetObject(SmartPtr<Object> ref);
		SmartPtr<Object> GetObject() const;

		template <typename T>
		SmartPtr<T> GetObjectAs() const;

		void SetOffset(streamOffsetValueType offset);
		streamOffsetValueType GetOffset() const;

		virtual Object::Type GetType(void) const override;

		/* IIndirectObject */
		//virtual IObject* GetIObject() const override;
		//virtual streamOffsetValueType GetOffset() const override;

	private:
		files::File * _file;
		int _objNumber;
		int _genNumber;
		streamOffsetValueType _offset;

		mutable SmartPtr<Object> _reference;
	};

	inline void IndirectObject::SetOffset(streamOffsetValueType offset)
	{
		_offset = offset;
	}

	inline streamOffsetValueType IndirectObject::GetOffset() const
	{
		return _offset;
	}

	inline Object::Type IndirectObject::GetType(void) const
	{
		return Object::Type::IndirectObject;
	}

	template <typename T>
	SmartPtr<T> IndirectObject::GetObjectAs() const
	{
		return dynamic_wrapper_cast<T>(GetObject());
	}
}

#endif /* _INDIRECT_OBJECT_H */
