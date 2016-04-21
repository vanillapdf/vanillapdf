#ifndef _OBJECT_H
#define _OBJECT_H

#include "syntax_fwd.h"
#include "constants.h"
#include "unknown_interface.h"

#include <iosfwd>
#include <memory>
#include <string>

namespace gotchangpdf
{
	namespace syntax
	{
		class Object : public IUnknown
		{
		public:
			enum class Type
			{
				Null = 0,
				Array,
				Boolean,
				Dictionary,
				Integer,
				Name,
				Real,
				Stream,
				String,
				IndirectReference
			};

			static const char* TypeName(Type type);

		public:
			virtual Type GetType(void) const _NOEXCEPT = 0;
			virtual std::string ToString(void) const { return ToPdf(); }
			virtual std::string ToPdf(void) const = 0;

		public:
			bool IsIndirect(void) const _NOEXCEPT { return _indirect; }
			void SetIndirect(bool indirect) _NOEXCEPT { _indirect = indirect; }

			void SetOffset(types::stream_offset offset) _NOEXCEPT { _offset = offset; }
			types::stream_offset GetOffset() const _NOEXCEPT { return _offset; }

			virtual void SetObjectNumber(types::big_uint number) _NOEXCEPT { _obj_number = number; }
			types::big_uint GetObjectNumber() const _NOEXCEPT { return _obj_number; }

			virtual void SetGenerationNumber(types::ushort number) _NOEXCEPT { _gen_number = number; }
			types::ushort GetGenerationNumber() const _NOEXCEPT { return _gen_number; }

			void SetFile(std::weak_ptr<File> file) _NOEXCEPT { _file = file; }
			std::weak_ptr<File> GetFile() const _NOEXCEPT { return _file; }

		protected:
			std::weak_ptr<File> _file;
			bool _indirect = false;
			types::big_uint _obj_number = 0;
			types::ushort _gen_number = 0;
			types::stream_offset _offset = std::_BADOFF;
		};

		class ObjectPtr : public Deferred<Object>
		{
			using Deferred<Object>::Deferred;

		public:
			ObjectPtr();
		};
	}
}

#endif /* _OBJECT_H */
