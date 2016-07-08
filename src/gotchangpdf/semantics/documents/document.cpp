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

