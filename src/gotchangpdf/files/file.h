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
			SmartPtr<documents::Catalog> GetDocumentCatalog(void) const;
			IndirectObjectPtr GetIndirectObject(types::integer objNumber,
				types::ushort genNumber);
			bool IsIndirectObjectIntialized(types::integer objNumber,
				types::ushort genNumber);

			XrefPtr GetXref(void) const;
			HeaderPtr GetHeader(void) const;
			TrailerPtr GetTrailer(void) const;

			inline std::string GetFilename(void) const { return _filename; }
			inline std::weak_ptr<FileDevice> GetInputStream(void) const { return _input; }

		private:
			std::shared_ptr<FileDevice> _input;
			HeaderPtr _header;
			TrailerPtr _trailer;
			XrefPtr _xref;
			std::vector<IndirectObjectPtr> _cache;

			bool _initialized = false;
			std::string _filename;
		};
	}
}

#endif /* _FILE_H */
