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

void Document::FixDestinationPage(ObjectPtr cloned_page, PageObjectPtr other_page, PageObjectPtr merged_page) {
	auto other_destination_obj = other_page->GetObject();
	auto other_weak_file = other_destination_obj->GetFile();

	// Other file should not be 
	if (other_weak_file.IsActive() && other_weak_file.IsEmpty()) {
		auto other_file = other_weak_file.GetReference();

		// We should not fix page from the source document
		// Probably just return, instead of searching for references?
		assert(other_file.get() != m_holder.get());
	}

	assert(ObjectUtils::IsType<IndirectObjectReferencePtr>(cloned_page)
		|| ObjectUtils::IsType<IntegerObjectPtr>(cloned_page));
	if (!ObjectUtils::IsType<IndirectObjectReferencePtr>(cloned_page)
		&& !ObjectUtils::IsType<IntegerObjectPtr>(cloned_page)) {
		throw GeneralException("Unknown object type");
	}

	auto original_catalog = GetDocumentCatalog();
	auto original_pages = original_catalog->Pages();
	auto original_page_count = original_pages->PageCount();

	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(cloned_page)) {
		auto cloned_page_reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(cloned_page);

		auto merged_page_object = merged_page->GetObject();
		cloned_page_reference->SetReferencedObject(merged_page_object);
	}

	if (ObjectUtils::IsType<IntegerObjectPtr>(cloned_page)) {
		auto cloned_page_index = ObjectUtils::ConvertTo<IntegerObjectPtr>(cloned_page);
		auto merged_page_object = merged_page->GetObject();

		// Find matching object for the other file and fix
		for (decltype(original_page_count) k = 0; k < original_page_count; ++k) {
			auto orignal_page = original_pages->Page(k + 1);
			auto orignal_page_obj = original_pages->GetObject();

			// Search for merged page index
			if (orignal_page_obj->GetObjectNumber() != merged_page_object->GetObjectNumber()
				|| orignal_page_obj->GetGenerationNumber() != merged_page_object->GetGenerationNumber()) {
				continue;
			}

			// Update the destination reference
			cloned_page_index->SetValue(k + 1);
		}
	}
}

void Document::AppendDocument(DocumentPtr other) {
	auto other_catalog = other->GetDocumentCatalog();
	auto other_pages = other_catalog->Pages();
	auto other_page_count = other_pages->PageCount();

	std::vector<ObjectPtr> merge_objects;
	for (decltype(other_page_count) i = 0; i < other_page_count; ++i) {
		auto other_page = other_pages->Page(i + 1);

		// Append new page
		AppendPage(other, other_page);
	}
}

bool Document::IsDestinationReferencingPage(DestinationPtr destination, PageObjectPtr page) {
	auto destionation_page_object = destination->GetPage();
	auto other_page_dictionary = page->GetObject();

	assert(ObjectUtils::IsType<IndirectObjectReferencePtr>(destionation_page_object)
		|| ObjectUtils::IsType<IntegerObjectPtr>(destionation_page_object));
	if (!ObjectUtils::IsType<IndirectObjectReferencePtr>(destionation_page_object)
		&& !ObjectUtils::IsType<IntegerObjectPtr>(destionation_page_object)) {
		throw GeneralException("Unknown object type");
	}

	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(destionation_page_object)) {
		auto cloned_page_reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(destionation_page_object);
		auto destination_page_dictionary = cloned_page_reference->GetReferencedObjectAs<DictionaryObjectPtr>();

		if (!destination_page_dictionary->Identity(other_page_dictionary)) {
			return false;
		}

		return true;
	}

	if (ObjectUtils::IsType<IntegerObjectPtr>(destionation_page_object)) {
		auto cloned_page_index = ObjectUtils::ConvertTo<IntegerObjectPtr>(destionation_page_object);

		auto other_root_node = page->GetPageRoot();
		auto other_root_node_obj = other_root_node->GetObject();
		auto other_pages = PageTreePtr(other_root_node_obj);

		auto index_converted = cloned_page_index->SafeConvert<types::integer>();
		auto check_page = other_pages->Page(index_converted);
		auto check_page_object = check_page->GetObject();

		if (!check_page_object->Identity(other_page_dictionary)) {
			return false;
		}

		return true;
	}
}

