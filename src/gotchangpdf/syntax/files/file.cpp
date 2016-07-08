#include "precompiled.h"
#include "file.h"

#include "raw_reverse_stream.h"
#include "parser.h"
#include "reverse_parser.h"
#include "exception.h"
#include "encryption.h"
#include "string_object.h"

#include "xref_chain.h"
#include "header.h"

namespace gotchangpdf
{
	namespace syntax
	{
		using namespace std;

		static const int invalid_xref_offset = -1;

		std::shared_ptr<File> File::Open(const std::string& path)
		{
			return std::shared_ptr<File>(new File(path));
		}

		std::shared_ptr<File> File::Create(const std::string& path)
		{
			auto result = std::shared_ptr<File>(new File(path));
			result->_input = make_shared<FileDevice>();
			result->_input->open(path,
				ios_base::in | ios_base::out | ios_base::binary | ios::trunc);

			result->_initialized = true;
			return result;
		}

		File::File(const std::string& path)
			: _full_path(path)
		{
			_filename = extract_filename(path);
			LOG_WARNING_GLOBAL << "File constructor " << _filename;
		}

		File::~File(void)
		{
			LOG_WARNING_GLOBAL << "File destructor " << _filename;

			if (nullptr != _input)
			{
				_input->close();
				_input = nullptr;
			}

			_cache.clear();
		}

		void File::Initialize()
		{
			LOG_DEBUG(_filename) << "Initialize";

			if (_initialized)
				return;

			_input = make_shared<FileDevice>();
			_input->open(_full_path,
				ios_base::in | ios_base::out | ios_base::binary | ios::ate);

			if (!_input || !_input->good())
				throw GeneralException("Could not open file");

			auto file_size = _input->tellg();
			auto stream = Parser(shared_from_this(), *_input);

			_header = stream.ReadHeader(0);
			auto offset = GetLastXrefOffset(file_size);

			if (invalid_xref_offset == offset) {
				auto xref = stream.FindAllObjects();
				_xref->Append(xref);
				_initialized = true;
				return;
			}

			try
			{
				ReadXref(offset);
			}
			catch (NotSupportedException&) {
				throw;
			}
			catch (...) {
				auto xref = stream.FindAllObjects();
				_xref->Append(xref);
			}

			// After xref informations are parsed, check for encryption
			auto dictionary = _xref->Begin()->Value()->GetTrailerDictionary();
			if (dictionary->Contains(constant::Name::Encrypt)) {
				_encryption_dictionary = dictionary->Find(constant::Name::Encrypt);
			}

			_initialized = true;
		}

