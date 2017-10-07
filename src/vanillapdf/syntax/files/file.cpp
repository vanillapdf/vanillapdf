#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/files/xref_chain.h"
#include "syntax/files/header.h"

#include "syntax/parsers/parser.h"
#include "syntax/parsers/reverse_parser.h"
#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/utils/encryption.h"

#include "utils/streams/input_reverse_stream.h"
#include "utils/streams/input_stream.h"
#include "utils/streams/output_stream.h"
#include "utils/streams/input_output_stream.h"

#include "utils/misc_utils.h"

#include <fstream>

namespace vanillapdf {
namespace syntax {

FilePtr File::Open(const std::string& path) {
	return FilePtr(pdf_new File(path));
}

FilePtr File::Create(const std::string& path) {
	FilePtr result(pdf_new File(path));

	result->_input = std::make_shared<std::fstream>();
	result->_input->open(path,
		std::ios_base::in |
		std::ios_base::out |
		std::ios_base::binary |
		std::ios::trunc
	);

	if (!result->_input || !result->_input->good()) {
		throw GeneralException("Could not open file");
	}

	result->_initialized = true;
	return result;
}

File::File(const std::string& path)
	: _full_path(path) {
	_filename = MiscUtils::ExtractFilename(path);
	LOG_WARNING_GLOBAL << "File constructor " << _filename;
}

File::~File(void) {
	LOG_WARNING_GLOBAL << "File destructor " << _filename;

	if (nullptr != _input) {
		_input->close();
		_input = nullptr;
	}

	_cache.clear();
}

void File::Initialize() {
	LOG_DEBUG(_filename) << "Initialize";

	if (_initialized) {
		return;
	}

	_input = std::make_shared<std::fstream>();
	_input->open(_full_path,
		std::ios::in |
		std::ios::binary |
		std::ios::ate
	);

	if (!_input || !_input->good()) {
		throw GeneralException("Could not open file");
	}

	// Opening the stream with ate option
	auto file_size = _input->tellg();

	InputStreamPtr input_stream = make_deferred<InputStream>(_input);
	Parser stream(GetWeakReference<File>(), input_stream);
	_header = stream.ReadHeader(0);

	try {
		auto offset = GetLastXrefOffset(file_size);
		ReadXref(offset);
	} catch (NotSupportedException&) {
		throw;
	} catch (ExceptionBase& e) {
		LOG_ERROR(_filename) << "Could not parse xref chain, using fallback mechanism";
		LOG_ERROR(_filename) << "Reason: " << e.what();

		_xref = stream.FindAllObjects();
	}

	// After xref informations are parsed, check for encryption
	auto last_trailer_dictionary = _xref->Begin()->Value()->GetTrailerDictionary();
	if (last_trailer_dictionary->Contains(constant::Name::Encrypt)) {
		_encryption_dictionary = last_trailer_dictionary->Find(constant::Name::Encrypt);

		if (ObjectUtils::IsType<IndirectObjectReferencePtr>(_encryption_dictionary)) {
			auto reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(_encryption_dictionary);
			auto obj_number = reference->GetReferencedObjectNumber();
			auto gen_number = reference->GetReferencedGenerationNumber();

			_encryption_dictionary = GetIndirectObjectInternal(obj_number, gen_number);
		}

		// Encryption dictionary itself is not encrypted
		_encryption_dictionary->SetEncryptionExempted();
	}

	// ID entries in trailer are exempted from encryption
	for (auto xref : _xref) {
		auto trailer_dictionary = xref->GetTrailerDictionary();

		if (trailer_dictionary->Contains(constant::Name::ID)) {
			auto ids = trailer_dictionary->Find(constant::Name::ID);
			assert(!ObjectUtils::IsType<IndirectObjectReferencePtr>(ids) && "Document ID is indirect reference");

			// I have found document, that have document ID as a single integer "77777777777777777" issue5599.pdf.
			// Adobe acrobat can open this document without any notifications,
			// therefore following requirement from specification 7.5.5 is considered invalid:
			// "An array of two byte-strings constituting a file identifier"
			//assert(ObjectUtils::IsType<ArrayObjectPtr<StringObjectPtr>>(ids) && "Document ID is not array of strings");

			ids->SetEncryptionExempted();
		}
	}

	_initialized = true;
}

bool File::SetEncryptionKey(IEncryptionKey& key) {
	if (!IsEncrypted()) {
		return false;
	}

	auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
	auto filter = dict->FindAs<NameObjectPtr>(constant::Name::Filter);
	auto sub_filter = dict->FindAs<NameObjectPtr>(constant::Name::SubFilter);
	EncryptionAlgorithm algorithm = EncryptionAlgorithm::None;

	IntegerObjectPtr length_bits = make_deferred<IntegerObject>(40);
	if (dict->Contains(constant::Name::Length)) {
		length_bits = dict->FindAs<IntegerObjectPtr>(constant::Name::Length);
		assert(length_bits->GetIntegerValue() % 8 == 0 && "Key length is not multiplier of 8");
	}

	ArrayObjectPtr<StringObjectPtr> recipients;
	if (sub_filter == constant::Name::AdbePkcs7s3) {
		recipients = dict->FindAs<ArrayObjectPtr<StringObjectPtr>>(constant::Name::Recipients);
	}

	if (sub_filter == constant::Name::AdbePkcs7s4) {
		recipients = dict->FindAs<ArrayObjectPtr<StringObjectPtr>>(constant::Name::Recipients);
	}

	if (sub_filter == constant::Name::AdbePkcs7s5) {

		do {
			if (!dict->Contains(constant::Name::CF))
				break;

			auto crypt_filter_dictionary = dict->FindAs<DictionaryObjectPtr>(constant::Name::CF);
			if (!crypt_filter_dictionary->Contains(constant::Name::DefaultCryptFilter))
				break;

			auto crypt_filter = crypt_filter_dictionary->FindAs<DictionaryObjectPtr>(constant::Name::DefaultCryptFilter);
			if (!crypt_filter->Contains(constant::Name::CFM))
				break;

			auto method = crypt_filter->FindAs<NameObjectPtr>(constant::Name::CFM);
			if (method == constant::Name::AESV2) {
				algorithm = EncryptionAlgorithm::AES;
			}

			if (method == constant::Name::AESV3) {
				algorithm = EncryptionAlgorithm::AES;

				if (crypt_filter->Contains(constant::Name::Length)) {
					length_bits = crypt_filter->FindAs<IntegerObjectPtr>(constant::Name::Length);
					assert(length_bits->GetIntegerValue() == 256 && "AESV3 length is not 256 bits");
				}
			}

			if (method == constant::Name::None) {
				algorithm = EncryptionAlgorithm::None;
			}

			if (method == constant::Name::V2) {
				algorithm = EncryptionAlgorithm::RC4;
			}

			recipients = crypt_filter->FindAs<ArrayObjectPtr<StringObjectPtr>>(constant::Name::Recipients);
		} while (false);
	}

	try {
		_decryption_key = EncryptionUtils::GetRecipientKey(recipients, length_bits, algorithm, key);
		return true;
	} catch (...) {
		return false;
	}
}

bool File::SetEncryptionPassword(const std::string& password) {
	Buffer buffer(password.begin(), password.end());
	return SetEncryptionPassword(buffer);
}

bool File::SetEncryptionPassword(const Buffer& password) {
	if (!IsEncrypted()) {
		return false;
	}

	auto ids = _xref->Begin()->Value()->GetTrailerDictionary()->FindAs<ArrayObjectPtr<StringObjectPtr>>(constant::Name::ID);
	auto id = ids->At(0);

	auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
	auto filter = dict->FindAs<NameObjectPtr>(constant::Name::Filter);

	if (filter != constant::Name::Standard) {
		return false;
	}

	auto user_value = dict->FindAs<StringObjectPtr>(constant::Name::U);
	auto owner_value = dict->FindAs<StringObjectPtr>(constant::Name::O);
	auto permissions = dict->FindAs<IntegerObjectPtr>(constant::Name::P);
	auto revision = dict->FindAs<IntegerObjectPtr>(constant::Name::R);
	auto version = dict->FindAs<IntegerObjectPtr>(constant::Name::V);

	IntegerObjectPtr length_bits = make_deferred<IntegerObject>(40);
	if (dict->Contains(constant::Name::Length)) {
		length_bits = dict->FindAs<IntegerObjectPtr>(constant::Name::Length);
		assert(length_bits->GetIntegerValue() % 8 == 0 && "Key length is not multiplier of 8");
	}

	// Pad password with predefined scheme
	BufferPtr padPassword = EncryptionUtils::PadTruncatePassword(password);
	BufferPtr encrypted_owner_data = EncryptionUtils::ComputeEncryptedOwnerData(padPassword, dict);

	// Check if entered password was owner password
	if (EncryptionUtils::CheckKey(encrypted_owner_data, id->GetValue(), owner_value->GetValue(), user_value->GetValue(), permissions, revision, length_bits, _decryption_key)) {
		return true;
	}

	// Check if entered password was user password
	if (EncryptionUtils::CheckKey(padPassword, id->GetValue(), owner_value->GetValue(), user_value->GetValue(), permissions, revision, length_bits, _decryption_key)) {
		return true;
	}

	return false;
}

bool File::IsEncrypted(void) const {
	// Returns false if wasn't initialized
	return (!_encryption_dictionary.empty() && _encryption_dictionary != NullObject::GetInstance());
}

BufferPtr File::DecryptStream(const Buffer& data,
	types::big_uint objNumber,
	types::ushort genNumber) {
	if (!IsEncrypted()) {
		return make_deferred<Buffer>(data);
	}

	auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
	auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

	if (version == 4 || version == 5) {
		auto filter_name = encryption_dictionary->FindAs<NameObjectPtr>(constant::Name::StmF);
		return DecryptData(data, objNumber, genNumber, filter_name);
	}

	return DecryptData(data, objNumber, genNumber, EncryptionAlgorithm::RC4);
}

BufferPtr File::DecryptString(const Buffer& data,
	types::big_uint objNumber,
	types::ushort genNumber) {
	if (!IsEncrypted()) {
		return make_deferred<Buffer>(data);
	}

	auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
	auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

	if (version == 4 || version == 5) {
		auto filter_name = encryption_dictionary->FindAs<NameObjectPtr>(constant::Name::StrF);
		return DecryptData(data, objNumber, genNumber, filter_name);
	}

	return DecryptData(data, objNumber, genNumber, EncryptionAlgorithm::RC4);
}

BufferPtr File::DecryptData(const Buffer& data,
	types::big_uint objNumber,
	types::ushort genNumber,
	const NameObject& filter_name) {
	if (!IsEncrypted()) {
		return make_deferred<Buffer>(data);
	}

	EncryptionAlgorithm algorithm = GetEncryptionAlgorithmForFilter(filter_name);
	return DecryptData(data, objNumber, genNumber, algorithm);
}

BufferPtr File::DecryptData(const Buffer& data,
	types::big_uint objNumber,
	types::ushort genNumber,
	EncryptionAlgorithm alg) {
	// Object number zero is most probably invalid
	// I'd like to see the document where object number zero
	// is encrypted, because it most probably violates many
	// rules and assertions I already have
	assert(objNumber != 0);

	if (!IsEncrypted()) {
		return make_deferred<Buffer>(data);
	}

	auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
	auto dictionary_object_number = encryption_dictionary->GetObjectNumber();
	auto dictionary_generation_number = encryption_dictionary->GetGenerationNumber();

	// The idea behind this was originally check whether we are dealing with
	// encryption dictionary based on object and generation number.
	// However, encryption dictionary does not have to be indirect object
	// therefore I have created SetEncryptionExempted property, which shall
	// prohibit any decryption on such objects. If this triggers
	// error in encryption exemption most probably occurred
	assert(!(dictionary_object_number == objNumber
		&& dictionary_generation_number == genNumber)
		&& "Encryption dictionary shall be exempted from encryption");

	// data inside encryption dictionary are not encrypted
	if ((dictionary_object_number == objNumber && dictionary_generation_number == genNumber)) {
		return make_deferred<Buffer>(data);
	}

	// Same idea as above. SetEncryptionExempted shall be used for every
	// compressed object and there should not by any call trying to decrypt
	// compressed objects. They are contained in streams which themselves are compressed
	auto object_entry = _xref->GetXrefEntry(objNumber, genNumber);
	assert(object_entry->GetUsage() != XrefEntryBase::Usage::Compressed
		&& "Compressed objects shall be exempted from encryption");

	// Any strings that are inside streams such as content streams and compressed object streams,
	// which themselves are encrypted
	if (object_entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
		return make_deferred<Buffer>(data);
	}

	if (_decryption_key.empty()) {

		// Encrypted documents shall be opened with default empty password
		bool passed = SetEncryptionPassword("");
		if (!passed) {
			throw GeneralException("Invalid password");
		}
	}

	// AES 256 bits behaves differently
	// It was not part of the core specification, but it was added in extension
	if (_decryption_key->size() == 32 && alg == EncryptionAlgorithm::AES) {
		return EncryptionUtils::AESDecrypt(_decryption_key, 32, data);
	}

	BufferPtr object_key = EncryptionUtils::ComputeObjectKey(_decryption_key, objNumber, genNumber, alg);

	switch (alg) {
		default:
		case EncryptionAlgorithm::None:
			// The application shall not decrypt data but shall direct the input stream to the security handler for decryption.
			// No clue
		case EncryptionAlgorithm::RC4:
			return EncryptionUtils::ComputeRC4(object_key, data);
		case EncryptionAlgorithm::AES:
			return EncryptionUtils::AESDecrypt(object_key, data);
	}
}

BufferPtr File::EncryptStream(const Buffer& data,
	types::big_uint objNumber,
	types::ushort genNumber) {
	if (!IsEncrypted()) {
		return make_deferred<Buffer>(data);
	}

	auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
	auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

	if (version == 4 || version == 5) {
		auto filter_name = encryption_dictionary->FindAs<NameObjectPtr>(constant::Name::StmF);
		return EncryptData(data, objNumber, genNumber, filter_name);
	}

	return EncryptData(data, objNumber, genNumber, EncryptionAlgorithm::RC4);
}

BufferPtr File::EncryptString(const Buffer& data,
	types::big_uint objNumber,
	types::ushort genNumber) {
	if (!IsEncrypted()) {
		return make_deferred<Buffer>(data);
	}

	auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
	auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

	if (version == 4 || version == 5) {
		auto filter_name = encryption_dictionary->FindAs<NameObjectPtr>(constant::Name::StrF);
		return EncryptData(data, objNumber, genNumber, filter_name);
	}

	return EncryptData(data, objNumber, genNumber, EncryptionAlgorithm::RC4);
}

BufferPtr File::EncryptData(const Buffer& data,
	types::big_uint objNumber,
	types::ushort genNumber,
	const NameObject& filter_name) {
	if (!IsEncrypted()) {
		return make_deferred<Buffer>(data);
	}

	EncryptionAlgorithm algorithm = GetEncryptionAlgorithmForFilter(filter_name);
	return EncryptData(data, objNumber, genNumber, algorithm);
}

BufferPtr File::EncryptData(const Buffer& data,
	types::big_uint objNumber,
	types::ushort genNumber,
	EncryptionAlgorithm alg) const {
	if (!IsEncrypted()) {
		return make_deferred<Buffer>(data);
	}

	auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
	auto dictionary_object_number = encryption_dictionary->GetObjectNumber();
	auto dictionary_generation_number = encryption_dictionary->GetGenerationNumber();

	// data inside encryption dictionary are not encrypted
	if (objNumber == 0 || (dictionary_object_number == objNumber && dictionary_generation_number == genNumber)) {
		return make_deferred<Buffer>(data);
	}

	// AES 256 bits behaves differently
	// It was not part of the core specification, but it was added in extension
	if (_decryption_key->size() == 32 && alg == EncryptionAlgorithm::AES) {
		return EncryptionUtils::AESEncrypt(_decryption_key, 32, data);
	}

	BufferPtr object_key = EncryptionUtils::ComputeObjectKey(_decryption_key, objNumber, genNumber, alg);

	switch (alg) {
		default:
		case EncryptionAlgorithm::None:
			// The application shall not decrypt data but shall direct the input stream to the security handler for decryption.
			// No clue
		case EncryptionAlgorithm::RC4:
			return EncryptionUtils::ComputeRC4(object_key, data);
		case EncryptionAlgorithm::AES:
			return EncryptionUtils::AESEncrypt(object_key, data);
	}
}

EncryptionAlgorithm File::GetEncryptionAlgorithmForFilter(const NameObject& filter_name) {
	auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
	auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

	do {
		if (version != 4 && version != 5) break;
		if (!encryption_dictionary->Contains(constant::Name::CF)) break;

		auto crypt_filter_dictionary = encryption_dictionary->FindAs<DictionaryObjectPtr>(constant::Name::CF);
		if (!crypt_filter_dictionary->Contains(filter_name)) break;

		auto crypt_filter = crypt_filter_dictionary->FindAs<DictionaryObjectPtr>(filter_name);
		if (!crypt_filter->Contains(constant::Name::CFM)) break;

		auto method = crypt_filter->FindAs<NameObjectPtr>(constant::Name::CFM);
		if (method == constant::Name::AESV2 || method == constant::Name::AESV3) {
			return EncryptionAlgorithm::AES;
		}

		if (method == constant::Name::None) {
			return EncryptionAlgorithm::None;
		}

		if (method == constant::Name::V2) {
			return EncryptionAlgorithm::RC4;
		}
	} while (false);

	return EncryptionAlgorithm::None;
}

void File::ReadXref(types::stream_offset offset) {
	InputStreamPtr input_stream = make_deferred<InputStream>(_input);
	Parser stream(GetWeakReference<File>(), input_stream);

	// Keep track of current offset to xref
	types::stream_offset current_offset = offset;

	// Clear all previously allocated xrefs
	_xref->Clear();

	for (;;) {
		auto xref = stream.ReadXref(current_offset);

		_xref->Append(xref);

		auto trailer_dictionary = xref->GetTrailerDictionary();
		if (trailer_dictionary->Contains(constant::Name::XRefStm)) {
			auto xref_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(xref);
			auto stm_offset_obj = trailer_dictionary->FindAs<IntegerObjectPtr>(constant::Name::XRefStm);
			auto stm_offset = stm_offset_obj->GetIntegerValue();
			auto hybrid_xref = stream.ReadXref(stm_offset);

			if (!ConvertUtils<XrefBasePtr>::IsType<XrefStreamPtr>(hybrid_xref)) {
				throw GeneralException("Hybrid xref is not a stream");
			}

			auto hybrid_xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(hybrid_xref);
			xref_table->SetHybridStream(hybrid_xref_stream);
		}

		if (!trailer_dictionary->Contains(constant::Name::Prev)) {
			break;
		}

		auto prev = trailer_dictionary->FindAs<IntegerObjectPtr>(constant::Name::Prev);
		current_offset = prev->GetIntegerValue();
	}
}

types::stream_offset File::GetLastXrefOffset(types::stream_size file_size) {
	InputReverseStreamPtr raw_reversed = make_deferred<InputReverseStream>(_input, file_size);
	auto reverse_stream = ReverseParser(raw_reversed);
	return reverse_stream.ReadLastXrefOffset();
}

ObjectPtr File::GetIndirectObject(
	types::big_uint objNumber,
	types::ushort genNumber) const {
	if (!_initialized) {
		throw FileNotInitializedException(_filename);
	}

	return GetIndirectObjectInternal(objNumber, genNumber);
}

ObjectPtr File::GetIndirectObjectInternal(
	types::big_uint objNumber,
	types::ushort genNumber) const {
	LOG_DEBUG(_filename) << "GetIndirectObject " << objNumber << " and " << genNumber;

	if (!_xref->Contains(objNumber, genNumber)) {
		return NullObject::GetInstance();
	}

	auto item = _xref->GetXrefEntry(objNumber, genNumber);

	switch (item->GetUsage()) {
		case XrefEntryBase::Usage::Used:
		{
			auto used = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(item);
			return used->GetReference();
		}
		case XrefEntryBase::Usage::Compressed:
		{
			auto compressed = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(item);
			return compressed->GetReference();
		}
		case XrefEntryBase::Usage::Null:
			LOG_ERROR(_filename) << "Xref entry type is null for object " << objNumber << " " << genNumber;
		case XrefEntryBase::Usage::Free:
			return NullObject::GetInstance();
		default:
			throw GeneralException("Unknown entry type");
	}
}

HeaderPtr File::GetHeader(void) const {
	return _header;
}

XrefChainPtr File::GetXrefChain(void) const {
	return _xref;
}

void File::FixObjectReferences(const std::map<ObjectPtr, ObjectPtr>& map, std::map<ObjectPtr, bool>& visited, ObjectPtr copied) {
	auto has_visited = visited.find(copied);
	if (visited.end() != has_visited && has_visited->second) {
		// already visited
		return;
	}

	// Visit
	visited[copied] = true;

	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(copied)) {
		auto copied_ref = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(copied);

		bool found = false;
		for (auto item : map) {
			auto original_obj = item.first;
			if (!original_obj->IsIndirect()) {
				continue;
			}

			if (original_obj->GetObjectNumber() != copied_ref->GetReferencedObjectNumber()
				|| original_obj->GetGenerationNumber() != copied_ref->GetReferencedGenerationNumber()) {
				continue;
			}

			auto copied_referenced_obj = item.second;
			copied_ref->SetReferencedObjectNumber(copied_referenced_obj->GetObjectNumber());
			copied_ref->SetReferencedGenerationNumber(copied_referenced_obj->GetGenerationNumber());

			FixObjectReferences(map, visited, copied_referenced_obj);

			found = true;
			break;
		}

		assert(found && "Found reference which was not copied"); UNUSED(found);
	}

