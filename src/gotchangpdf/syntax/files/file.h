#ifndef _FILE_H
#define _FILE_H

#include "syntax_fwd.h"
#include "file_device.h"
#include "header.h"
#include "xref_chain.h"
#include "encryption.h"

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
			// Filesystem
			static std::shared_ptr<File> Open(const std::string& path);
			static std::shared_ptr<File> Create(const std::string& path);

			void Initialize(void);
			bool IsInitialized(void) const { return _initialized; }

			ObjectPtr GetIndirectObject(types::big_uint objNumber,
				types::ushort genNumber) const;

			XrefChainPtr GetXrefChain(void) const;
			HeaderPtr GetHeader(void) const;

			std::string GetFilename(void) const { return _filename; }
			std::shared_ptr<FileDevice> GetInputStream(void) const { return _input; }

			// Encryption
			bool IsEncrypted(void) const;
			ObjectPtr GetEncryptionDictionary(void) const { return _encryption_dictionary; }
			void SetEncryptionDictionary(DictionaryObjectPtr object) { _encryption_dictionary = object; }

			bool SetEncryptionPassword(const Buffer& password);
			bool SetEncryptionPassword(const std::string& password);
			bool SetEncryptionKey(const IEncryptionKey& key);

			BufferPtr DecryptStream(const Buffer& data,
				types::big_uint objNumber,
				types::ushort genNumber);

			BufferPtr DecryptString(const Buffer& data,
				types::big_uint objNumber,
				types::ushort genNumber);

			BufferPtr DecryptData(const Buffer& data,
				types::big_uint objNumber,
				types::ushort genNumber,
				const NameObject& filter_name);

			BufferPtr DecryptData(const Buffer& data,
				types::big_uint objNumber,
				types::ushort genNumber,
				EncryptionAlgorithm alg);

			BufferPtr EncryptStream(const Buffer& data,
				types::big_uint objNumber,
				types::ushort genNumber);

			BufferPtr EncryptString(const Buffer& data,
				types::big_uint objNumber,
				types::ushort genNumber);

			BufferPtr EncryptData(const Buffer& data,
				types::big_uint objNumber,
				types::ushort genNumber,
				const NameObject& filter_name);

			BufferPtr EncryptData(const Buffer& data,
				types::big_uint objNumber,
				types::ushort genNumber,
				EncryptionAlgorithm alg) const;

			~File(void);

		private:
			std::shared_ptr<FileDevice> _input;
			HeaderPtr _header;
			XrefChainPtr _xref;
			std::vector<ObjectPtr> _cache;

			bool _initialized = false;
			std::string _full_path;
			std::string _filename;
			BufferPtr _decryption_key;
			ObjectPtr _encryption_dictionary;

			types::stream_offset GetLastXrefOffset(types::stream_size file_size);
			void ReadXref(types::stream_offset offset);
			EncryptionAlgorithm GetEncryptionAlgorithmForFilter(const NameObject& filter_name);

		private:
			File(const std::string& path);
		};

		class FileHolder : public IUnknown
		{
		public:
			void Open(const std::string& path) { _file = File::Open(path); }
			void Create(const std::string& path) { _file = File::Create(path); }
			std::shared_ptr<File> Value() const
			{
				assert(_file.get() != nullptr && "File was not initialized");
				return _file;
			}

		private:
			std::shared_ptr<File> _file;
		};
	}
}

#endif /* _FILE_H */
