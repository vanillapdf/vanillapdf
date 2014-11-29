#ifndef _FILE_H
#define _FILE_H

#include "xref.h"
#include "dictionary_object.h"
#include "parser.h"
#include "header.h"
#include "trailer.h"
#include "indirect_object.h"
#include "deferred.h"
#include "catalog.h"
#include "file_device.h"
#include "constants.h"

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
			Deferred<Xref> GetXref(void) const;
			SmartPtr<documents::Catalog> GetDocumentCatalog(void) const;
			Deferred<IndirectObject> GetIndirectObject(types::integer objNumber,
				types::ushort genNumber) const;
			Deferred<Header> GetHeader(void) const;
			Deferred<Trailer> GetTrailer(void) const;
			std::weak_ptr<FileDevice> GetInputStream(void) const;
			const char * GetFilename(void) const;

		private:
			std::shared_ptr<FileDevice> _input;
			Deferred<Header> _header;
			Deferred<Trailer> _trailer;
			Deferred<Xref> _xref;
			std::vector<Deferred<IndirectObject>> _cache;

			bool _initialized = false;
			const char *_filename = nullptr;
		};

		inline Deferred<Header> File::GetHeader(void) const
		{
			return _header;
		
		}
		inline Deferred<Trailer> File::GetTrailer(void) const
		{
			return _trailer;
		}

		inline Deferred<Xref> File::GetXref(void) const
		{
			return _xref;
		}

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
