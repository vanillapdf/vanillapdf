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

		class XrefEntryBase : public IUnknown
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
			types::big_uint GetObjectNumber(void) const _NOEXCEPT { return _obj_number; }
			void SetObjectNumber(types::big_uint value) _NOEXCEPT { _obj_number = value; }

			types::ushort GetGenerationNumber(void) const _NOEXCEPT { return _gen_number; }
			void SetGenerationNumber(types::ushort value) _NOEXCEPT { _gen_number = value; }

			virtual Usage GetUsage(void) const _NOEXCEPT = 0;

			bool InUse(void) const _NOEXCEPT{ return GetUsage() == Usage::Compressed || GetUsage() == Usage::Used; }

			void SetFile(std::weak_ptr<File> file) _NOEXCEPT { _file = file; }
			std::weak_ptr<File> GetFile() const _NOEXCEPT { return _file; }

			virtual ~XrefEntryBase() {};

		protected:
			std::weak_ptr<File> _file;
			types::big_uint _obj_number = 0;
			types::ushort _gen_number = 0;

		private:
			// Private only for NullEntry
			XrefEntryBase() = default;
			friend class XrefNullEntry;
		};

		class XrefNullEntry : public XrefEntryBase
		{
		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override { return XrefEntryBase::Usage::Null; }
		};

		class XrefFreeEntry : public XrefEntryBase
		{
		public:
			XrefFreeEntry(types::big_uint obj_number, types::ushort gen_number, types::big_uint next)
				: XrefEntryBase(obj_number, gen_number), _next(next) {}

		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override { return XrefEntryBase::Usage::Free; }

			types::big_uint GetNextFreeObjectNumber(void) const _NOEXCEPT { return _next; }
			void SetNextFreeObjectNumber(types::big_uint value) _NOEXCEPT { _next = value; }

		private:
			types::big_uint _next = 0;
		};

		class XrefUsedEntry : public XrefEntryBase
		{
		public:
			XrefUsedEntry(types::big_uint obj_number, types::ushort gen_number, types::stream_offset offset)
				: XrefEntryBase(obj_number, gen_number), _offset(offset) {}

		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override{ return XrefEntryBase::Usage::Used; }

			ObjectPtr GetReference(void) { Initialize(); return _reference; }
			void SetReference(ObjectPtr ref) { _reference = ref; }

			types::stream_offset GetOffset(void) const _NOEXCEPT { return _offset; }
			void SetOffset(types::stream_offset value) _NOEXCEPT { _offset = value; }

			bool Initialized(void) const { return _initialized; }
			void SetInitialized(bool value) { _initialized = value; }

		private:
			void Initialize(void);

			ObjectPtr _reference = NullObject::GetInstance();
			types::stream_offset _offset = std::_BADOFF;
			bool _initialized = false;
		};

		class XrefCompressedEntry : public XrefEntryBase
		{
		public:
			XrefCompressedEntry(types::big_uint obj_number, types::ushort gen_number, types::big_uint object_stream_number, types::uinteger index)
				: XrefEntryBase(obj_number, gen_number), _object_stream_number(object_stream_number), _index(index) {}

		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override { return XrefEntryBase::Usage::Compressed; }

			ObjectPtr GetReference(void) { Initialize(); return _reference; }
			void SetReference(ObjectPtr ref) { _reference = ref; }

			types::big_uint GetObjectStreamNumber(void) const _NOEXCEPT { return _object_stream_number; }
			void SetObjectStreamNumber(types::uinteger value) _NOEXCEPT { _object_stream_number = value; }

			types::uinteger GetIndex(void) const _NOEXCEPT { return _index; }
			void SetIndex(types::uinteger value) _NOEXCEPT { _index = value; }

			bool Initialized(void) const { return _initialized; }
			void SetInitialized(bool value) { _initialized = value; }

		private:
			void Initialize(void);

			ObjectPtr _reference = NullObject::GetInstance();
			types::big_uint _object_stream_number = 0;
			types::uinteger _index = 0;
			bool _initialized = false;
		};

		class XrefBase : public IUnknown
		{
		public:
			enum class Type
			{
				Table = 0,
				Stream
			};

			void SetFile(std::weak_ptr<File>file) _NOEXCEPT { _file = file; }
			std::weak_ptr<File> GetFile() const _NOEXCEPT { return _file; }

			DictionaryObjectPtr GetTrailerDictionary(void) const { return _trailer_dictionary; }
			void SetTrailerDictionary(DictionaryObjectPtr dictionary) { _trailer_dictionary = dictionary; }

			types::stream_offset GetLastXrefOffset() const _NOEXCEPT { return _last_xref_offset; }
			void SetLastXrefOffset(types::stream_offset offset) _NOEXCEPT { _last_xref_offset = offset; }

			void Add(XrefEntryBasePtr entry)
			{
				Key key(entry->GetObjectNumber(), entry->GetGenerationNumber());
				std::pair<Key, XrefEntryBasePtr> pair(key, entry);
				auto found = _entries.find(key);
				if (found != _entries.end()) {
					_entries.erase(found);
				}

				_entries.insert(pair);
			}

			size_t Size(void) const _NOEXCEPT { return _entries.size(); }
			XrefEntryBasePtr Find(types::big_uint obj_number, types::ushort gen_number)
			{
				Key key(obj_number, gen_number);
				auto found = _entries.find(key);
				if (found == _entries.end()) {
					throw ObjectMissingException(obj_number, gen_number);
				}

				return found->second;
			}

			bool Contains(types::big_uint obj_number, types::ushort gen_number)
			{
				Key key(obj_number, gen_number);
				auto found = _entries.find(key);
				return (found != _entries.end());
			}

			std::vector<XrefEntryBasePtr> Entries(void) const
			{
				std::vector<XrefEntryBasePtr> result;
				result.reserve(_entries.size());
				std::for_each(_entries.begin(), _entries.end(), [&result](const std::pair<Key, XrefEntryBasePtr> pair) { result.push_back(pair.second); });
				return result;
			}

			virtual Type GetType(void) const _NOEXCEPT = 0;
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
			DictionaryObjectPtr _trailer_dictionary;
		};

		class XrefTable : public XrefBase
		{
		public:
			virtual Type GetType(void) const _NOEXCEPT override { return XrefBase::Type::Table; }
		};

		class XrefStream : public XrefBase
		{
		public:
			virtual Type GetType(void) const _NOEXCEPT override { return XrefBase::Type::Stream; }
		};
	}
}

#endif /* _XREF_H */
