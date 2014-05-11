#ifndef _FILE_H
#define _FILE_H

#include "xref.h"
#include "dictionary_object.h"
#include "parser.h"
#include "header.h"
#include "trailer.h"
#include "indirect_object.h"
#include "object_reference_wrapper.h"
#include "catalog.h"
#include "interface.h"

#include <fstream>

namespace gotchangpdf
{
	namespace files
	{
		class File : public IFile
		{
		public:
			File(const char *filename);
			virtual ~File(void);

			void Initialize(void);
			ObjectReferenceWrapper<Xref> GetXref(void) const;
			ObjectReferenceWrapper<documents::Catalog> GetDocumentCatalog(void) const;
			ObjectReferenceWrapper<IndirectObject> GetIndirectObject(int objNumber, int genNumber) const;
			ObjectReferenceWrapper<Header> GetHeader(void) const;
			ObjectReferenceWrapper<Trailer> GetTrailer(void) const;
			lexical::Parser GetParser(void) const;

		private:
			std::shared_ptr<lexical::Parser> _stream;
			std::shared_ptr<std::fstream> _input;
			ObjectReferenceWrapper<Header> _header;
			ObjectReferenceWrapper<Trailer> _trailer;
			ObjectReferenceWrapper<Xref> _xref;
			std::vector<ObjectReferenceWrapper<IndirectObject>> _cache;
		};
	}
}

#endif /* _FILE_H */
