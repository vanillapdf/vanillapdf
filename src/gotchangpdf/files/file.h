#ifndef _FILE_H
#define _FILE_H

#include "fwd.h"
#include "smart_ptr.h"
#include "file_device.h"
#include "constants.h"

#include <memory>
#include <vector>
#include <string>

namespace gotchangpdf
{
	namespace files
	{
		class File
		{
		public:
			File(std::string filename);
			~File(void);

			void Initialize(void);
			Deferred<files::Xref> GetXref(void) const;
			SmartPtr<documents::Catalog> GetDocumentCatalog(void) const;
			Deferred<IndirectObject> GetIndirectObject(types::integer objNumber,
				types::ushort genNumber);
			bool IsIndirectObjectIntialized(types::integer objNumber,
				types::ushort genNumber);
			Deferred<files::Header> GetHeader(void) const;
			Deferred<files::Trailer> GetTrailer(void) const;
			std::weak_ptr<FileDevice> GetInputStream(void) const;
			std::string GetFilename(void) const;

		private:
			std::shared_ptr<FileDevice> _input;
			Deferred<Header> _header;
			Deferred<Trailer> _trailer;
			Deferred<Xref> _xref;
			std::vector<IndirectObjectPtr> _cache;

			bool _initialized = false;
			std::string _filename;
		};

		inline std::weak_ptr<FileDevice> File::GetInputStream(void) const
		{
			return _input;
		}

		inline std::string File::GetFilename(void) const
		{
			return _filename;
		}
	}
}

#endif /* _FILE_H */
