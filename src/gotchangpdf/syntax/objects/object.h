#ifndef _OBJECT_H
#define _OBJECT_H

#include "syntax_fwd.h"
#include "constants.h"
#include "unknown_interface.h"
#include "modify_observer_interface.h"

#include <iosfwd>
#include <memory>
#include <string>

namespace gotchangpdf
{
	namespace syntax
	{
		class Object : public IUnknown, public IModifyObservable
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
			virtual Type GetType(void) const noexcept = 0;
			virtual std::string ToString(void) const { return ToPdf(); }
			virtual std::string ToPdf(void) const = 0;

		public:
			bool IsIndirect(void) const noexcept { return _indirect; }
			void SetIndirect(bool indirect = true) noexcept { _indirect = indirect; }

			bool IsDirty(void) const noexcept { return _dirty; }
			void SetDirty(bool dirty = true) noexcept { _dirty = dirty; }

			void SetOffset(types::stream_offset offset) noexcept { _offset = offset; }
			types::stream_offset GetOffset() const noexcept { return _offset; }

			virtual void SetObjectNumber(types::big_uint number) noexcept { _obj_number = number; }
			types::big_uint GetObjectNumber() const noexcept { return _obj_number; }

			virtual void SetGenerationNumber(types::ushort number) noexcept { _gen_number = number; }
			types::ushort GetGenerationNumber() const noexcept { return _gen_number; }

			virtual void SetFile(std::weak_ptr<File> file) noexcept { _file = file; }
			std::weak_ptr<File> GetFile() const noexcept { return _file; }

			virtual Object* Clone(void) const = 0;

			virtual void OnChanged() override
			{
				if (!m_initialized) {
					return;
				}

				SetDirty();
				IModifyObservable::OnChanged();
			}

		protected:
			std::weak_ptr<File> _file;
			bool _indirect = false;
			bool _dirty = false;
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
