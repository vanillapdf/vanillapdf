#include "precompiled.h"
#include "semantics/objects/document.h"
#include "syntax/files/file_writer.h"
#include "semantics/objects/contents.h"
#include "semantics/objects/annotations.h"
#include "semantics/objects/name_dictionary.h"
#include "semantics/utils/semantic_utils.h"

namespace gotchangpdf {
namespace semantics {

using namespace syntax;

Document::Document(const std::string& filename) : m_holder(File::Open(filename)) {
	m_holder->Initialize();

	SemanticUtils::AddDocumentMapping(m_holder, GetWeakReference<Document>());
}

Document::Document(syntax::FilePtr holder) : m_holder(holder) {
	SemanticUtils::AddDocumentMapping(m_holder, GetWeakReference<Document>());
}

syntax::FilePtr Document::GetFile() const {
	return m_holder;
}

Document::~Document() {
	SemanticUtils::ReleaseMapping(m_holder->GetWeakReference<syntax::File>());
}

CatalogPtr Document::GetDocumentCatalog(void) {
	if (!m_catalog.empty()) {
		return m_catalog;
	}

	auto chain = m_holder->GetXrefChain();
	auto xref = chain->Begin()->Value();
	auto dictionary = xref->GetTrailerDictionary();
	auto root = dictionary->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Root);

