#ifndef _FILE_H
#define _FILE_H

#include "cross_reference_info.h"
#include "dictionary_object.h"
#include "parser.h"
#include "Header.h"
#include "Trailer.h"
#include "indirect_object.h"

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
			boost::intrusive_ptr<CrossReferenceInfo> GetCrossReferenceTable(void) const;
			boost::intrusive_ptr<IndirectObject> GetDocumentCatalog(void) const;
			boost::intrusive_ptr<IndirectObject> GetIndirectObject(int objNumber, int genNumber) const;
			boost::intrusive_ptr<Header> GetHeader(void) const;
			boost::intrusive_ptr<Trailer> GetTrailer(void) const;
			lexical::Parser GetParser(void) const;

		private:
			std::shared_ptr<lexical::Parser> _stream;
			std::shared_ptr<std::fstream> _input;
			boost::intrusive_ptr<Header> _header;
			boost::intrusive_ptr<Trailer> _trailer;
			boost::intrusive_ptr<CrossReferenceInfo> _xref;
			std::vector<boost::intrusive_ptr<IndirectObject>> _cache;
		};
	}
}

#endif /* _FILE_H */