		bool File::SetEncryptionKey(const IEncryptionKey& key)
		{
			if (!IsEncrypted()) {
				return false;
			}

			auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
			auto filter = dict->FindAs<NameObjectPtr>(constant::Name::Filter);
			auto sub_filter = dict->FindAs<NameObjectPtr>(constant::Name::SubFilter);
			EncryptionAlgorithm algorithm = EncryptionAlgorithm::None;

			IntegerObjectPtr length_bits = 40;
			if (dict->Contains(constant::Name::Length)) {
				length_bits = dict->FindAs<IntegerObjectPtr>(constant::Name::Length);
				assert(length_bits->GetValue() % 8 == 0 && "Key length is not multiplier of 8");
			}

			ArrayObjectPtr<StringObjectPtr> recipients;
			if (sub_filter == constant::Name::AdbePkcs7s3) {
				recipients = dict->FindAs<ArrayObjectPtr<StringObjectPtr>>(constant::Name::Recipients);
			}

			if (sub_filter == constant::Name::AdbePkcs7s4) {
				recipients = dict->FindAs<ArrayObjectPtr<StringObjectPtr>>(constant::Name::Recipients);
			}

			if (sub_filter == constant::Name::AdbePkcs7s5) {

				do
				{
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
							assert(length_bits->GetValue() == 256 && "AESV3 length is not 256 bits");
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

			try
			{
				_decryption_key = EncryptionUtils::GetRecipientKey(recipients, length_bits, algorithm, key);
				return true;
			}
			catch (...) {
				return false;
			}
		}

		bool File::SetEncryptionPassword(const std::string& password)
		{
			Buffer buffer(password.begin(), password.end());
			return SetEncryptionPassword(buffer);
		}

		bool File::SetEncryptionPassword(const Buffer& password)
		{
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

			IntegerObjectPtr length_bits = 40;
			if (dict->Contains(constant::Name::Length)) {
				length_bits = dict->FindAs<IntegerObjectPtr>(constant::Name::Length);
				assert(length_bits->GetValue() % 8 == 0 && "Key length is not multiplier of 8");
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

		bool File::IsEncrypted(void) const
		{
			return !_encryption_dictionary.empty() && _encryption_dictionary != NullObject::GetInstance();
		}

		BufferPtr File::DecryptStream(const Buffer& data,
			types::big_uint objNumber,
			types::ushort genNumber)
		{
			if (!IsEncrypted()) {
				return data;
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
			types::ushort genNumber)
		{
			if (!IsEncrypted()) {
				return data;
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
			const NameObject& filter_name)
		{
			if (!IsEncrypted()) {
				return data;
			}

			EncryptionAlgorithm algorithm = GetEncryptionAlgorithmForFilter(filter_name);
			return DecryptData(data, objNumber, genNumber, algorithm);
		}

		BufferPtr File::DecryptData(const Buffer& data,
			types::big_uint objNumber,
			types::ushort genNumber,
			EncryptionAlgorithm alg)
		{
			if (!IsEncrypted()) {
				return data;
			}

			auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
			auto dictionary_object_number = encryption_dictionary->GetObjectNumber();
			auto dictionary_generation_number = encryption_dictionary->GetGenerationNumber();

			// data inside encryption dictionary are not encrypted
			if (objNumber == 0 || (dictionary_object_number == objNumber && dictionary_generation_number == genNumber)) {
				return data;
			}

			// Any strings that are inside streams such as content streams and compressed object streams,
			// which themselves are encrypted
			auto object_entry = _xref->GetXrefEntry(objNumber, genNumber);
			if (object_entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
				return data;
			}

			if (_decryption_key.empty()) {
				// Encrypted documents shall be opened with default empty password
				SetEncryptionPassword("");
			}

			// AES 256 bits behaves differently
			// It was not part of the core specification, but it was added in extension
			if (_decryption_key->size() == 32 && alg == EncryptionAlgorithm::AES) {
				return EncryptionUtils::AESDecrypt(_decryption_key, 32, data);
			}

			BufferPtr object_key = EncryptionUtils::ComputeObjectKey(_decryption_key, objNumber, genNumber, alg);

			switch (alg)
			{
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
			types::ushort genNumber)
		{
			if (!IsEncrypted()) {
				return data;
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
			types::ushort genNumber)
		{
			if (!IsEncrypted()) {
				return data;
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
			const NameObject& filter_name)
		{
			if (!IsEncrypted()) {
				return data;
			}

			EncryptionAlgorithm algorithm = GetEncryptionAlgorithmForFilter(filter_name);
			return EncryptData(data, objNumber, genNumber, algorithm);
		}

		BufferPtr File::EncryptData(const Buffer& data,
			types::big_uint objNumber,
			types::ushort genNumber,
			EncryptionAlgorithm alg) const
		{
			if (!IsEncrypted()) {
				return data;
			}

			auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
			auto dictionary_object_number = encryption_dictionary->GetObjectNumber();
			auto dictionary_generation_number = encryption_dictionary->GetGenerationNumber();

			// data inside encryption dictionary are not encrypted
			if (objNumber == 0 || (dictionary_object_number == objNumber && dictionary_generation_number == genNumber)) {
				return data;
			}

			// AES 256 bits behaves differently
			// It was not part of the core specification, but it was added in extension
			if (_decryption_key->size() == 32 && alg == EncryptionAlgorithm::AES) {
				return EncryptionUtils::AESEncrypt(_decryption_key, 32, data);
			}

			BufferPtr object_key = EncryptionUtils::ComputeObjectKey(_decryption_key, objNumber, genNumber, alg);

			switch (alg)
			{
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

		EncryptionAlgorithm File::GetEncryptionAlgorithmForFilter(const NameObject& filter_name)
		{
			auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
			auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

			do
			{
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

		void File::ReadXref(types::stream_offset offset)
		{
			auto stream = Parser(shared_from_this(), *_input);

			do {
				auto xref = stream.ReadXref(offset);
				_xref->Append(xref);

				if (xref->GetTrailerDictionary()->Contains(constant::Name::XRefStm)) {
					auto stm_offset = xref->GetTrailerDictionary()->FindAs<IntegerObjectPtr>(constant::Name::XRefStm)->GetValue();
					auto xref_stm = stream.ReadXref(stm_offset);

					assert(!xref_stm->GetTrailerDictionary()->Contains(constant::Name::Prev));
					_xref->Append(xref_stm);
				}

				if (!xref->GetTrailerDictionary()->Contains(constant::Name::Prev)) {
					break;
				}

				offset = xref->GetTrailerDictionary()->FindAs<IntegerObjectPtr>(constant::Name::Prev)->GetValue();
			} while (true);
		}

		types::stream_offset File::GetLastXrefOffset(types::stream_size file_size)
		{
			try
			{
				ReverseStream raw_reversed(*_input, file_size);
				auto reverse_stream = ReverseParser(raw_reversed);
				return reverse_stream.ReadLastXrefOffset();
			}
			catch (...) {
				LOG_ERROR(_filename) << "Could not find xref offset, using fallback mechanism";
				return invalid_xref_offset;
			}
		}

		ObjectPtr File::GetIndirectObject(types::big_uint objNumber,
			types::ushort genNumber) const
		{
			LOG_DEBUG(_filename) << "GetIndirectObject " << objNumber << " and " << genNumber;

			if (!_initialized)
				throw FileNotInitializedException(_filename);

			if (!_xref->Contains(objNumber, genNumber))
				return NullObject::GetInstance();

			auto item = _xref->GetXrefEntry(objNumber, genNumber);

			if (!item->InUse())
				return NullObject::GetInstance();

			switch (item->GetUsage()) {
			case XrefEntryBase::Usage::Used:
			{
				auto used = XrefUtils::ConvertTo<XrefUsedEntryPtr>(item);
				return used->GetReference();
			}
			case XrefEntryBase::Usage::Compressed:
			{
				auto compressed = XrefUtils::ConvertTo<XrefCompressedEntryPtr>(item);
				return compressed->GetReference();
			}
			case XrefEntryBase::Usage::Null:
				LOG_ERROR(_filename) << "Xref entry type is null for object " << objNumber << " " << genNumber;
				return NullObject::GetInstance();
			case XrefEntryBase::Usage::Free:
				LOG_ERROR(_filename) << "Xref entry type is free for object " << objNumber << " " << genNumber << " and InUse() returned true";
				assert(!"Current entry is supposed to be InUse(), while it's type is Free");
				return NullObject::GetInstance();
			default:
				throw GeneralException("Unknown entry type");
			}
		}

		HeaderPtr File::GetHeader(void) const
		{
			if (!_initialized)
				throw FileNotInitializedException(_filename);

			return _header;
		}

		XrefChainPtr File::GetXrefChain(void) const
		{
			if (!_initialized)
				throw FileNotInitializedException(_filename);

			return _xref;
		}
	}
}
