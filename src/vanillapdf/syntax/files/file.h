#ifndef _FILE_H
#define _FILE_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/utils/encryption.h"

#include "syntax/files/header.h"
#include "syntax/files/xref_chain.h"

#include "utils/streams/input_stream_interface.h"
#include "utils/streams/output_stream_interface.h"
#include "utils/streams/input_output_stream_interface.h"

#include <ios>
#include <vector>
#include <string>

namespace vanillapdf {
namespace syntax {

class File : public virtual IUnknown, public IWeakReferenceable<File> {
public:
	// Filesystem
	static FilePtr Open(const std::string& path);
	static FilePtr Create(const std::string& path);

	static FilePtr OpenStream(IInputOutputStreamPtr stream, const std::string& name);
	static FilePtr CreateStream(IInputOutputStreamPtr stream, const std::string& name);
	static IInputOutputStreamPtr GetFilestream(const std::string& path, std::ios_base::openmode mode);

	BufferPtr GetByteRange(types::stream_size begin, types::size_type length);
	IInputStreamPtr GetByteRangeStream(types::stream_size begin, types::size_type length);

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
	XrefChainPtr GetXrefChain(bool check_initialization) const;
	HeaderPtr GetHeader(void) const;

	XrefUsedEntryBasePtr AllocateNewEntry();

	BufferPtr GetFilenameData(void) const {
		return _filename;
	}

	std::string GetFilenameString(void) const {
		return _filename->ToString();
	}

	IInputStreamPtr GetInputStream(void);
	IOutputStreamPtr GetOutputStream(void);
	IInputOutputStreamPtr GetInputOutputStream(void);

	// Encryption
	bool IsEncrypted(void) const;
	ObjectPtr GetEncryptionDictionary(void) const { return _encryption_dictionary; }
	void SetEncryptionDictionary(DictionaryObjectPtr object) { _encryption_dictionary = object; }
	BufferPtr GetEncryptionKey() const { return _decryption_key; }
	void SetEncryptionKey(BufferPtr data) { _decryption_key = data; }

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
	IInputOutputStreamPtr _input;
	HeaderPtr _header;
	XrefChainPtr _xref;
	std::vector<ObjectPtr> _cache;

	bool _initialized = false;
	std::string _full_path;
	BufferPtr _filename;
	BufferPtr _decryption_key;
	ObjectPtr _encryption_dictionary;

	types::stream_offset GetLastXrefOffset(types::stream_size file_size);
	void ReadXref(types::stream_offset offset);
	EncryptionAlgorithm GetEncryptionAlgorithmForFilter(const NameObject& filter_name);

	ObjectPtr GetIndirectObjectInternal(
		types::big_uint obj_number,
		types::ushort gen_number) const;

	types::big_uint m_next_allocation = 0;

private:
	File(IInputOutputStreamPtr stream, const std::string& path);
};

} // syntax
} // vanillapdf

#endif /* _FILE_H */
