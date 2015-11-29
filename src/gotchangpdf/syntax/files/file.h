#ifndef _FILE_H
#define _FILE_H

#include "syntax_fwd.h"
#include "file_device.h"
#include "header.h"
#include "xref_chain.h"

#include <memory>
#include <vector>
#include <string>

namespace gotchangpdf
{
	namespace syntax
	{
		class File
		{
		public:
			File(std::string filename);
			~File(void);

			void Initialize(void);
			semantics::CatalogPtr GetDocumentCatalog(void) const;
			DirectObject GetIndirectObject(types::integer objNumber,
				types::ushort genNumber);

			XrefChainPtr GetXrefChain(void) const;
			HeaderPtr GetHeader(void) const;

			inline std::string GetFilename(void) const { return _filename; }
			inline std::weak_ptr<FileDevice> GetInputStream(void) const { return _input; }

		private:
			std::shared_ptr<FileDevice> _input;
			HeaderPtr _header;
			XrefChainPtr _xref;
			std::vector<DirectObject> _cache;

			bool _initialized = false;
			std::string _filename;
		};
	}
}

#endif /* _FILE_H */
