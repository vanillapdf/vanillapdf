#include "precompiled.h"
#include "document.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace syntax;

		Document::Document(const std::string& filename) : _holder(filename) { _holder->Value()->Initialize(); }
		Document::Document(syntax::FileHolderPtr holder) : _holder(holder) { assert(holder->Value()); }

		CatalogPtr Document::GetDocumentCatalog(void) const
		{
			auto chain = _holder->Value()->GetXrefChain();
			auto xref = chain->Begin()->Value();
			auto dictionary = xref->GetTrailerDictionary();
			auto root = dictionary->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Root);
			return Catalog(root);
		}

		bool Document::GetDocumentInfo(OutputDocumentInfoPtr& result) const
		{
			auto chain = _holder->Value()->GetXrefChain();
			auto xref = chain->Begin()->Value();
			auto dictionary = xref->GetTrailerDictionary();

			if (!dictionary->Contains(constant::Name::Info))
				return false;

			auto info = dictionary->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Info);
			result = DocumentInfoPtr(info);
			return true;
		}

		void Document::Save(const std::string& path)
		{
			_holder->Value()->SaveAs(path);
		}

		void Document::SaveIncremental(const std::string& path)
		{
			XrefChainPtr chain = _holder->Value()->GetXrefChain();

			XrefTablePtr new_table;
			XrefFreeEntryPtr free_entry(0, (types::ushort)65535);
			new_table->Add(free_entry);

			bool new_entries = false;
			auto end = chain->End();
			for (auto it = chain->Begin(); *it != *end; ++(*it)) {
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
					if (!used_entry->IsDirty())
						continue;

					auto obj = used_entry->GetReference();
					auto new_obj_number = obj->GetObjectNumber();
					auto new_gen_number = obj->GetGenerationNumber();
					auto new_offset = 0;

					XrefUsedEntryPtr new_entry(
						new_obj_number,
						new_gen_number,
						new_offset);

					new_entry->SetReference(obj);
					new_entry->SetInitialized();
					new_table->Add(new_entry);
				}
			}

			// Construct new trailer for latest xref table
			DictionaryObjectPtr new_trailer;

			// Set size of new entries
			IntegerObjectPtr new_size(new_table->Size());
			new_trailer->Insert(constant::Name::Size, new_size);

			XrefBasePtr prev_table = chain->Begin()->Value();
			auto prev_trailer = prev_table->GetTrailerDictionary();

			// Set document ID
			if (prev_trailer->Contains(constant::Name::ID)) {
				ContainableObjectPtr id = prev_trailer->Find(constant::Name::ID);
				ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(id);
				new_trailer->Insert(constant::Name::ID, cloned);
			}

			// Set document Info
			if (prev_trailer->Contains(constant::Name::Info)) {
				ContainableObjectPtr id = prev_trailer->Find(constant::Name::Info);
				ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(id);
				new_trailer->Insert(constant::Name::Info, cloned);
			}

			// Skip table, if there were no dirty entries
			if (new_entries) {
				auto last_xref_offset = prev_table->GetOffset();
				IntegerObjectPtr new_offset(last_xref_offset);
				new_trailer->Insert(constant::Name::Prev, new_offset);
			}

			// Add encryption entry to trailer
			if (_holder->Value()->IsEncrypted()) {
				ObjectPtr obj = _holder->Value()->GetEncryptionDictionary();

				if (ObjectUtils::IsType<DictionaryObjectPtr>(obj)) {
					DictionaryObjectPtr encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(obj);
					DictionaryObjectPtr cloned = ObjectUtils::Clone<DictionaryObjectPtr>(encryption_dictionary);
					new_trailer->Insert(constant::Name::Encrypt, encryption_dictionary);
				}
			}

			// Set trailer
			new_table->SetTrailerDictionary(new_trailer);

			// File writer does all the dirty work
			_holder->Value()->SaveIncremental(path, new_table);
		}
	}
}

