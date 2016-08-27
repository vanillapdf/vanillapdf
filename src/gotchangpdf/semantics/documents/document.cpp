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
				auto stream_object = ObjectUtils::ConvertTo<StreamObjectPtr>(object);

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
	}
}

