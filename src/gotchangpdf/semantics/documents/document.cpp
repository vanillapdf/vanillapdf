#include "precompiled.h"
#include "document.h"
#include "file_writer.h"

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

