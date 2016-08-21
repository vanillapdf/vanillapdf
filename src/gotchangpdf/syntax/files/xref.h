#ifndef _XREF_H
#define _XREF_H

#include "syntax_fwd.h"
#include "unknown_interface.h"
#include "constants.h"

#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		class XrefUtils
		{
		public:
			template <typename T>
			static T ConvertTo(const XrefEntryBasePtr& obj)
			{
				auto ptr = obj.get();
				auto converted = dynamic_cast<typename T::value_type *>(ptr);
				if (nullptr == converted)
					throw ConversionExceptionFactory<T>::Construct(obj);

				return T(converted);
			}

			template <typename T>
			static bool IsType(const XrefEntryBasePtr& obj)
			{
				auto ptr = obj.get();
				auto converted = dynamic_cast<typename T::value_type *>(ptr);
				return (nullptr != converted);
			}
		};

		class XrefEntryBase : public IUnknown, public IModifyObservable
		{
		public:
			enum class Usage {
				Null = 0,
				Used,
				Free,
				Compressed
			};

		public:
			XrefEntryBase(types::big_uint obj_number, types::ushort gen_number)
				: _obj_number(obj_number), _gen_number(gen_number) {}

		public:
			types::big_uint GetObjectNumber(void) const noexcept { return _obj_number; }
			void SetObjectNumber(types::big_uint value) noexcept { _obj_number = value; OnChanged(); }

			types::ushort GetGenerationNumber(void) const noexcept { return _gen_number; }
			void SetGenerationNumber(types::ushort value) noexcept { _gen_number = value; OnChanged(); }

			virtual Usage GetUsage(void) const noexcept = 0;

			bool InUse(void) const noexcept { return GetUsage() == Usage::Compressed || GetUsage() == Usage::Used; }

			void SetFile(std::weak_ptr<File> file) noexcept { _file = file; }
			std::weak_ptr<File> GetFile() const noexcept { return _file; }

			bool IsDirty(void) const noexcept { return _dirty; }
			void SetDirty(bool dirty = true) noexcept { _dirty = dirty; }

			virtual ~XrefEntryBase() {};

		protected:
			std::weak_ptr<File> _file;
			types::big_uint _obj_number = 0;
			types::ushort _gen_number = 0;
			bool _dirty = false;

		private:
			// Private only for NullEntry
			XrefEntryBase() = default;
			friend class XrefNullEntry;
		};

		class XrefNullEntry : public XrefEntryBase
		{
		public:
			virtual Usage GetUsage(void) const noexcept override { return XrefEntryBase::Usage::Null; }
		};

		class XrefFreeEntry : public XrefEntryBase
		{
		public:
			XrefFreeEntry(types::big_uint obj_number, types::ushort gen_number)
				: XrefEntryBase(obj_number, gen_number) {}

		public:
			virtual Usage GetUsage(void) const noexcept override { return XrefEntryBase::Usage::Free; }

			types::big_uint GetNextFreeObjectNumber(void) const noexcept { return _obj_number; }
			void SetNextFreeObjectNumber(types::big_uint value) noexcept { _obj_number = value; OnChanged(); }
		};

		class XrefUsedEntry : public XrefEntryBase, public IModifyObserver
		{
		public:
			XrefUsedEntry(types::big_uint obj_number, types::ushort gen_number, types::stream_offset offset)
				: XrefEntryBase(obj_number, gen_number), _offset(offset) {}

		public:
			virtual Usage GetUsage(void) const noexcept override{ return XrefEntryBase::Usage::Used; }

			ObjectPtr GetReference(void) { Initialize(); return _reference; }
			void SetReference(ObjectPtr ref);

			types::stream_offset GetOffset(void) const noexcept { return _offset; }
			void SetOffset(types::stream_offset value) noexcept { _offset = value; OnChanged(); }

			virtual void ObserveeChanged(IModifyObservable*) override
			{
				if (m_initialized) {
					SetDirty();
				}

				// Notify observers
				OnChanged();
			}

			~XrefUsedEntry() { _reference->Unsubscribe(this); }

		private:
			void Initialize(void);

			ObjectPtr _reference = NullObject::GetInstance();
			types::stream_offset _offset = std::_BADOFF;
		};

		class XrefCompressedEntry : public XrefEntryBase, public IModifyObserver
		{
		public:
			XrefCompressedEntry(types::big_uint obj_number, types::ushort gen_number, types::big_uint object_stream_number, types::uinteger index)
				: XrefEntryBase(obj_number, gen_number), _object_stream_number(object_stream_number), _index(index) {}

		public:
			virtual Usage GetUsage(void) const noexcept override { return XrefEntryBase::Usage::Compressed; }

			ObjectPtr GetReference(void) { Initialize(); return _reference; }
			void SetReference(ObjectPtr ref);

			types::big_uint GetObjectStreamNumber(void) const noexcept { return _object_stream_number; }
			void SetObjectStreamNumber(types::uinteger value) noexcept { _object_stream_number = value; OnChanged(); }

			types::uinteger GetIndex(void) const noexcept { return _index; }
			void SetIndex(types::uinteger value) noexcept { _index = value; OnChanged(); }

			virtual void ObserveeChanged(IModifyObservable*) override
			{
				if (m_initialized) {
					SetDirty();
				}

				// Notify observers
				OnChanged();
			}

			~XrefCompressedEntry() { _reference->Unsubscribe(this); }

		private:
			void Initialize(void);

			ObjectPtr _reference = NullObject::GetInstance();
			types::big_uint _object_stream_number = 0;
			types::uinteger _index = 0;
		};

		class XrefBase : public IUnknown, public IModifyObserver
		{
		public:
			enum class Type
			{
				Table = 0,
				Stream
			};

			bool IsInitialized(void) const noexcept { return m_initialized; }
			void SetInitialized(bool initialized = true) noexcept { m_initialized = initialized; }

			virtual void ObserveeChanged(IModifyObservable*) override { OnEntryChanged(); }

			void SetFile(std::weak_ptr<File>file) noexcept { _file = file; }
			std::weak_ptr<File> GetFile() const noexcept { return _file; }

			DictionaryObjectPtr GetTrailerDictionary(void) const { return _trailer_dictionary; }
			void SetTrailerDictionary(DictionaryObjectPtr dictionary) { _trailer_dictionary = dictionary; }

			types::stream_offset GetOffset() const noexcept { return _offset; }
			void SetOffset(types::stream_offset offset) noexcept { _offset = offset; }

			types::stream_offset GetLastXrefOffset() const noexcept { return _last_xref_offset; }
			void SetLastXrefOffset(types::stream_offset offset) noexcept { _last_xref_offset = offset; }

			void Add(XrefEntryBasePtr entry);
			bool Remove(types::big_uint obj_number, types::ushort gen_number);
			size_t Size(void) const noexcept;
			XrefEntryBasePtr Find(types::big_uint obj_number, types::ushort gen_number) const;
			bool Contains(types::big_uint obj_number, types::ushort gen_number) const;
			std::vector<XrefEntryBasePtr> Entries(void) const;

			virtual Type GetType(void) const noexcept = 0;
			virtual ~XrefBase() {};

		protected:
			struct Key
			{
				Key() = default;
				Key(types::big_uint obj, types::ushort gen) : obj_number(obj), gen_number(gen) {}
				types::big_uint obj_number = 0;
				types::ushort gen_number = 0;

				bool operator<(const Key& other) const
				{
					if (obj_number != other.obj_number)
						return obj_number < other.obj_number;

					if (gen_number != other.gen_number)
						return gen_number < other.gen_number;

					return false;
				}
			};

		protected:
			std::weak_ptr<File> _file;
			std::map<Key, XrefEntryBasePtr> _entries;
			types::stream_offset _last_xref_offset = std::_BADOFF;
			types::stream_offset _offset = std::_BADOFF;
			DictionaryObjectPtr _trailer_dictionary;
			bool m_initialized = false;

			virtual void OnEntryChanged() {}
		};

		class XrefTable : public XrefBase
		{
		public:
			virtual Type GetType(void) const noexcept override { return XrefBase::Type::Table; }
		};

		class XrefStream : public XrefBase
		{
		public:
			virtual Type GetType(void) const noexcept override { return XrefBase::Type::Stream; }

			StreamObjectPtr GetStreamObject(void) const { return _stream; }
			void SetStreamObject(const StreamObject& stream)
			{
				_stream->Unsubscribe(this);
				_stream = stream;
				_stream->Subscribe(this);
			}

		private:
			StreamObjectPtr _stream;

			void WriteValue(std::ostream& dest, types::big_uint value, int64_t width);
			virtual void OnEntryChanged() override;
		};
	}
}

#endif /* _XREF_H */
