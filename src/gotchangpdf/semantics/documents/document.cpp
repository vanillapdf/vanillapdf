#include "precompiled.h"
#include "document.h"
#include "file_writer.h"
#include "contents.h"
#include "annotations.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace syntax;

		Document::Document(const std::string& filename) { _holder->Open(filename); _holder->Value()->Initialize(); }
		Document::Document(syntax::FileHolderPtr holder) : _holder(holder) { assert(holder->Value()); }

		CatalogPtr Document::GetDocumentCatalog(void)
		{
			if (!m_catalog.empty()) {
				return m_catalog;
			}

			auto chain = _holder->Value()->GetXrefChain();
			auto xref = chain->Begin()->Value();
			auto dictionary = xref->GetTrailerDictionary();
			auto root = dictionary->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Root);

			auto catalog = CatalogPtr(root);
			catalog->SetDocument(GetWeakReference<Document>());

			m_catalog = catalog;
			return m_catalog;
		}

		bool Document::GetDocumentInfo(OutputDocumentInfoPtr& result)
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
			doc_info->SetDocument(GetWeakReference<Document>());

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

		void Document::AppendContent(DocumentPtr other)
		{
			auto original_catalog = GetDocumentCatalog();
			auto original_pages = original_catalog->Pages();
			auto original_page_count = original_pages->PageCount();

			auto other_catalog = other->GetDocumentCatalog();
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

			// First merge all page objects
			for (decltype(other_page_count) i = 0; i < other_page_count; ++i) {
				auto new_obj = new_objects[i];

				assert(ObjectUtils::IsType<DictionaryObjectPtr>(new_obj) && "Page object is not dictionary");
				if (!ObjectUtils::IsType<DictionaryObjectPtr>(new_obj)) {
					continue;
				}

				DictionaryObjectPtr new_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(new_obj);
				PageObjectPtr new_page(new_dictionary);
				original_pages->Append(new_page);
			}

			// Fix missing named destinations
			auto merged_pages_count = original_pages->PageCount();
			for (decltype(other_page_count) i = original_page_count; i < merged_pages_count; ++i) {
				auto new_page = original_pages->Page(i + 1);

				OutputPageAnnotationsPtr annots_ptr;
				bool has_annots = new_page->GetAnnotations(annots_ptr);
				if (has_annots) {
					auto annots = annots_ptr.GetValue();
					auto annots_size = annots->Size();
					for (decltype(annots_size) j = 0; j < annots_size; ++j) {
						auto annotation = annots->At(j);
						auto raw_annotation = annotation->GetObject();

						// Ignore annotations without destination
						if (!raw_annotation->Contains(constant::Name::Dest)) {
							continue;
						}

						auto destination_obj = raw_annotation->Find(constant::Name::Dest);

						// we care only for named destinations
						if (!ObjectUtils::IsType<NameObjectPtr>(destination_obj)) {
							continue;
						}

						auto destination_name = ObjectUtils::ConvertTo<NameObjectPtr>(destination_obj);

						OutputNamedDestinationsPtr oritinal_destinations_ptr;
						bool has_original_destinations = original_catalog->Destinations(oritinal_destinations_ptr);
						if (!has_original_destinations) {
							break;
						}

						auto original_destinations = oritinal_destinations_ptr.GetValue();
						bool original_contains = original_destinations->Contains(destination_name);
						if (original_contains) {
							continue;
						}

						// Found unreferenced named destination

						OutputNamedDestinationsPtr other_destinations_ptr;
						bool has_other_destinations = other_catalog->Destinations(other_destinations_ptr);
						if (!has_other_destinations) {
							break;
						}

						auto other_destinations = other_destinations_ptr.GetValue();
						bool other_contains = other_destinations->Contains(destination_name);
						if (!other_contains) {
							// This is really interesting
							continue;
						}

						auto other_destination = other_destinations->Find(destination_name);
						auto other_destination_obj = other_destination->GetObject();

						// Create only shallow copies
						auto cloned_destination_obj = file->ShallowCopyObject(other_destination_obj);
						auto cloned_destination = DestinationBase::Create(cloned_destination_obj, GetWeakReference<Document>());
						auto cloned_destination_page = cloned_destination->GetPage();

						if (ObjectUtils::IsType<IndirectObjectReferencePtr>(cloned_destination_page)) {
							auto cloned_page_reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(cloned_destination_page);

							// Find matching object for the other file and fix
							bool found = false;
							for (decltype(other_page_count) k = 0; k < other_page_count; ++k) {
								auto other_page = other_pages->Page(k + 1);
								auto other_obj = other_page->GetObject();

								if (other_obj->GetObjectNumber() != cloned_page_reference->GetReferencedObjectNumber()
									|| other_obj->GetGenerationNumber() != cloned_page_reference->GetReferencedGenerationNumber()) {
									continue;
								}

								// k + 1 is page index in the other file
								// we need to fix reference to point to object original_page_count + k + 1

								auto new_referenced_page = original_pages->Page(original_page_count + k + 1);
								auto new_referenced_obj = new_referenced_page->GetObject();
								cloned_page_reference->SetReferencedObject(new_referenced_obj);
								found = true;
								break;
							}

							assert(found && "Referenced object could not be found in the other file");
						}

						if (ObjectUtils::IsType<IntegerObjectPtr>(cloned_destination_page)) {
							// Increment referring page number by number of pages from original document
							auto cloned_page_index = ObjectUtils::ConvertTo<IntegerObjectPtr>(cloned_destination_page);
							cloned_page_index->SetValue(cloned_page_index->GetValue() + original_page_count);
						}

						original_destinations->Insert(destination_name, cloned_destination);
					}
				}
			}
		}
	}
}

