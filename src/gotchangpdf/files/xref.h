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
				USED = 0,
				FREE,
				COMPRESSED
			};

		public:
			inline types::integer GetObjectNumber(void) const { return _obj_number; }
			inline void SetObjectNumber(types::integer value) { _obj_number = value; }

			inline types::ushort GetGenerationNumber(void) const { return _gen_number; }
			inline void SetGenerationNumber(types::ushort value) { _gen_number = value; }

			virtual Usage GetUsage(void) const = 0;

			inline bool InUse(void) const { return GetUsage() == Usage::COMPRESSED || GetUsage() == Usage::USED; }

			inline bool Initialized(void) const { return _initialized; }
			inline void SetInitialized(bool value) { _initialized = value; }

			virtual ~XrefEntry() {};

		protected:
			types::integer _obj_number = 0;
			types::ushort _gen_number = 0;
			bool _initialized = false;
		};

		class XrefFreeEntry : public XrefEntry
		{
		public:
			virtual Usage GetUsage(void) const override { return XrefEntry::Usage::FREE; }

			inline types::integer GetNextFreeObjectNumber(void) const { return _next; }
			inline void SetNextFreeObjectNumber(types::integer value) { _next = value; }

		private:
			types::integer _next;
		};

		class XrefUsedEntry : public XrefEntry
		{
		public:
			virtual Usage GetUsage(void) const override { return XrefEntry::Usage::USED; }

			inline DirectObject GetReference(void) const { return _reference; }
			inline void SetReference(DirectObject ref) { _reference = ref; }

			inline types::stream_offset GetOffset(void) const { return _offset; }
			inline void SetOffset(types::stream_offset value) { _offset = value; }

		private:
			DirectObject _reference;
			types::stream_offset _offset;
		};

		class XrefCompressedEntry : public XrefEntry
		{
		public:
			virtual Usage GetUsage(void) const override { return XrefEntry::Usage::COMPRESSED; }

			inline DirectObject GetReference(void) const { return _reference; }
			inline void SetReference(DirectObject ref) { _reference = ref; }

			inline types::integer GetObjectStreamNumber(void) const { return _object_stream_number; }
			inline void SetObjectStreamNumber(types::integer value) { _object_stream_number = value; }

			inline types::integer GetIndex(void) const { return _index; }
			inline void SetIndex(types::integer value) { _index = value; }

		private:
			DirectObject _reference;
			types::integer _object_stream_number;
			types::integer _index;
		};

		class Xref : public std::vector<XrefEntryPtr>, public IUnknown
		{
		public:
			enum class Type
			{
				TABLE = 0,
				STREAM
			};

			virtual Type GetType(void) const = 0;

			virtual ~Xref() {};
		};

		class XrefTable : public Xref
		{
		public:
			virtual Type GetType(void) const override { return Xref::Type::TABLE; }
		};

		class XrefStream : public Xref
		{
		public:
			virtual Type GetType(void) const override { return Xref::Type::STREAM; }

			inline DictionaryObjectPtr GetDictionary(void) const { return _dictionary; }
			inline void SetDictionary(DictionaryObjectPtr dictionary) { _dictionary = dictionary; }

		private:
			DictionaryObjectPtr _dictionary;
		};
	}
}

#endif /* _XREF_H */