void Document::MergeNameDestinations(NamedDestinationsPtr destinations, PageObjectPtr other_page, PageObjectPtr merged_page) {
	for (auto destination : destinations) {
		auto destination_name = destination.first;
		auto destination_value = destination.second;

		if (!IsDestinationReferencingPage(destination_value, other_page)) {
			continue;
		}

		AppendNameDestination(destination_name, destination_value, other_page, merged_page);
	}
}

void Document::MergeStringDestinations(NameTreePtr<DestinationPtr> destinations, PageObjectPtr other_page, PageObjectPtr merged_page) {
	for (auto destination : destinations) {
		auto destination_key = destination.first;
		auto destination_value = destination.second;

		if (!IsDestinationReferencingPage(destination_value, other_page)) {
			continue;
		}

		AppendStringDestination(destination_key, destination_value, other_page, merged_page);
	}
}

void Document::AppendStringDestination(StringObjectPtr key, DestinationPtr value, PageObjectPtr other_page, PageObjectPtr merged_page) {
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

	FixDestinationPage(cloned_destination_page, other_page, merged_page);

	original_string_destinations->Insert(key, cloned_destination);
}

void Document::AppendNameDestination(NameObjectPtr key, DestinationPtr value, PageObjectPtr other_page, PageObjectPtr merged_page) {
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

	FixDestinationPage(cloned_destination_page, other_page, merged_page);

	original_destinations->Insert(key, cloned_destination);
}

void Document::AppendPage(DocumentPtr other, PageObjectPtr other_page) {
	auto original_catalog = GetDocumentCatalog();
	auto original_pages = original_catalog->Pages();

	auto page_object = other_page->GetObject();
	auto new_objects = m_holder->DeepCopyObject(page_object);

	auto new_page_object = new_objects[0];

	assert(ObjectUtils::IsType<DictionaryObjectPtr>(new_page_object) && "Page object is not dictionary");
	if (!ObjectUtils::IsType<DictionaryObjectPtr>(new_page_object)) {
		throw GeneralException("Cloned page object is not dictionary");
	}

	DictionaryObjectPtr new_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(new_page_object);
	PageObjectPtr new_page(new_dictionary);

	// Insert into our page tree
	original_pages->Append(new_page);

	MergePageDestinations(other, other_page, new_page);
}

void Document::MergePageDestinations(DocumentPtr other, PageObjectPtr other_page, PageObjectPtr merged_page) {
	auto other_catalog = other->GetDocumentCatalog();

	OutputNamedDestinationsPtr other_destinations_ptr;
	OutputNameDictionaryPtr other_name_dictionary_ptr;

	bool has_other_destinations = other_catalog->Destinations(other_destinations_ptr);
	bool has_other_name_dictionary = other_catalog->Names(other_name_dictionary_ptr);

	if (has_other_destinations) {
		NamedDestinationsPtr other_destinations = other_destinations_ptr.GetValue();
		MergeNameDestinations(other_destinations, other_page, merged_page);
	}

	if (has_other_name_dictionary) {
		NameDictionaryPtr other_name_dictionary = other_name_dictionary_ptr.GetValue();

		OutputNameTreePtr<DestinationPtr> other_string_destinations_ptr;
		bool other_has_string_destinations = other_name_dictionary->Dests(other_string_destinations_ptr);
		if (other_has_string_destinations) {
			NameTreePtr<DestinationPtr> other_string_destinations = other_string_destinations_ptr.GetValue();
			MergeStringDestinations(other_string_destinations, other_page, merged_page);
		}
	}
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
