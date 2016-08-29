#include "precompiled.h"
#include "document.h"
#include "file_writer.h"
#include "contents.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace syntax;

		Document::Document(const std::string& filename) { _holder->Open(filename); _holder->Value()->Initialize(); }
		Document::Document(syntax::FileHolderPtr holder) : _holder(holder) { assert(holder->Value()); }

		CatalogPtr Document::GetDocumentCatalog(void) const
		{
			if (!m_catalog.empty()) {
				return m_catalog;
			}

			auto chain = _holder->Value()->GetXrefChain();
			auto xref = chain->Begin()->Value();
			auto dictionary = xref->GetTrailerDictionary();
			auto root = dictionary->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Root);

			m_catalog = CatalogPtr(root);
			return m_catalog;
		}

		bool Document::GetDocumentInfo(OutputDocumentInfoPtr& result) const
		{
			if (!m_info.empty()) {
				result = m_info;
				return true;
			}

			auto chain = _holder->Value()->GetXrefChain();
			auto xref = chain->Begin()->Value();
			auto dictionary = xref->GetTrailerDictionary();

			if (!dictionary->Contains(constant::Name::Info))
				return false;

			auto info = dictionary->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Info);
			DocumentInfoPtr doc_info(info);
			m_info = doc_info;
			result = doc_info;
			return true;
		}

		void Document::Save(const std::string& path)
		{
			auto destination = File::Create(path);

			// On before Save
			auto catalog = GetDocumentCatalog();
			auto pages = catalog->Pages();
			auto page_count = pages->PageCount();
			for (decltype(page_count) i = 1; i < page_count + 1; ++i) {
				auto page = pages->Page(i);

				OutputContentsPtr page_contents_ptr;
				bool has_contents = page->GetContents(page_contents_ptr);
				if (!has_contents) {
					continue;
				}

				ContentsPtr page_contents = page_contents_ptr.GetValue();
				if (!page_contents->IsDirty()) {
					continue;
				}

				std::stringstream ss;
				for (auto instruction : *page_contents->Instructions()) {
					ss << instruction->ToPdf() << std::endl;
				}

				auto object = page_contents->GetObject();

				StreamObjectPtr stream_object;
				if (ObjectUtils::IsType<StreamObjectPtr>(object)) {
					stream_object = ObjectUtils::ConvertTo<StreamObjectPtr>(object);
				}

				if (ObjectUtils::IsType<ArrayObjectPtr<StreamObjectPtr>>(object)) {
					auto stream_array = ObjectUtils::ConvertTo<ArrayObjectPtr<StreamObjectPtr>>(object);
					auto stream_array_size = stream_array->Size();

					assert(0 != stream_array_size && "Content stream array is empty");
					if (0 == stream_array_size) {
						throw GeneralException("Content stream array is empty");
					}

					for (decltype(stream_array_size) j = 0; j < stream_array_size; ++j) {
						auto referenced_stram = stream_array->At(j);

						// TODO divide output into multiple streams
						BufferPtr empty_body;
						referenced_stram->SetBody(empty_body);
					}

					stream_object = stream_array->At(0);
				}

				std::string string_body = ss.str();
				BufferPtr new_body(string_body.begin(), string_body.end());
				stream_object->SetBody(new_body);
			}

			FileWriter writer;
			writer.Write(_holder->Value(), destination);
		}

		void Document::SaveIncremental(const std::string& path)
		{
			auto destination = File::Create(path);

			FileWriter writer;
			writer.WriteIncremental(_holder->Value(), destination);
		}

		void Document::AppendContent(const Document& other)
		{
			auto other_catalog = other.GetDocumentCatalog();
			auto other_pages = other_catalog->Pages();
			auto other_page_count = other_pages->PageCount();

			std::vector<ObjectPtr> merge_objects;
			for (decltype(other_page_count) i = 0; i < other_page_count; ++i) {
				auto other_page = other_pages->Page(i + 1);
				auto other_obj = other_page->GetObject();
				merge_objects.push_back(other_obj);
			}

			auto file = _holder->Value();
			auto new_objects = file->DeepCopyObjects(merge_objects);

			for (decltype(other_page_count) i = 0; i < other_page_count; ++i) {
				auto new_obj = new_objects[i];

				assert(ObjectUtils::IsType<DictionaryObjectPtr>(new_obj) && "Page object is not dictionary");
				if (!ObjectUtils::IsType<DictionaryObjectPtr>(new_obj)) {
					continue;
				}

				auto catalog = GetDocumentCatalog();
				auto pages = catalog->Pages();

				DictionaryObjectPtr new_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(new_obj);
				PageObjectPtr new_page(new_dictionary);
				pages->Append(new_page);
			}
		}
	}
}