	auto catalog = CatalogPtr(root);
	m_catalog = catalog;
	return m_catalog;
}

bool Document::GetDocumentInfo(OutputDocumentInfoPtr& result) {
	if (!m_info.empty()) {
		result = m_info.GetValue();
		return true;
	}

	auto chain = m_holder->GetXrefChain();
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

void Document::Save(const std::string& path) {
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
	writer.Write(m_holder, destination);
}

void Document::SaveIncremental(const std::string& path) {
	auto destination = File::Create(path);

	FileWriter writer;
	writer.WriteIncremental(m_holder, destination);
}

OutputNamedDestinationsPtr Document::CreateNamedDestinations(CatalogPtr catalog) {
	auto chain = m_holder->GetXrefChain();
	auto entry = chain->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto raw_catalog = catalog->GetObject();

	IndirectObjectReferencePtr ref(raw_dictionary);
	raw_catalog->Insert(constant::Name::Dests, ref);

	NamedDestinationsPtr result(raw_dictionary);
	return result;
}

OutputNameDictionaryPtr Document::CreateNameDictionary(CatalogPtr catalog) {
	auto chain = m_holder->GetXrefChain();
	auto entry = chain->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto raw_catalog = catalog->GetObject();

	IndirectObjectReferencePtr ref(raw_dictionary);
	raw_catalog->Insert(constant::Name::Names, ref);

	NameDictionaryPtr result(raw_dictionary);
	return result;
}

OutputNameTreePtr<DestinationPtr> Document::CreateNameTreeDestinations(NameDictionaryPtr dictionary) {
	auto chain = m_holder->GetXrefChain();
	auto entry = chain->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto name_dictionary = dictionary->GetObject();

	IndirectObjectReferencePtr ref(raw_dictionary);
	name_dictionary->Insert(constant::Name::Dests, ref);

	OutputNameTreePtr<DestinationPtr> result;
	auto found_destinations = dictionary->Dests(result);
	assert(found_destinations); found_destinations;

	return result;
}

void Document::FixDestinationPage(DocumentPtr other, ObjectPtr page) {
	assert(ObjectUtils::IsType<IndirectObjectReferencePtr>(page)
		|| ObjectUtils::IsType<IntegerObjectPtr>(page));
	if (!ObjectUtils::IsType<IndirectObjectReferencePtr>(page)
		&& !ObjectUtils::IsType<IntegerObjectPtr>(page)) {
		throw GeneralException("Unknown object type");
	}

	auto other_catalog = other->GetDocumentCatalog();
	auto other_pages = other_catalog->Pages();
	auto other_page_count = other_pages->PageCount();

	auto original_catalog = GetDocumentCatalog();
	auto original_pages = original_catalog->Pages();
	auto original_page_count_after_merge = original_pages->PageCount();

	// This function is called after document pages have been merged
	auto original_page_count = original_page_count_after_merge - other_page_count;

	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(page)) {
		auto cloned_page_reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(page);

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

	if (ObjectUtils::IsType<IntegerObjectPtr>(page)) {
		// Increment referring page number by number of pages from original document
		auto cloned_page_index = ObjectUtils::ConvertTo<IntegerObjectPtr>(page);
		cloned_page_index->SetValue(cloned_page_index->GetIntegerValue() + original_page_count);
	}
}

void Document::AppendContent(DocumentPtr other) {
	auto original_catalog = GetDocumentCatalog();
	auto original_pages = original_catalog->Pages();

	auto other_catalog = other->GetDocumentCatalog();
	auto other_pages = other_catalog->Pages();
	auto other_page_count = other_pages->PageCount();

	std::vector<ObjectPtr> merge_objects;
	for (decltype(other_page_count) i = 0; i < other_page_count; ++i) {
		auto other_page = other_pages->Page(i + 1);
		auto other_obj = other_page->GetObject();
		merge_objects.push_back(other_obj);
	}

	auto new_objects = m_holder->DeepCopyObjects(merge_objects);

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

	/// Gather basic properties

	OutputNamedDestinationsPtr original_destinations_ptr;
	OutputNameDictionaryPtr original_name_dictionary_ptr;
	OutputNamedDestinationsPtr other_destinations_ptr;
	OutputNameDictionaryPtr other_name_dictionary_ptr;

	bool has_original_destinations = original_catalog->Destinations(original_destinations_ptr);
	bool has_original_name_dictionary = original_catalog->Names(original_name_dictionary_ptr);
	bool has_other_destinations = other_catalog->Destinations(other_destinations_ptr);
	bool has_other_name_dictionary = other_catalog->Names(other_name_dictionary_ptr);

	if (has_other_destinations) {

		// Create missing destination objects
		if (!has_original_destinations) {
			original_destinations_ptr = CreateNamedDestinations(original_catalog);
			assert(!original_destinations_ptr.empty() && "CreateNamedDestinations returned empty result");
		}

		NamedDestinationsPtr original_destinations = original_destinations_ptr.GetValue();
		NamedDestinationsPtr other_destinations = other_destinations_ptr.GetValue();

		auto other_destinations_obj = other_destinations->GetObject();
		for (auto destination : *other_destinations_obj) {
			auto destination_name = destination.first;
			auto destination_value = destination.second;

			// Check for name conflicts
			assert(!original_destinations->Contains(destination_name) && "Name conflict");
			if (original_destinations->Contains(destination_name)) {
				throw NotSupportedException("Merge of conflicting names is not yet supported");
			}

			// Derefence destination in case it is indirect reference
			if (ObjectUtils::IsType<IndirectObjectReferencePtr>(destination_value)) {
				auto destination_reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(destination_value);
				destination_value = destination_reference->GetReferencedObjectAs<ContainableObjectPtr>();
			}

			// Create only shallow copies
			auto cloned_destination_obj = m_holder->ShallowCopyObject(destination_value);
			auto cloned_destination_unique_ptr = DestinationBase::Create(cloned_destination_obj);
			auto cloned_destination_raw_ptr = cloned_destination_unique_ptr.release();
			auto cloned_destination = DestinationPtr(cloned_destination_raw_ptr);
			auto cloned_destination_page = cloned_destination->GetPage();

			FixDestinationPage(other, cloned_destination_page);

			original_destinations->Insert(destination_name, cloned_destination);
		}
	}

	if (has_other_name_dictionary) {
		NameDictionaryPtr other_name_dictionary = other_name_dictionary_ptr.GetValue();

		OutputNameTreePtr<DestinationPtr> other_string_destinations_ptr;
		bool other_has_string_destinations = other_name_dictionary->Dests(other_string_destinations_ptr);
		if (other_has_string_destinations) {
			if (!has_original_name_dictionary) {
				original_name_dictionary_ptr = CreateNameDictionary(original_catalog);
				assert(!original_name_dictionary_ptr.empty() && "CreateNameDictionary returned empty result");
			}

			NameDictionaryPtr original_name_dictionary = original_name_dictionary_ptr.GetValue();

			OutputNameTreePtr<DestinationPtr> original_string_destinations_ptr;
			bool original_has_string_destinations = original_name_dictionary->Dests(original_string_destinations_ptr);
			if (!original_has_string_destinations) {
				original_string_destinations_ptr = CreateNameTreeDestinations(original_name_dictionary);
				assert(!original_string_destinations_ptr.empty() && "CreateNameTreeDestinations returned empty result");
			}

			auto original_string_destinations = original_string_destinations_ptr.GetValue();
			auto other_string_destination = other_string_destinations_ptr.GetValue();

			for (auto destination : *other_string_destination) {
				auto destination_key = destination.first;
				auto destination_value = destination.second;

				// Check for name conflicts
				assert(!original_string_destinations->Contains(destination_key) && "Name conflict");
				if (original_string_destinations->Contains(destination_key)) {
					throw NotSupportedException("Merge of conflicting names is not yet supported");
				}

				// Derefence destination in case it is indirect reference
				if (ObjectUtils::IsType<IndirectObjectReferencePtr>(destination_value)) {
					auto destination_reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(destination_value);
					destination_value = destination_reference->GetReferencedObjectAs<ContainableObjectPtr>();
				}

				// Create only shallow copies
				auto cloned_destination_obj = m_holder->ShallowCopyObject(destination_value);
				auto cloned_destination_unique_ptr = DestinationBase::Create(cloned_destination_obj);
				auto cloned_destination_raw_ptr = cloned_destination_unique_ptr.release();
				auto cloned_destination = DestinationPtr(cloned_destination_raw_ptr);
				auto cloned_destination_page = cloned_destination->GetPage();

				FixDestinationPage(other, cloned_destination_page);

				original_string_destinations->Insert(destination_key, cloned_destination);
			}
		}
	}
}

} // semantics
} // gotchangpdf
