#ifndef _OBJECT_H
#define _OBJECT_H

#include "fwd.h"
#include "constants.h"
#include "unknown_interface.h"

#include <iosfwd>

namespace gotchangpdf
{
	class Object : public IUnknown
	{
	public:
		enum class Type : unsigned char
		{
			Array,
			Boolean,
			Dictionary,
			Function,
			Integer,
			Name,
			Null,
			Real,
			Stream,
			HexadecimalString,
			LiteralString,
			IndirectReference
		};

		static const char* TypeName(Type type) _NOEXCEPT;

	public:
		Object() = default;
		virtual ~Object() = 0;

		virtual inline Type GetType(void) const _NOEXCEPT = 0;

	public:
		inline bool IsIndirect(void) const _NOEXCEPT { return _obj_number > 0; }

		inline void SetOffset(types::stream_offset offset) _NOEXCEPT { _offset = offset; }
		inline types::stream_offset GetOffset() const _NOEXCEPT { return _offset; }

		inline void SetObjectNumber(types::integer number) _NOEXCEPT { _obj_number = number; }
		inline types::integer GetObjectNumber() const _NOEXCEPT { return _obj_number; }

		inline void SetGenerationNumber(types::ushort number) _NOEXCEPT { _gen_number = number; }
		inline types::ushort GetGenerationNumber() const _NOEXCEPT { return _gen_number; }

		inline void SetFile(files::File *file) _NOEXCEPT{ _file = file; }
		inline files::File* GetFile() const _NOEXCEPT { return _file; }

	protected:
		files::File * _file = nullptr;
		types::integer _obj_number;
		types::ushort _gen_number;
		types::stream_offset _offset = std::_BADOFF;
	};

	inline Object::~Object() {}
}

#endif /* _OBJECT_H */
