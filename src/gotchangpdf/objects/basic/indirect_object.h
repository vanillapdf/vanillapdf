#ifndef _INDIRECT_OBJECT_H
#define _INDIRECT_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "objects.h"
#include "direct_object.h"
#include "constants.h"
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

		inline void SetOffset(types::stream_offset offset) { _offset = offset; }
		inline types::stream_offset GetOffset() const { return _offset; }

		inline void SetObjectNumber(IntegerObjectPtr number){ _obj_number = number; }
		inline IntegerObjectPtr GetObjectNumber() const { return _obj_number; }

		inline void SetGenerationNumber(IntegerObjectPtr number){ _gen_number = number; }
		inline IntegerObjectPtr GetGenerationNumber() const { return _gen_number; }

		inline void SetFile(files::File *file) { _file = file; }
		inline files::File* GetFile() const { return _file; }

		virtual inline Object::Type GetType(void) const override { return Object::Type::IndirectObject; }

		/* IIndirectObject */
		//virtual IObject* GetIObject() const override;
		//virtual streamOffsetValueType GetOffset() const override;

	public:
		IntegerObjectPtr _obj_number;
		IntegerObjectPtr _gen_number;
		types::stream_offset _offset = std::_BADOFF;

		mutable DirectObject _reference;

	private:
		files::File * _file = nullptr;
		//mutable Deferred<Object> _reference;

		explicit IndirectObject() = default;

		friend Deferred<IndirectObject>;

		template <typename T>
		friend T* Allocate();
	};

	template <typename T>
	const T IndirectObject::GetObjectAs() const
	{
		auto obj = GetObject();
		ObjectVisitor<T> visitor;

		return obj.apply_visitor(visitor);
	}
}

#endif /* _INDIRECT_OBJECT_H */
