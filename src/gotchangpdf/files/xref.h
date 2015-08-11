#ifndef _XREF_H
#define _XREF_H

#include "fwd.h"
#include "direct_object.h"
#include "unknown_interface.h"
#include "constants.h"

#include <vector>

namespace gotchangpdf
{
	namespace files
	{
		class XrefEntry : public IUnknown
		{
		public:
			enum class Usage {
				Used = 0,
				Free,
				Compressed
			};

		public:
			inline types::integer GetObjectNumber(void) const _NOEXCEPT { return _obj_number; }
			inline void SetObjectNumber(types::integer value) _NOEXCEPT { _obj_number = value; }

			inline types::ushort GetGenerationNumber(void) const _NOEXCEPT { return _gen_number; }
			inline void SetGenerationNumber(types::ushort value) _NOEXCEPT { _gen_number = value; }

			virtual Usage GetUsage(void) const _NOEXCEPT = 0;

			inline bool InUse(void) const _NOEXCEPT{ return GetUsage() == Usage::Compressed || GetUsage() == Usage::Used; }

			inline bool Initialized(void) const { return _initialized; }
			inline void SetInitialized(bool value) { _initialized = value; }

			inline void SetFile(files::File *file) _NOEXCEPT { _file = file; }
			inline files::File* GetFile() const _NOEXCEPT { return _file; }

			virtual ~XrefEntry() {};

		protected:
			virtual void Initialize(void) = 0;

			files::File * _file = nullptr;
			types::integer _obj_number = -1;
			types::ushort _gen_number = 0;
			bool _initialized = false;
		};

		class XrefFreeEntry : public XrefEntry
		{
		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override { return XrefEntry::Usage::Free; }

			inline types::integer GetNextFreeObjectNumber(void) const _NOEXCEPT { return _next; }
			inline void SetNextFreeObjectNumber(types::integer value) _NOEXCEPT { _next = value; }

		protected:
			virtual void Initialize(void) override { /* Nothing to initialize for free entries */ }

		private:
			types::integer _next = -1;
		};

		class XrefUsedEntry : public XrefEntry
		{
		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override{ return XrefEntry::Usage::Used; }

			inline DirectObject GetReference(void) { Initialize(); return _reference; }
			inline void SetReference(DirectObject ref) { _reference = ref; }

			inline types::stream_offset GetOffset(void) const _NOEXCEPT { return _offset; }
			inline void SetOffset(types::stream_offset value) _NOEXCEPT { _offset = value; }

		protected:
			virtual void Initialize(void) override;

		private:
			DirectObject _reference;
			types::stream_offset _offset = std::_BADOFF;
		};

		class XrefCompressedEntry : public XrefEntry
		{
		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override { return XrefEntry::Usage::Compressed; }

			inline DirectObject GetReference(void) { Initialize(); return _reference; }
			inline void SetReference(DirectObject ref) { _reference = ref; }

			inline types::integer GetObjectStreamNumber(void) const _NOEXCEPT { return _object_stream_number; }
			inline void SetObjectStreamNumber(types::integer value) _NOEXCEPT { _object_stream_number = value; }

			inline types::integer GetIndex(void) const _NOEXCEPT { return _index; }
			inline void SetIndex(types::integer value) _NOEXCEPT { _index = value; }

		protected:
			virtual void Initialize(void) override;

		private:
			DirectObject _reference;
			types::integer _object_stream_number = -1;
			types::integer _index = -1;
		};

		class XrefSubsection : public IUnknown
		{
		public:
			XrefSubsection(types::integer index, types::integer size) : _index(index) { _entries.reserve(size); }
			void Add(XrefEntryPtr entry) { _entries.push_back(entry); }
			types::integer Size(void) const _NOEXCEPT { return _entries.size(); }
			XrefEntryPtr At(types::uinteger at) { return _entries.at(at); }
			void SetParent(XrefPtr parent) { _parent = parent; }
			XrefPtr GetParent(void) const { return _parent; }

			types::integer Index(void) const _NOEXCEPT
			{
				if (_entries.size() > 0) {
					auto entry = _entries.at(0);
					assert(entry->GetObjectNumber() == _index);
				}

				return _index;
			}

		private:
			types::integer _index;
			std::vector<XrefEntryPtr> _entries;
			XrefPtr _parent;
		};

		class Xref : public IUnknown
		{
		public:
			enum class Type
			{
				Table = 0,
				Stream
			};

			inline void SetFile(files::File *file) _NOEXCEPT { _file = file; }
			inline files::File* GetFile() const _NOEXCEPT { return _file; }

			void Add(XrefSubsectionPtr section) { section->SetParent(this); _sections.push_back(section); }
			types::integer Size(void) const _NOEXCEPT { return _sections.size(); }
			XrefSubsectionPtr At(types::integer at) { return _sections.at(at); }

			virtual Type GetType(void) const _NOEXCEPT = 0;
			virtual ~Xref() {};

		protected:
			files::File * _file = nullptr;
			std::vector<XrefSubsectionPtr> _sections;
		};

		class XrefTable : public Xref
		{
		public:
			virtual Type GetType(void) const _NOEXCEPT override { return Xref::Type::Table; }
		};

		class XrefStream : public Xref
		{
		public:
			virtual Type GetType(void) const _NOEXCEPT override { return Xref::Type::Stream; }

			inline DictionaryObjectPtr GetDictionary(void) const { return _dictionary; }
			inline void SetDictionary(DictionaryObjectPtr dictionary) { _dictionary = dictionary; }

		private:
			DictionaryObjectPtr _dictionary;
		};
	}
}

#endif /* _XREF_H */
