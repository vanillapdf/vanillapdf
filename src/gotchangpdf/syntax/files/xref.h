#ifndef _XREF_H
#define _XREF_H

#include "syntax_fwd.h"
#include "unknown_interface.h"
#include "constants.h"

#include <vector>
#include <unordered_map>

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

			bool operator<(const XrefEntryBase& other) const;

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

		class XrefUsedEntryBase : public XrefEntryBase
		{
		public:
			using XrefEntryBase::XrefEntryBase;

		public:
			virtual ObjectPtr GetReference(void) = 0;
			virtual void SetReference(ObjectPtr ref) = 0;
		};

		class XrefUsedEntry : public XrefUsedEntryBase, public IModifyObserver
		{
		public:
			XrefUsedEntry(types::big_uint obj_number, types::ushort gen_number, types::stream_offset offset)
				: XrefUsedEntryBase(obj_number, gen_number), _offset(offset) {}

		public:
			virtual Usage GetUsage(void) const noexcept override{ return XrefEntryBase::Usage::Used; }

			virtual ObjectPtr GetReference(void) override { Initialize(); return _reference; }
			virtual void SetReference(ObjectPtr ref) override;

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
			types::stream_offset _offset = constant::BAD_OFFSET;
		};

		class XrefCompressedEntry : public XrefUsedEntryBase, public IModifyObserver
		{
		public:
			XrefCompressedEntry(types::big_uint obj_number, types::ushort gen_number, types::big_uint object_stream_number, types::uinteger index)
				: XrefUsedEntryBase(obj_number, gen_number), _object_stream_number(object_stream_number), _index(index) {}

		public:
			virtual Usage GetUsage(void) const noexcept override { return XrefEntryBase::Usage::Compressed; }

			virtual ObjectPtr GetReference(void) override { Initialize(); return _reference; }
			virtual void SetReference(ObjectPtr ref) override;

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

		class XrefBase : public IUnknown, public IModifyObserver, public IModifyObservable
		{
		public:
			using map_type = std::unordered_map<types::big_uint, XrefEntryBasePtr>;

		public:
			class Iterator : public IUnknown
			{
			public:
				typedef map_type::const_iterator::value_type value_type;
				typedef map_type::const_iterator::difference_type difference_type;
				typedef map_type::const_iterator::pointer pointer;
				typedef map_type::const_iterator::reference reference;
				typedef map_type::const_iterator::iterator_category iterator_category;

			public:
				Iterator() = default;
				Iterator(map_type::const_iterator it) : _it(it) {}

				const Iterator& operator++()
				{
					++_it;
					return *this;
				}

				const Iterator operator++(int)
				{
					Iterator temp(_it);
					++_it;
					return temp;
				}

				map_type::key_type Key() const { return _it->first; }
				map_type::mapped_type Value() const { return _it->second; }

				bool operator==(const Iterator& other) const
				{
					return _it == other._it;
				}

			private:
				map_type::const_iterator _it;
			};

			using IteratorPtr = DeferredIterator<Iterator>;

		public:
			enum class Type
			{
				Table = 0,
				Stream
			};

			bool IsInitialized(void) const noexcept { return m_initialized; }
			void SetInitialized(bool initialized = true) noexcept { m_initialized = initialized; }

			virtual void ObserveeChanged(IModifyObservable*) override;

			void SetFile(std::weak_ptr<File>file) noexcept { _file = file; }
			std::weak_ptr<File> GetFile() const noexcept { return _file; }

			DictionaryObjectPtr GetTrailerDictionary(void) const { return _trailer_dictionary; }
			void SetTrailerDictionary(DictionaryObjectPtr dictionary) { _trailer_dictionary = dictionary; }

			types::stream_offset GetOffset() const noexcept { return _offset; }
			void SetOffset(types::stream_offset offset) noexcept { _offset = offset; }

			types::stream_offset GetLastXrefOffset() const noexcept { return _last_xref_offset; }
			void SetLastXrefOffset(types::stream_offset offset) noexcept { _last_xref_offset = offset; }

			bool IsDirty(void) const noexcept { return m_dirty; }
			void SetDirty(bool dirty = true) noexcept { m_dirty = dirty; }

			IteratorPtr Begin(void) const noexcept { return _entries.begin(); }
			IteratorPtr End(void) const noexcept { return _entries.end(); }

			void Add(XrefEntryBasePtr entry);
			bool Remove(types::big_uint obj_number);
			size_t Size(void) const noexcept;
			XrefEntryBasePtr Find(types::big_uint obj_number) const;
			bool Contains(types::big_uint obj_number) const;
			std::vector<XrefEntryBasePtr> Entries(void) const;

			virtual Type GetType(void) const noexcept = 0;
			virtual ~XrefBase();

		protected:
			std::weak_ptr<File> _file;
			map_type _entries;
			types::stream_offset _last_xref_offset = constant::BAD_OFFSET;
			types::stream_offset _offset = constant::BAD_OFFSET;
			DictionaryObjectPtr _trailer_dictionary;
			bool m_initialized = false;
			bool m_dirty = false;
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
			void SetStreamObject(StreamObjectPtr stream)
			{
				_stream->Unsubscribe(this);
				_stream = stream;
				_stream->Subscribe(this);

				auto stream_header = stream->GetHeader();
				SetTrailerDictionary(stream_header);
			}

			void RecalculateContent();

			~XrefStream();

		private:
			StreamObjectPtr _stream;

			void WriteValue(std::ostream& dest, types::big_uint value, int64_t width);
		};

		inline bool operator<(const Deferred<syntax::XrefEntryBase>& left, const Deferred<syntax::XrefEntryBase>& right)
		{
			return *left < *right;
		}
	}
}

#endif /* _XREF_H */
