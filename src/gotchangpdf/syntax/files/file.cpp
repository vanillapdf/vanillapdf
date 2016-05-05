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

#include <iomanip>

#include <openssl/md5.h>

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

			//std::string dest("C:\\Users\\Gotcha\\Documents\\");
			//dest += _filename;
			//SaveAs(dest);
		}

		void File::SetEncryptionKey(const IEncryptionKey& key)
		{
			auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
			auto filter = dict->FindAs<NameObjectPtr>(constant::Name::Filter);
			auto sub_filter = dict->FindAs<NameObjectPtr>(constant::Name::SubFilter);
			EncryptionAlgorithm algorithm = EncryptionAlgorithm::None;

			ArrayObjectPtr<StringObjectPtr> recipients;

			IntegerObjectPtr length_bits = 40;
			if (dict->Contains(constant::Name::Length)) {
				length_bits = dict->FindAs<IntegerObjectPtr>(constant::Name::Length);
				assert(length_bits->Value() % 8 == 0 && "Key length is not multiplier of 8");
			}

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
							assert(length_bits->Value() % 8 == 0 && "Key length is not multiplier of 8");
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

			_decryption_key = EncryptionUtils::GetRecipientKey(recipients, length_bits, algorithm, key);
		}

		void File::SetEncryptionPassword(const std::string& password)
		{
			Buffer buffer(password.begin(), password.end());
			SetEncryptionPassword(buffer);
		}

		void File::SetEncryptionPassword(const Buffer& password)
		{
			if (!IsEncrypted()) {
				return;
			}

			auto ids = _xref->Begin()->Value()->GetTrailerDictionary()->FindAs<ArrayObjectPtr<StringObjectPtr>>(constant::Name::ID);
			auto id = ids->At(0);

			auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
			auto filter = dict->FindAs<NameObjectPtr>(constant::Name::Filter);

			if (filter != constant::Name::Standard) {
				return;
			}

			auto user_value = dict->FindAs<StringObjectPtr>(constant::Name::U);
			auto owner_value = dict->FindAs<StringObjectPtr>(constant::Name::O);
			auto permissions = dict->FindAs<IntegerObjectPtr>(constant::Name::P);
			auto revision = dict->FindAs<IntegerObjectPtr>(constant::Name::R);
			auto version = dict->FindAs<IntegerObjectPtr>(constant::Name::V);

			IntegerObjectPtr length_bits = 40;
			if (dict->Contains(constant::Name::Length)) {
				length_bits = dict->FindAs<IntegerObjectPtr>(constant::Name::Length);
				assert(length_bits->Value() % 8 == 0 && "Key length is not multiplier of 8");
			}

			// Pad password with predefined scheme
			BufferPtr padPassword = EncryptionUtils::PadTruncatePassword(password);
			BufferPtr encrypted_owner_data = EncryptionUtils::ComputeEncryptedOwnerData(padPassword, dict);

			Buffer decryption_key;

			// Check if entered password was owner password
			if (EncryptionUtils::CheckKey(encrypted_owner_data, id->Value(), owner_value->Value(), user_value->Value(), permissions, revision, length_bits, decryption_key)) {
				_decryption_key = decryption_key;
				return;
			}

			// Check if entered password was user password
			if (EncryptionUtils::CheckKey(padPassword, id->Value(), owner_value->Value(), user_value->Value(), permissions, revision, length_bits, decryption_key)) {
				_decryption_key = decryption_key;
				return;
			}

			throw GeneralException("Bad password entered");
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

			auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
			auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

			do
			{
				if (version != 4 && version != 5)
					break;

				if (!encryption_dictionary->Contains(constant::Name::CF))
					break;

				auto crypt_filter_dictionary = encryption_dictionary->FindAs<DictionaryObjectPtr>(constant::Name::CF);
				if (!crypt_filter_dictionary->Contains(filter_name))
					break;

				auto crypt_filter = crypt_filter_dictionary->FindAs<DictionaryObjectPtr>(filter_name);
				if (!crypt_filter->Contains(constant::Name::CFM))
					break;

				auto method = crypt_filter->FindAs<NameObjectPtr>(constant::Name::CFM);
				if (method == constant::Name::AESV2 || method == constant::Name::AESV3) {
					return DecryptData(data, objNumber, genNumber, EncryptionAlgorithm::AES);
				}

				if (method == constant::Name::None) {
					return DecryptData(data, objNumber, genNumber, EncryptionAlgorithm::None);
				}

				if (method == constant::Name::V2) {
					return DecryptData(data, objNumber, genNumber, EncryptionAlgorithm::RC4);
				}
			} while (false);

			return DecryptData(data, objNumber, genNumber, EncryptionAlgorithm::RC4);
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

			if (_decryption_key.empty()) {
				// Encrypted documents shall be opened with default empty password
				SetEncryptionPassword("");

				//PKCS12Key key = PKCS12Key("C:\\Users\\Gotcha\\Documents\\it2u\\cert\\TestUser4.pfx", Buffer("a"));
				//SetEncryptionKey(key);
			if (_decryption_key->size() == 32 && alg == EncryptionAlgorithm::AES) {
				return EncryptionUtils::AESDecrypt(_decryption_key, 32, data);
			}

			BufferPtr object_key(MD5_DIGEST_LENGTH);

			uint8_t object_info[5];
			object_info[0] = objNumber & 0xFF;
			object_info[1] = (objNumber >> 8) & 0xFF;
			object_info[2] = (objNumber >> 16) & 0xFF;
			object_info[3] = (genNumber) & 0xFF;
			object_info[4] = (genNumber >> 8) & 0xFF;

			MD5_CTX ctx;
			MD5_Init(&ctx);
			MD5_Update(&ctx, _decryption_key->data(), _decryption_key->size());
			MD5_Update(&ctx, object_info, sizeof(object_info));

			if (alg == EncryptionAlgorithm::AES) {
				MD5_Update(&ctx, &AES_ADDITIONAL_SALT[0], AES_ADDITIONAL_SALT_LENGTH);
			}

			MD5_Final((unsigned char*)object_key->data(), &ctx);

			auto key_length = std::min(_decryption_key->size() + 5, 16u);

			switch (alg)
			{
			default:
			case EncryptionAlgorithm::None:
				// The application shall not decrypt data but shall direct the input stream to the security handler for decryption.
				// No clue
			case EncryptionAlgorithm::RC4:
				return EncryptionUtils::ComputeRC4(object_key, key_length, data);
			case EncryptionAlgorithm::AES:
				return EncryptionUtils::AESDecrypt(object_key, key_length, data);
			}
		}

		BufferPtr File::EncryptData(const Buffer& data,
			types::big_uint objNumber,
			types::ushort genNumber) const
		{
			if (!IsEncrypted())
				return data;

			// do work
			return data;
		}

		void File::ReadXref(types::stream_offset offset)
		{
			auto stream = Parser(shared_from_this(), *_input);

			do {
				auto xref = stream.ReadXref(offset);
				_xref->Append(xref);

				if (xref->GetTrailerDictionary()->Contains(constant::Name::XRefStm)) {
					auto stm_offset = xref->GetTrailerDictionary()->FindAs<IntegerObjectPtr>(constant::Name::XRefStm)->Value();
					auto xref_stm = stream.ReadXref(stm_offset);

					assert(!xref_stm->GetTrailerDictionary()->Contains(constant::Name::Prev));
					_xref->Append(xref_stm);
				}

				if (!xref->GetTrailerDictionary()->Contains(constant::Name::Prev)) {
					break;
				}

				offset = xref->GetTrailerDictionary()->FindAs<IntegerObjectPtr>(constant::Name::Prev)->Value();
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

		// experimental
		void File::SaveAs(const std::string& path)
		{
			if (!_initialized)
				throw FileNotInitializedException(_filename);

			std::fstream output;
			output.open(path, ios_base::out | ios_base::binary);

			if (!output || !output.good())
				throw GeneralException("Could not open file");

			auto ver = _header->GetVersion();
			output << "%PDF-1." << static_cast<int>(ver) << endl;

			auto end = _xref->End();
			for (auto it = _xref->Begin(); *it != *end; ++(*it)) {
				auto xref_base = it->Value();

				if (xref_base->GetType() == XrefBase::Type::Stream)
					continue;

				auto xref_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(xref_base);
				auto table_size = xref_table->Size();
				auto table_items = xref_table->Entries();
				for (decltype(table_size) i = 0; i < table_size; ++i) {
					auto entry = table_items[i];

					if (!entry->InUse())
						continue;

					auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);
					auto obj = used_entry->GetReference();
					auto obj_str = obj->ToPdf();

					//TODO: This is kinda hacky
					// make new xref table for storing modified values
					auto obj_offset = output.tellg();
					used_entry->SetOffset(obj_offset);
					obj->SetOffset(obj_offset);

					output << obj->GetObjectNumber() << " " << obj->GetGenerationNumber() << " " << "obj" << endl;
					output << obj_str << endl;
					output << "endobj" << endl;
				}

				auto last_offset = output.tellg();
				output << "xref" << endl;

				for (decltype(table_size) i = 0; i < table_size; ++i) {
					auto first = table_items[i];
					auto subsection_idx = first->GetObjectNumber();

					size_t subsection_size = 0;
					for (decltype(i) j = i; j < table_size; ++j, ++subsection_size) {
						auto next_entry = table_items[j];
						if (next_entry->GetObjectNumber() != gotchangpdf::SafeAddition<types::big_uint>(subsection_idx, j)) {
							break;
						}
					}

					output << subsection_idx << " " << subsection_size << endl;
					for (decltype(subsection_size) j = i; j < subsection_size; ++j) {
						auto entry = table_items[j];
						if (!entry->InUse()) {
							output << setfill('0') << setw(10) << 0;
							output << ' ';
							output << setfill('0') << setw(10) << 65535;
							output << ' ';
							output << 'f';
							output << ' ';
							output << '\n';
							continue;
						}

						auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);
						output << setfill('0') << setw(10) << used_entry->GetOffset();
						output << ' ';
						output << setfill('0') << setw(10) << used_entry->GetGenerationNumber();
						output << ' ';
						output << 'n';
						output << ' ';
						output << '\n';
					}

					i += subsection_size;
				}

				auto trailer = xref_table->GetTrailerDictionary();
				output << "trailer" << endl;
				output << trailer->ToPdf() << endl;
				output << "startxref" << endl;
				output << last_offset << endl;
				output << "%%EOF" << endl;
			}

			output.flush();
			output.close();
		}
	}
}
