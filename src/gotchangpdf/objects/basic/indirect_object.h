#ifndef _INDIRECT_OBJECT_H
#define _INDIRECT_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "direct_object.h"
#include "constants.h"
#include "smart_ptr.h"
#include "version.h"

namespace gotchangpdf
{
	class IndirectObject : public Object, public RequireVersion<Version::PDF12>
	{
	public:
		explicit IndirectObject(files::File * file);
		IndirectObject(const IndirectObject& other);

		void SetObject(DirectObject ref);
		DirectObject GetObject() const;

		template <typename T>
		const T GetObjectAs() const;

		void SetOffset(types::stream_offset offset);
		types::stream_offset GetOffset() const;

		void SetObjectNumber(types::integer number);
		types::integer GetObjectNumber() const;

		void SetGenerationNumber(types::ushort number);
		types::ushort GetGenerationNumber() const;

		virtual Object::Type GetType(void) const override;

		/* IIndirectObject */
		//virtual IObject* GetIObject() const override;
		//virtual streamOffsetValueType GetOffset() const override;

	public:
		types::integer _obj_number = 0;
		types::ushort _gen_number = 0;
		types::stream_offset _offset = std::_BADOFF;
		mutable DirectObject _reference;

	private:
		files::File * _file = nullptr;
		//mutable SmartPtr<Object> _reference;
	};

	inline void IndirectObject::SetOffset(types::stream_offset offset)
	{
		_offset = offset;
	}

	inline types::stream_offset IndirectObject::GetOffset() const
	{
		return _offset;
	}

	inline void IndirectObject::SetObjectNumber(types::integer number)
	{
		_obj_number = number;
	}

	inline types::integer IndirectObject::GetObjectNumber() const
	{
		return _obj_number;
	}

	inline void IndirectObject::SetGenerationNumber(types::ushort number)
	{
		_gen_number = number;
	}

	inline types::ushort IndirectObject::GetGenerationNumber() const
	{
		return _gen_number;
	}

	inline Object::Type IndirectObject::GetType(void) const
	{
		return Object::Type::IndirectObject;
	}

	template <typename T>
	const T IndirectObject::GetObjectAs() const
	{
		auto obj = GetObject();
		ObjectVisitor<T> visitor;

		return obj.apply_visitor(visitor);
	}

	typedef SmartPtr<IndirectObject> IndirectObjectPtr;
}

#endif /* _INDIRECT_OBJECT_H */
