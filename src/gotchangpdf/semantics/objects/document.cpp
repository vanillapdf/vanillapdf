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
	SemanticUtils::ReleaseMapping(m_holder);
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

	if (!dictionary->Contains(constant::Name::Info)) {
		return false;
	}

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
		for (auto instruction : page_contents->Instructions()) {
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

OutputNamedDestinationsPtr Document::CreateNameDestinations(CatalogPtr catalog) {
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

OutputNameTreePtr<DestinationPtr> Document::CreateStringDestinations(NameDictionaryPtr dictionary) {
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

void Document::FixDestinationPage(DestinationPtr other_destination, ObjectPtr cloned_page, types::uinteger merged_pages_count) {
	auto other_destination_obj = other_destination->GetObject();
	auto other_weak_file = other_destination_obj->GetFile();

	// Other file should not be 
	if (other_weak_file.IsActive() && other_weak_file.IsEmpty()) {
		auto other_file = other_weak_file.GetReference();

		// We should not fix page from the source document
		// Probably just return, instead of searching for references?
		assert(other_file != m_holder);
	}

	assert(ObjectUtils::IsType<IndirectObjectReferencePtr>(cloned_page)
		|| ObjectUtils::IsType<IntegerObjectPtr>(cloned_page));
	if (!ObjectUtils::IsType<IndirectObjectReferencePtr>(cloned_page)
		&& !ObjectUtils::IsType<IntegerObjectPtr>(cloned_page)) {
		throw GeneralException("Unknown object type");
	}

	auto original_catalog = GetDocumentCatalog();
	auto original_pages = original_catalog->Pages();
	auto original_page_count_after_merge = original_pages->PageCount();

	// This function is called after document pages have been merged
	auto original_page_count = original_page_count_after_merge - merged_pages_count;

	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(cloned_page)) {
		auto cloned_page_reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(cloned_page);
		auto other_page_reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(other_destination_obj);

		auto other_page_dictionary = other_page_reference->GetReferencedObjectAs<syntax::DictionaryObjectPtr>();
		auto other_page_node = PageNodeBase::CreatePageNode(other_page_dictionary);
		auto other_root_node = other_page_node->GetPageRoot();
		auto other_root_node_obj = other_root_node->GetObject();
		auto other_pages = PageTreePtr(other_root_node_obj);

		// Find matching object for the other file and fix
		bool found = false;
		for (decltype(merged_pages_count) k = 0; k < merged_pages_count; ++k) {
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

	if (ObjectUtils::IsType<IntegerObjectPtr>(cloned_page)) {
		// Increment referring page number by number of pages from original document
		auto cloned_page_index = ObjectUtils::ConvertTo<IntegerObjectPtr>(cloned_page);
		cloned_page_index->SetValue(cloned_page_index->GetIntegerValue() + original_page_count);
	}
}

void Document::AppendDocument(DocumentPtr other) {
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

	OutputNamedDestinationsPtr other_destinations_ptr;
	OutputNameDictionaryPtr other_name_dictionary_ptr;

	bool has_other_destinations = other_catalog->Destinations(other_destinations_ptr);
	bool has_other_name_dictionary = other_catalog->Names(other_name_dictionary_ptr);

	if (has_other_destinations) {
		NamedDestinationsPtr other_destinations = other_destinations_ptr.GetValue();
		MergeNameDestinations(other_destinations, other_page_count);
	}

	if (has_other_name_dictionary) {
		NameDictionaryPtr other_name_dictionary = other_name_dictionary_ptr.GetValue();

		OutputNameTreePtr<DestinationPtr> other_string_destinations_ptr;
		bool other_has_string_destinations = other_name_dictionary->Dests(other_string_destinations_ptr);
		if (other_has_string_destinations) {
			auto other_string_destinations = other_string_destinations_ptr.GetValue();
			MergeStringDestinations(other_string_destinations, other_page_count);
		}
	}
}

void Document::MergeNameDestinations(NamedDestinationsPtr destinations, types::uinteger merged_pages_count) {
	for (auto destination : destinations) {
		auto destination_name = destination.first;
		auto destination_value = destination.second;

		AppendNameDestination(destination_name, destination_value, merged_pages_count);
	}
}

void Document::MergeStringDestinations(NameTreePtr<DestinationPtr> destinations, types::uinteger merged_pages_count) {
	for (auto destination : destinations) {
		auto destination_key = destination.first;
		auto destination_value = destination.second;

		AppendStringDestination(destination_key, destination_value, merged_pages_count);
	}
}

void Document::AppendStringDestination(StringObjectPtr key, DestinationPtr value, types::uinteger merged_pages_count) {
	auto original_catalog = GetDocumentCatalog();

	OutputNameDictionaryPtr original_name_dictionary_ptr;
	bool has_original_name_dictionary = original_catalog->Names(original_name_dictionary_ptr);

	if (!has_original_name_dictionary) {
		original_name_dictionary_ptr = CreateNameDictionary(original_catalog);
		assert(!original_name_dictionary_ptr.empty() && "CreateNameDictionary returned empty result");
	}

	NameDictionaryPtr original_name_dictionary = original_name_dictionary_ptr.GetValue();

	OutputNameTreePtr<DestinationPtr> original_string_destinations_ptr;
	bool original_has_string_destinations = original_name_dictionary->Dests(original_string_destinations_ptr);
	if (!original_has_string_destinations) {
		original_string_destinations_ptr = CreateStringDestinations(original_name_dictionary);
		assert(!original_string_destinations_ptr.empty() && "CreateNameTreeDestinations returned empty result");
	}

	// Check for name conflicts
	auto original_string_destinations = original_string_destinations_ptr.GetValue();
	assert(!original_string_destinations->Contains(key) && "Name conflict");
	if (original_string_destinations->Contains(key)) {
		throw NotSupportedException("Merge of conflicting names is not yet supported");
	}

	// Derefence destination in case it is indirect reference
	auto value_object = value->GetObject();
	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(value_object)) {
		auto destination_reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(value_object);
		value_object = destination_reference->GetReferencedObjectAs<ContainableObjectPtr>();
	}

	// Create only shallow copies
	auto cloned_destination_obj = m_holder->ShallowCopyObject(value_object);
	auto cloned_destination_unique_ptr = DestinationBase::Create(cloned_destination_obj);
	auto cloned_destination_raw_ptr = cloned_destination_unique_ptr.release();
	auto cloned_destination = DestinationPtr(cloned_destination_raw_ptr);
	auto cloned_destination_page = cloned_destination->GetPage();

	FixDestinationPage(value, cloned_destination_page, merged_pages_count);

	original_string_destinations->Insert(key, cloned_destination);
}

void Document::AppendNameDestination(NameObjectPtr key, DestinationPtr value, types::uinteger merged_pages_count) {
	auto original_catalog = GetDocumentCatalog();

	OutputNamedDestinationsPtr original_destinations_ptr;
	bool has_original_destinations = original_catalog->Destinations(original_destinations_ptr);

	// Create missing destination objects
	if (!has_original_destinations) {
		original_destinations_ptr = CreateNameDestinations(original_catalog);
		assert(!original_destinations_ptr.empty() && "CreateNamedDestinations returned empty result");
	}

	// Check for name conflicts
	NamedDestinationsPtr original_destinations = original_destinations_ptr.GetValue();
	assert(!original_destinations->Contains(key) && "Name conflict");
	if (original_destinations->Contains(key)) {
		throw NotSupportedException("Merge of conflicting names is not yet supported");
	}

	// Derefence destination in case it is indirect reference
	auto value_object = value->GetObject();
	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(value_object)) {
		auto destination_reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(value_object);
		value_object = destination_reference->GetReferencedObjectAs<ContainableObjectPtr>();
	}

	// Create only shallow copies
	auto cloned_destination_obj = m_holder->ShallowCopyObject(value_object);
	auto cloned_destination_unique_ptr = DestinationBase::Create(cloned_destination_obj);
	auto cloned_destination_raw_ptr = cloned_destination_unique_ptr.release();
	auto cloned_destination = DestinationPtr(cloned_destination_raw_ptr);
	auto cloned_destination_page = cloned_destination->GetPage();

	FixDestinationPage(value, cloned_destination_page, merged_pages_count);

	original_destinations->Insert(key, cloned_destination);
}

void Document::AppendPage(PageObjectPtr page) {

}

void Document::Sign() {
	// Create new signature dictionary
	DictionaryObjectPtr signature_dictionary;
	signature_dictionary->Insert(constant::Name::Type, NameObjectPtr("Sig"));
	signature_dictionary->Insert(constant::Name::Cert, HexadecimalStringObjectPtr("\x1\x0"));
	signature_dictionary->Insert(constant::Name::Name, LiteralStringObjectPtr("Jurko"));
	signature_dictionary->Insert(constant::Name::Location, LiteralStringObjectPtr("Here"));
	signature_dictionary->Insert(constant::Name::Reason, LiteralStringObjectPtr("I agree"));

	// Leave byte ranges empty for now
	ArrayObjectPtr<IntegerObjectPtr> byte_ranges;
	byte_ranges->Append(IntegerObjectPtr());
	byte_ranges->Append(IntegerObjectPtr());

	signature_dictionary->Insert(constant::Name::ByteRange, byte_ranges);

	// Create new signature field
	DictionaryObjectPtr signature_field;
	signature_field->Insert(constant::Name::FT, NameObjectPtr("Sig"));
	signature_field->Insert(constant::Name::V, signature_dictionary);

	auto chain = m_holder->GetXrefChain();
	auto new_entry = chain->AllocateNewEntry();
	new_entry->SetReference(signature_field);

	auto catalog = GetDocumentCatalog();

	OuputInteractiveFormPtr interactive_form_ptr;
	bool has_interactive_form = catalog->AcroForm(interactive_form_ptr);
	if (!has_interactive_form) {
		assert(!"TODO");
	}

	auto interactive_form = interactive_form_ptr.GetValue();
	auto fields = interactive_form->Fields();
	auto fields_obj = fields->GetObject();

	fields_obj->Append(IndirectObjectReferencePtr(signature_field));
}

} // semantics
} // gotchangpdf
