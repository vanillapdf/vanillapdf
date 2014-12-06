#ifndef _FILE_H
#define _FILE_H

#include "fwd.h"
//#include "xref.h"
//#include "dictionary_object.h"
//#include "parser.h"
//#include "header.h"
//#include "trailer.h"
//#include "indirect_object.h"
//#include "deferred.h"
//#include "catalog.h"
#include "smart_ptr.h"
#include "file_device.h"
#include "constants.h"

#include <memory>
#include <vector>

namespace gotchangpdf
{
	namespace files
	{
		class File
		{
		public:
			File(const char *filename);
			~File(void);

			void Initialize(void);
			Deferred<files::Xref> GetXref(void) const;
			SmartPtr<documents::Catalog> GetDocumentCatalog(void) const;
			Deferred<IndirectObject> GetIndirectObject(types::integer objNumber,
				types::ushort genNumber) const;
			Deferred<files::Header> GetHeader(void) const;
			Deferred<files::Trailer> GetTrailer(void) const;
			std::weak_ptr<FileDevice> GetInputStream(void) const;
			const char * GetFilename(void) const;

		private:
			std::shared_ptr<FileDevice> _input;
			Deferred<files::Header> _header;
			Deferred<files::Trailer> _trailer;
			Deferred<files::Xref> _xref;
			std::vector<IndirectObjectPtr> _cache;

			bool _initialized = false;
			const char *_filename = nullptr;
		};

		inline std::weak_ptr<FileDevice> File::GetInputStream(void) const
		{
			return _input;
		}

		inline const char * File::GetFilename(void) const
		{
			return _filename;
		}
	}
}

#endif /* _FILE_H */
