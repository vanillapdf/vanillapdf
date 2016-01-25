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
			XrefEntryBase(types::uinteger obj_number, types::ushort gen_number)
				: _obj_number(obj_number), _gen_number(gen_number) {}

		public:
			types::uinteger GetObjectNumber(void) const _NOEXCEPT { return _obj_number; }
			void SetObjectNumber(types::uinteger value) _NOEXCEPT { _obj_number = value; }

			types::ushort GetGenerationNumber(void) const _NOEXCEPT { return _gen_number; }
			void SetGenerationNumber(types::ushort value) _NOEXCEPT { _gen_number = value; }

			virtual Usage GetUsage(void) const _NOEXCEPT = 0;

			bool InUse(void) const _NOEXCEPT{ return GetUsage() == Usage::Compressed || GetUsage() == Usage::Used; }

			void SetFile(std::weak_ptr<File> file) _NOEXCEPT { _file = file; }
			std::weak_ptr<File> GetFile() const _NOEXCEPT { return _file; }

			virtual ~XrefEntryBase() {};

		protected:
			std::weak_ptr<File> _file;
			types::uinteger _obj_number = 0;
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
			XrefFreeEntry(types::uinteger obj_number, types::ushort gen_number, types::uinteger next)
				: XrefEntryBase(obj_number, gen_number), _next(next) {}

		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override { return XrefEntryBase::Usage::Free; }

			types::uinteger GetNextFreeObjectNumber(void) const _NOEXCEPT { return _next; }
			void SetNextFreeObjectNumber(types::uinteger value) _NOEXCEPT { _next = value; }

		private:
			types::uinteger _next = 0;
		};

		class XrefUsedEntry : public XrefEntryBase
		{
		public:
			XrefUsedEntry(types::uinteger obj_number, types::ushort gen_number, types::stream_offset offset)
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
			XrefCompressedEntry(types::uinteger obj_number, types::ushort gen_number, types::uinteger object_stream_number, types::integer index)
				: XrefEntryBase(obj_number, gen_number), _object_stream_number(object_stream_number), _index(index) {}

		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override { return XrefEntryBase::Usage::Compressed; }

			ObjectPtr GetReference(void) { Initialize(); return _reference; }
			void SetReference(ObjectPtr ref) { _reference = ref; }

			types::uinteger GetObjectStreamNumber(void) const _NOEXCEPT { return _object_stream_number; }
			void SetObjectStreamNumber(types::uinteger value) _NOEXCEPT { _object_stream_number = value; }

			types::uinteger GetIndex(void) const _NOEXCEPT { return _index; }
			void SetIndex(types::uinteger value) _NOEXCEPT { _index = value; }

			bool Initialized(void) const { return _initialized; }
			void SetInitialized(bool value) { _initialized = value; }

		private:
			void Initialize(void);

			ObjectPtr _reference = NullObject::GetInstance();
			types::uinteger _object_stream_number = 0;
			types::uinteger _index = 0;
			bool _initialized = false;
		};

		class XrefSubsection : public IUnknown
		{
		public:
			XrefSubsection() = default;
			XrefSubsection(types::uinteger index, types::uinteger size) : _index(index) { _entries.reserve(size); }
			void Add(XrefEntryBasePtr entry) { _entries.push_back(entry); }
			types::uinteger Size(void) const _NOEXCEPT { return _entries.size(); }
			XrefEntryBasePtr At(types::uinteger at) { return _entries.at(at); }
			//void SetParent(Xref parent) { _parent = parent; }
			//Xref GetParent(void) const { return _parent; }
			void SetFile(std::weak_ptr<File> file) _NOEXCEPT { _file = file; }
			std::weak_ptr<File> GetFile() const _NOEXCEPT { return _file; }

			types::uinteger Index(void) const
			{
				if (_entries.size() > 0) {
					auto entry = _entries.at(0);
					assert(entry->GetObjectNumber() == _index);
				}

				return _index;
			}

		private:
			std::weak_ptr<File> _file;
			types::uinteger _index = 0;
			std::vector<XrefEntryBasePtr> _entries;
			//Xref _parent; // TODO parent holding strong ref - cyclic dependecies
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

			void Add(XrefSubsectionPtr section) { /*section->SetParent(this);*/ _sections.push_back(section); }
			types::integer Size(void) const _NOEXCEPT { return _sections.size(); }
			XrefSubsectionPtr At(types::integer at) { return _sections.at(at); }

			virtual Type GetType(void) const _NOEXCEPT = 0;
			virtual ~XrefBase() {};

		protected:
			std::weak_ptr<File> _file;
			std::vector<XrefSubsectionPtr> _sections;
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