	if (ObjectUtils::IsType<MixedArrayObjectPtr>(copied)) {
		auto arr = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(copied);
		for (auto item : arr) {
			FixObjectReferences(map, visited, item);
		}
	}

	if (ObjectUtils::IsType<DictionaryObjectPtr>(copied)) {
		auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(copied);
		for (auto item : dict) {
			FixObjectReferences(map, visited, item.second);
		}
	}

	if (ObjectUtils::IsType<StreamObjectPtr>(copied)) {
		auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(copied);
		auto dict = stream->GetHeader();
		for (auto item : dict) {
			FixObjectReferences(map, visited, item.second);
		}
	}
}

void File::ShallowCopyObject(std::map<ObjectPtr, ObjectPtr>& map, ObjectPtr original) {
	auto found = map.find(original);
	if (map.end() != found) {
		// already copied
		return;
	}

	// Copy only indirect objects
	if (original->IsIndirect()) {
		auto new_obj = ShallowCopyObject(original);

		auto pair = std::pair<ObjectPtr, ObjectPtr>(original, new_obj);
		map.insert(pair);
	}
}

ObjectPtr File::ShallowCopyObject(ObjectPtr original) {
	ObjectPtr new_obj(original->Clone());

	if (original->IsIndirect()) {
		auto new_entry = _xref->AllocateNewEntry();
		new_entry->SetReference(new_obj);
		new_entry->SetInitialized();
	}

	new_obj->SetFile(GetWeakReference<File>());
	new_obj->SetInitialized();

	return new_obj;
}

void File::DeepCopyObject(std::map<ObjectPtr, ObjectPtr>& map, std::map<ObjectPtr, bool>& visited, ObjectPtr original) {
	auto has_visited = visited.find(original);
	if (visited.end() != has_visited && has_visited->second) {
		// already visited
		return;
	}

	// Visit
	visited[original] = true;

	ShallowCopyObject(map, original);

	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(original)) {
		auto ref = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(original);
		auto referenced_obj = ref->GetReferencedObject();
		DeepCopyObject(map, visited, referenced_obj);
	}

