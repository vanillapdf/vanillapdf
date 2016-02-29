#include "precompiled.h"
#include "file.h"

#include "raw_reverse_stream.h"
#include "parser.h"
#include "reverse_parser.h"
#include "exception.h"
#include "encryption.h"

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

			_initialized = true;

			//std::string dest("C:\\Users\\Gotcha\\Documents\\");
			//dest += _filename;
			//SaveAs(dest);
		}

		void File::SetPassword(const std::string& password)
		{
			Buffer buffer(password.begin(), password.end());
			SetPassword(buffer);
		}

		void File::SetPassword(const Buffer& password)
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

			// Pad password with predefined scheme
			auto padPassword = EncryptionUtils::PadTruncatePassword(password);

			// check owner key
			BufferPtr password_digest(MD5_DIGEST_LENGTH);
			MD5((unsigned char*)padPassword->data(), padPassword->size(), (unsigned char*)password_digest->data());

			auto encrypted_owner_data = EncryptionUtils::ComputeRC4(password_digest, 5, owner_value->Value());

			// Check if entered password was owner password
			if (CheckPassword(encrypted_owner_data, id->Value(), owner_value->Value(), user_value->Value(), permissions)) {
				return;
			}

			// Check if entered password was user password
			if (CheckPassword(padPassword, id->Value(), owner_value->Value(), user_value->Value(), permissions)) {
				return;
			}

			throw GeneralException("Bad password entered");
		}

		bool File::CheckPassword(BufferPtr input, BufferPtr document_id, BufferPtr owner_data, BufferPtr user_data, IntegerObjectPtr permissions)
		{
			BufferPtr decryption_key_digest(MD5_DIGEST_LENGTH);

			MD5_CTX ctx;
			MD5_Init(&ctx);
			MD5_Update(&ctx, input->data(), input->size());
			MD5_Update(&ctx, owner_data->data(), owner_data->size());

			auto permissions_value = permissions->Value();
			uint32_t permissions_raw = reinterpret_cast<uint32_t&>(permissions_value);
			uint8_t permissions_array[sizeof(permissions_raw)];
			permissions_array[0] = permissions_raw & 0xFF;
			permissions_array[1] = (permissions_raw >> 8) & 0xFF;
			permissions_array[2] = (permissions_raw >> 16) & 0xFF;
			permissions_array[3] = (permissions_raw >> 24) & 0xFF;

			MD5_Update(&ctx, permissions_array, sizeof(permissions_array));
			MD5_Update(&ctx, document_id->data(), document_id->size());
			MD5_Final((unsigned char*)decryption_key_digest->data(), &ctx);

			BufferPtr hardcoded_pad(&HARDCODED_PFD_PAD[0], HARDCODED_PFD_PAD_LENGTH);
			auto compare_data = EncryptionUtils::ComputeRC4(decryption_key_digest, 5, hardcoded_pad);

			if (compare_data->Equals(user_data)) {
				_decryption_key = BufferPtr(decryption_key_digest->begin(), decryption_key_digest->begin() + 5);
				return true;
			}

			return false;
		}

		bool File::IsEncrypted(void) const
		{
			return !_encryption_dictionary.empty() && _encryption_dictionary != NullObject::GetInstance();
		}

		BufferPtr File::DecryptData(BufferPtr data,
			types::big_uint objNumber,
			types::ushort genNumber)
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
				SetPassword("");
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
			MD5_Final((unsigned char*)object_key->data(), &ctx);

			auto key_length = std::min(_decryption_key->size() + 5, 16u);
			return EncryptionUtils::ComputeRC4(object_key, key_length, data);
		}

		BufferPtr File::EncryptData(BufferPtr data,
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

			// After xref informations are parsed, check for encryption
			auto dictionary = _xref->Begin()->Value()->GetTrailerDictionary();
			if (dictionary->Contains(constant::Name::Encrypt)) {
				_encryption_dictionary = dictionary->Find(constant::Name::Encrypt);
			}
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
