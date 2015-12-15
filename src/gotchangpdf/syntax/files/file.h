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

			void Initialize(std::shared_ptr<File> holder);
			ObjectPtr GetIndirectObject(types::integer objNumber,
				types::ushort genNumber);

			XrefChainPtr GetXrefChain(void) const;
			HeaderPtr GetHeader(void) const;

			inline std::string GetFilename(void) const { return _filename; }
			inline std::shared_ptr<FileDevice> GetInputStream(void) const { return _input; }

		private:
			std::shared_ptr<FileDevice> _input;
			HeaderPtr _header;
			XrefChainPtr _xref;
			std::vector<ObjectPtr> _cache;

			bool _initialized = false;
			std::string _full_path;
			std::string _filename;
		};

		class FileHolder : public IUnknown
		{
		public:
			explicit FileHolder(File* file) : _file(file) {}
			std::shared_ptr<File> Value() const { return _file; }

		private:
			std::shared_ptr<File> _file;
		};
	}
}

#endif /* _FILE_H */
