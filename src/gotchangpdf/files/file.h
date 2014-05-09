#ifndef _FILE_H
#define _FILE_H

#include "cross_reference_info.h"
#include "dictionary_object.h"
#include "parser.h"
#include "Header.h"
#include "Trailer.h"
#include "indirect_object.h"
#include "object_reference_wrapper.h"

#include <fstream>

namespace gotchangpdf
{
	namespace files
	{
		class File
		{
		public:
			File(const char *filename);
			virtual ~File(void);

			void Initialize(void);
			ObjectReferenceWrapper<CrossReferenceInfo> GetCrossReferenceTable(void) const;
			ObjectReferenceWrapper<IndirectObject> GetDocumentCatalog(void) const;
			ObjectReferenceWrapper<IndirectObject> GetIndirectObject(int objNumber, int genNumber) const;
			ObjectReferenceWrapper<Header> GetHeader(void) const;
			ObjectReferenceWrapper<Trailer> GetTrailer(void) const;
			lexical::Parser GetParser(void) const;

		private:
			std::shared_ptr<lexical::Parser> _stream;
			std::shared_ptr<std::fstream> _input;
			ObjectReferenceWrapper<Header> _header;
			ObjectReferenceWrapper<Trailer> _trailer;
			ObjectReferenceWrapper<CrossReferenceInfo> _xref;
			std::vector<ObjectReferenceWrapper<IndirectObject>> _cache;
		};
	}
}

#endif /* _FILE_H */
