#ifndef _FILE_H
#define _FILE_H

#include "xref.h"
#include "dictionary_object.h"
#include "parser.h"
#include "header.h"
#include "trailer.h"
#include "indirect_object.h"
#include "smart_ptr.h"
#include "catalog.h"

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
			SmartPtr<Xref> GetXref(void) const;
			SmartPtr<documents::Catalog> GetDocumentCatalog(void) const;
			SmartPtr<IndirectObject> GetIndirectObject(unsigned int objNumber, unsigned int genNumber) const;
			SmartPtr<Header> GetHeader(void) const;
			SmartPtr<Trailer> GetTrailer(void) const;
			lexical::Parser GetParser(void) const;

		private:
			std::shared_ptr<lexical::Parser> _stream;
			std::shared_ptr<std::fstream> _input;
			SmartPtr<Header> _header;
			SmartPtr<Trailer> _trailer;
			SmartPtr<Xref> _xref;
			std::vector<SmartPtr<IndirectObject>> _cache;
		};
	}
}

#endif /* _FILE_H */