	if (ObjectUtils::IsType<MixedArrayObjectPtr>(original)) {
		auto arr = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(original);
		for (auto item : arr) {
			DeepCopyObject(map, visited, item);
		}
	}

	if (ObjectUtils::IsType<DictionaryObjectPtr>(original)) {
		auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(original);
		for (auto item : dict) {
			DeepCopyObject(map, visited, item.second);
		}
	}

	if (ObjectUtils::IsType<StreamObjectPtr>(original)) {
		auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(original);
		auto dict = stream->GetHeader();
		for (auto item : dict) {
			DeepCopyObject(map, visited, item.second);
		}
	}
}

std::vector<ObjectPtr> File::DeepCopyObject(ObjectPtr object) {
	std::vector<ObjectPtr> objects { object };
	return DeepCopyObjects(objects);
}

std::vector<ObjectPtr> File::DeepCopyObjects(const std::vector<ObjectPtr>& objects) {
	// Original to copied mapping
	std::map<ObjectPtr, ObjectPtr> map;

	std::map<ObjectPtr, bool> copy_visited;
	for (auto obj : objects) {
		DeepCopyObject(map, copy_visited, obj);
	}

	// Fix references
	std::map<ObjectPtr, bool> ref_visited;
	for (auto check : map) {
		FixObjectReferences(map, ref_visited, check.second);
	}

	std::vector<ObjectPtr> result;
	for (auto item : objects) {
		auto found = map.find(item);
		assert(found != map.end());
		result.push_back(found->second);
	}

	return result;
}

IInputStreamPtr File::GetInputStream(void) {
	return make_deferred<InputStream>(_input);
}

IOutputStreamPtr File::GetOutputStream(void) {
	return make_deferred<OutputStream>(_input);
}

IInputOutputStreamPtr File::GetInputOutputStream(void) {
	return make_deferred<InputOutputStream>(_input);
}

BufferPtr File::GetByteRange(types::stream_size begin, types::size_type length) const {
	if (!_initialized) {
		throw FileNotInitializedException(_filename);
	}

	InputStream stream(_input);
	stream.SetInputPosition(begin);
	return stream.Read(length);
}

IInputStreamPtr File::GetByteRangeStream(types::stream_size begin, types::size_type length) const {
	if (!_initialized) {
		throw FileNotInitializedException(_filename);
	}

	// Avoid huge allocation and use filtering stream buffer
	InputStream stream(_input);
	stream.SetInputPosition(begin);
	auto buffer = stream.Read(length);
	return buffer->ToInputStream();
}

} // syntax
} // vanillapdf
