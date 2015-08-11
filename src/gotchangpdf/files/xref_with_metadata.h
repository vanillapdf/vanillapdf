#ifndef _XREF_WITH_METADATA_H
#define _XREF_WITH_METADATA_H

#include "fwd.h"
#include "deferred.h"
#include "smart_ptr.h"

#include "xref.h"
#include "trailer.h"
#include "dictionary_object.h"
#include "stream_object.h"

namespace gotchangpdf
{
	namespace files
	{
		class XrefWithMetadata : public IUnknown
		{
		public:
			XrefWithMetadata(XrefPtr xref, DictionaryObjectPtr dictionary, types::stream_offset offset)
				: _xref(xref), _dictionary(dictionary), _offset(offset)
			{}

			XrefWithMetadata(XrefPtr xref, TrailerPtr trailer)
				: XrefWithMetadata(xref, trailer->GetDictionary(), trailer->GetXrefOffset())
			{}

			XrefWithMetadata(XrefStreamPtr xref, types::stream_offset offset)
				: XrefWithMetadata(xref, xref->GetDictionary(), offset)
			{}

			XrefPtr GetXref() const { return _xref; }
			DictionaryObjectPtr GetDictionary() const { return _dictionary; }
			types::stream_offset GetOffset() const _NOEXCEPT { return _offset; }

		private:
			XrefPtr _xref;
			DictionaryObjectPtr _dictionary;
			types::stream_offset _offset;
		};
	}
}

#endif /* _XREF_WITH_METADATA_H */
