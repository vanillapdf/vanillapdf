#ifndef _XREF_H
#define _XREF_H

#include "syntax_fwd.h"
#include "direct_object.h"
#include "unknown_interface.h"
#include "constants.h"

#include <vector>

#include <boost/variant/variant.hpp>
#include <boost/variant/static_visitor.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		typedef boost::variant <
			XrefNullEntryPtr,
			XrefFreeEntryPtr,
			XrefUsedEntryPtr,
			XrefCompressedEntryPtr
		> XrefEntry;

		typedef boost::variant <
			XrefTablePtr,
			XrefStreamPtr
		> Xref;

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
			XrefEntryBase(types::integer obj_number, types::ushort gen_number)
				: _obj_number(obj_number), _gen_number(gen_number) {}

		public:
			inline types::integer GetObjectNumber(void) const _NOEXCEPT { return _obj_number; }
			inline void SetObjectNumber(types::integer value) _NOEXCEPT { _obj_number = value; }

			inline types::ushort GetGenerationNumber(void) const _NOEXCEPT { return _gen_number; }
			inline void SetGenerationNumber(types::ushort value) _NOEXCEPT { _gen_number = value; }

			virtual Usage GetUsage(void) const _NOEXCEPT = 0;

			inline bool InUse(void) const _NOEXCEPT{ return GetUsage() == Usage::Compressed || GetUsage() == Usage::Used; }

			inline void SetFile(File *file) _NOEXCEPT { _file = file; }
			inline File* GetFile() const _NOEXCEPT { return _file; }

			virtual ~XrefEntryBase() {};

		protected:
			File * _file = nullptr;
			types::integer _obj_number = -1;
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
			XrefFreeEntry(types::integer obj_number, types::ushort gen_number, types::integer next)
				: XrefEntryBase(obj_number, gen_number), _next(next) {}

		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override { return XrefEntryBase::Usage::Free; }

			inline types::integer GetNextFreeObjectNumber(void) const _NOEXCEPT { return _next; }
			inline void SetNextFreeObjectNumber(types::integer value) _NOEXCEPT { _next = value; }

		private:
			types::integer _next = -1;
		};

		class XrefUsedEntry : public XrefEntryBase
		{
		public:
			XrefUsedEntry(types::integer obj_number, types::ushort gen_number, types::stream_offset offset)
				: XrefEntryBase(obj_number, gen_number), _offset(offset) {}

		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override{ return XrefEntryBase::Usage::Used; }

			inline DirectObject GetReference(void) { Initialize(); return _reference; }
			inline void SetReference(DirectObject ref) { _reference = ref; }

			inline types::stream_offset GetOffset(void) const _NOEXCEPT { return _offset; }
			inline void SetOffset(types::stream_offset value) _NOEXCEPT { _offset = value; }

			inline bool Initialized(void) const { return _initialized; }
			inline void SetInitialized(bool value) { _initialized = value; }

		private:
			void Initialize(void);

			DirectObject _reference;
			types::stream_offset _offset = std::_BADOFF;
			bool _initialized = false;
		};

		class XrefCompressedEntry : public XrefEntryBase
		{
		public:
			XrefCompressedEntry(types::integer obj_number, types::ushort gen_number, types::integer object_stream_number, types::integer index)
				: XrefEntryBase(obj_number, gen_number), _object_stream_number(object_stream_number), _index(index) {}

		public:
			virtual Usage GetUsage(void) const _NOEXCEPT override { return XrefEntryBase::Usage::Compressed; }

			inline DirectObject GetReference(void) { Initialize(); return _reference; }
			inline void SetReference(DirectObject ref) { _reference = ref; }

			inline types::integer GetObjectStreamNumber(void) const _NOEXCEPT { return _object_stream_number; }
			inline void SetObjectStreamNumber(types::integer value) _NOEXCEPT { _object_stream_number = value; }

			inline types::integer GetIndex(void) const _NOEXCEPT { return _index; }
			inline void SetIndex(types::integer value) _NOEXCEPT { _index = value; }

			inline bool Initialized(void) const { return _initialized; }
			inline void SetInitialized(bool value) { _initialized = value; }

		private:
			void Initialize(void);

			DirectObject _reference;
			types::integer _object_stream_number = -1;
			types::integer _index = -1;
			bool _initialized = false;
		};

		class XrefEntryBaseVisitor : public boost::static_visitor<XrefEntryBase*>
		{
		public:
			template <typename T>
			inline XrefEntryBase* operator()(T& entry) const { return entry.Content.get(); }
		};

		template <typename T>
		class XrefEntryVisitor : public boost::static_visitor<T>
		{
		public:
			inline T operator()(T& entry) const { return entry; }

			template <typename U>
			inline T operator()(const U&) const { throw Exception("Type cast error"); }
		};

		class XrefSubsection : public IUnknown
		{
		public:
			XrefSubsection() = default;
			XrefSubsection(types::integer index, types::integer size) : _index(index) { _entries.reserve(size); }
			void Add(XrefEntry entry) { _entries.push_back(entry); }
			types::integer Size(void) const _NOEXCEPT { return _entries.size(); }
			XrefEntry At(types::uinteger at) { return _entries.at(at); }
			//void SetParent(Xref parent) { _parent = parent; }
			//Xref GetParent(void) const { return _parent; }
			inline void SetFile(File *file) _NOEXCEPT { _file = file; }
			inline File* GetFile() const _NOEXCEPT { return _file; }

			types::integer Index(void) const
			{
				if (_entries.size() > 0) {
					auto entry = _entries.at(0);
					XrefEntryBaseVisitor visitor;
					auto base = entry.apply_visitor(visitor);
					assert(base->GetObjectNumber() == _index);
				}

				return _index;
			}

		private:
			File * _file = nullptr;
			types::integer _index = 0;
			std::vector<XrefEntry> _entries;
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

			inline void SetFile(File *file) _NOEXCEPT { _file = file; }
			inline File* GetFile() const _NOEXCEPT { return _file; }

			inline DictionaryObjectPtr GetTrailerDictionary(void) const { return _trailer_dictionary; }
			inline void SetTrailerDictionary(DictionaryObjectPtr dictionary) { _trailer_dictionary = dictionary; }

			types::stream_offset GetLastXrefOffset() const _NOEXCEPT { return _last_xref_offset; }
			void SetLastXrefOffset(types::stream_offset offset) _NOEXCEPT { _last_xref_offset = offset; }

			void Add(XrefSubsectionPtr section) { /*section->SetParent(this);*/ _sections.push_back(section); }
			types::integer Size(void) const _NOEXCEPT { return _sections.size(); }
			XrefSubsectionPtr At(types::integer at) { return _sections.at(at); }

			virtual Type GetType(void) const _NOEXCEPT = 0;
			virtual ~XrefBase() {};

		protected:
			File * _file = nullptr;
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

		class XrefBaseVisitor : public boost::static_visitor<XrefBase*>
		{
		public:
			template <typename T>
			inline XrefBase* operator()(T& xref) const { return xref.Content.get(); }
		};

		template <typename T>
		class XrefVisitor : public boost::static_visitor<T>
		{
		public:
			inline T operator()(T& xref) const { return xref; }

			template <typename U>
			inline T operator()(const U&) const { throw exceptions::Exception("Type cast error"); }
		};
	}
}

#endif /* _XREF_H */
