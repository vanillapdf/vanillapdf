#ifndef _FILE_H
#define _FILE_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/utils/encryption.h"

#include "syntax/files/header.h"
#include "syntax/files/xref_chain.h"

#include "utils/streams/input_stream_interface.h"
#include "utils/streams/output_stream_interface.h"
#include "utils/streams/input_output_stream_interface.h"

#include <memory>
#include <vector>
#include <string>
#include <iosfwd>

namespace gotchangpdf {
namespace syntax {

class File : public virtual IUnknown, public IWeakReferenceable<File> {
public:
	// Filesystem
	static FilePtr Open(const std::string& path);
	static FilePtr Create(const std::string& path);

	BufferPtr GetByteRange(types::stream_size begin, types::size_type length) const;
	IInputStreamPtr GetByteRangeStream(types::stream_size begin, types::size_type length) const;

	std::vector<ObjectPtr> DeepCopyObject(ObjectPtr object);
	std::vector<ObjectPtr> DeepCopyObjects(const std::vector<ObjectPtr>& objects);
	void DeepCopyObject(std::map<ObjectPtr, ObjectPtr>& map, std::map<ObjectPtr, bool>& visited, ObjectPtr original);
	void ShallowCopyObject(std::map<ObjectPtr, ObjectPtr>& map, ObjectPtr original);
	ObjectPtr ShallowCopyObject(ObjectPtr original);
	void FixObjectReferences(const std::map<ObjectPtr, ObjectPtr>& map, std::map<ObjectPtr, bool>& visited, ObjectPtr copied);

	void Initialize(void);
	bool IsInitialized(void) const noexcept { return _initialized; }

	ObjectPtr GetIndirectObject(
		types::big_uint objNumber,
		types::ushort genNumber) const;

	XrefChainPtr GetXrefChain(void) const;
	HeaderPtr GetHeader(void) const;

	std::string GetFilename(void) const { return _filename; }
	IInputStreamPtr GetInputStream(void);
	IOutputStreamPtr GetOutputStream(void);
	IInputOutputStreamPtr GetInputOutputStream(void);

	// Encryption
	bool IsEncrypted(void) const;
	ObjectPtr GetEncryptionDictionary(void) const { return _encryption_dictionary; }
	void SetEncryptionDictionary(DictionaryObjectPtr object) { _encryption_dictionary = object; }

	bool SetEncryptionPassword(const Buffer& password);
	bool SetEncryptionPassword(const std::string& password);
	bool SetEncryptionKey(IEncryptionKey& key);

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
	std::shared_ptr<std::fstream> _input;
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

	ObjectPtr GetIndirectObjectInternal(
		types::big_uint objNumber,
		types::ushort genNumber) const;

private:
	File(const std::string& path);
};

} // syntax
} // gotchangpdf

#endif /* _FILE_H */
