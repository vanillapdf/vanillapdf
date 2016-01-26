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
		class File : public std::enable_shared_from_this<File>
		{
		public:
			static std::shared_ptr<File> Open(const std::string& path);
			void SaveAs(const std::string& path);

			void Initialize(void);
			ObjectPtr GetIndirectObject(types::uinteger objNumber,
				types::ushort genNumber);

			XrefChainPtr GetXrefChain(void) const;
			HeaderPtr GetHeader(void) const;

			std::string GetFilename(void) const { return _filename; }
			std::shared_ptr<FileDevice> GetInputStream(void) const { return _input; }

			~File(void);

		private:
			std::shared_ptr<FileDevice> _input;
			HeaderPtr _header;
			XrefChainPtr _xref;
			std::vector<ObjectPtr> _cache;

			bool _initialized = false;
			std::string _full_path;
			std::string _filename;

		private:
			File(const std::string& path);
		};

		class FileHolder : public IUnknown
		{
		public:
			explicit FileHolder(const std::string& path) { _file = File::Open(path); }
			std::shared_ptr<File> Value() const { return _file; }

		private:
			std::shared_ptr<File> _file;
		};
	}
}

#endif /* _FILE_H */
