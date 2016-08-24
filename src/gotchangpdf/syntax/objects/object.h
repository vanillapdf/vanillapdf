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
			bool IsIndirect(void) const noexcept { return m_indirect; }
			void SetIndirect(bool indirect = true) noexcept { m_indirect = indirect; }

			bool IsDirty(void) const noexcept { return m_dirty; }
			void SetDirty(bool dirty = true) noexcept { m_dirty = dirty; }

			bool IsEncryptionExempted() const noexcept { return m_encryption_exempted; }
			void SetEncryptionExempted(bool exempted = true) { m_encryption_exempted = exempted; }

			void SetOffset(types::stream_offset offset) noexcept { m_offset = offset; }
			types::stream_offset GetOffset() const noexcept { return m_offset; }

			virtual void SetObjectNumber(types::big_uint number) noexcept { m_obj_number = number; }
			types::big_uint GetObjectNumber() const noexcept { return m_obj_number; }

			virtual void SetGenerationNumber(types::ushort number) noexcept { m_gen_number = number; }
			types::ushort GetGenerationNumber() const noexcept { return m_gen_number; }

			virtual void SetFile(std::weak_ptr<File> file) noexcept { m_file = file; }
			std::weak_ptr<File> GetFile() const noexcept { return m_file; }

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
			std::weak_ptr<File> m_file;
			bool m_indirect = false;
			bool m_dirty = false;
			types::big_uint m_obj_number = 0;
			types::ushort m_gen_number = 0;
			types::stream_offset m_offset = std::_BADOFF;
			bool m_encryption_exempted = false;
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
