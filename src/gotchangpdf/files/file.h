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
#include "file_device.h"

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
			SmartPtr<Xref> GetXref(void) const;
			SmartPtr<documents::Catalog> GetDocumentCatalog(void) const;
			SmartPtr<IndirectObject> GetIndirectObject(unsigned int objNumber,
				unsigned int genNumber) const;
			SmartPtr<Header> GetHeader(void) const;
			SmartPtr<Trailer> GetTrailer(void) const;
			std::weak_ptr<FileDevice> GetInputStream(void) const;

		private:
			std::shared_ptr<FileDevice> _input;
			SmartPtr<Header> _header;
			SmartPtr<Trailer> _trailer;
			SmartPtr<Xref> _xref;
			std::vector<SmartPtr<IndirectObject>> _cache;

			bool _initialized = false;
			const char *_filename = nullptr;
		};

		inline SmartPtr<Header> File::GetHeader(void) const
		{
			return _header;
		
		}
		inline SmartPtr<Trailer> File::GetTrailer(void) const
		{
			return _trailer;
		}

		inline SmartPtr<Xref> File::GetXref(void) const
		{
			return _xref;
		}

		inline std::weak_ptr<FileDevice> File::GetInputStream(void) const
		{
			return _input;
		}
	}
}

#endif /* _FILE_H */
