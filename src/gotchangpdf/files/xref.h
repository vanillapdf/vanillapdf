#ifndef _XREF_H
#define _XREF_H

#include "fwd.h"
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
			XrefEntry() = default;

			IndirectObjectPtr GetReference(void) const { return _reference; }
			void SetReference(IndirectObjectPtr ref) { _reference = ref; }

			types::integer GetObjectNumber(void) const { return _obj_number; }
			void SetObjectNumber(types::integer value) { _obj_number = value; }

			types::ushort GetGenerationNumber(void) const { return _gen_number; }
			void SetGenerationNumber(types::ushort value) { _gen_number = value; }

			types::stream_offset GetOffset(void) const { return _offset; }
			void SetOffset(types::stream_offset value) { _offset = value; }

			bool InUse(void) const { return _in_use; }
			void SetUsage(bool value) { _in_use = value; }

			bool Initialized(void) const { return _initialized; }
			void SetInitialized(bool value) { _initialized = value; }

		private:
			IndirectObjectPtr _reference;
			types::integer _obj_number;
			types::ushort _gen_number;
			types::stream_offset _offset;
			bool _in_use;
			bool _initialized;
		};

		typedef Deferred<XrefEntry> XrefEntryPtr;

		class Xref : public std::vector<XrefEntryPtr>, public IUnknown
		{
		public:
			enum class Type : unsigned char
			{
				TABLE = 0,
				STREAM
			};

			friend lexical::Parser& operator>> (lexical::Parser& s, Xref& o);

		private:
			Type _type = Type::TABLE;
		};
	}
}

#endif /* _XREF_H */
