#include "precompiled.h"

#include "syntax/files/file_writer.h"
#include "syntax/utils/serialization_override_attribute.h"

#include "semantics/objects/document.h"
#include "semantics/objects/contents.h"
#include "semantics/objects/annotations.h"
#include "semantics/objects/name_dictionary.h"

#include "semantics/utils/semantic_utils.h"
#include "semantics/utils/document_signer.h"

namespace gotchangpdf {
namespace semantics {

using namespace syntax;

DocumentPtr Document::Open(const std::string& path) {
	FilePtr file = File::Open(path);
	file->Initialize();

	return DocumentPtr(pdf_new Document(file));
}

DocumentPtr Document::Create(const std::string& path) {
	FilePtr file = File::Create(path);

	HeaderPtr header = file->GetHeader();
	header->SetVersion(Version::PDF17);

	XrefFreeEntryPtr initial_entry = XrefFreeEntry::Create(0, constant::MAX_GENERATION_NUMBER);

	XrefTablePtr xref_table;
	xref_table->Add(initial_entry);

	XrefChainPtr chain = file->GetXrefChain();
	chain->Append(xref_table);

	DocumentPtr document = DocumentPtr(pdf_new Document(file));
	document->CreateCatalog();

	DocumentInfoPtr document_info = document->CreateDocumentInfo();

	LiteralStringObjectPtr producer = make_deferred<LiteralStringObject>("I am the producer");
	document_info->SetProducer(producer);

	DatePtr creation_date = Date::GetCurrentDate();
	document_info->SetCreationDate(creation_date);

	return document;
}

DocumentPtr Document::OpenFile(syntax::FilePtr holder) {
	return DocumentPtr(pdf_new Document(holder));
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

bool Document::GetDocumentCatalog(OutputCatalogPtr& result) const {
	auto chain = m_holder->GetXrefChain();
	if (chain->Empty()) {
		return false;
	}

	auto xref = chain->Begin()->Value();
	auto dictionary = xref->GetTrailerDictionary();

	if (!dictionary->Contains(constant::Name::Root)) {
		return false;
	}

	auto root = dictionary->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Root);
	result = make_deferred<Catalog>(root);
	return true;
}

bool Document::GetDocumentInfo(OutputDocumentInfoPtr& result) const {
	auto chain = m_holder->GetXrefChain();
	auto xref = chain->Begin()->Value();
	auto dictionary = xref->GetTrailerDictionary();

	if (!dictionary->Contains(constant::Name::Info)) {
		return false;
	}

	auto info = dictionary->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Info);
	result = make_deferred<DocumentInfo>(info);
	return true;
}

void Document::RecalculatePageContents() {

	// On before Save

	OutputCatalogPtr catalog_ptr;
	bool has_catalog = GetDocumentCatalog(catalog_ptr);
	if (!has_catalog) {
		return;
	}

	OutputPageTreePtr pages;
	bool has_pages = catalog_ptr->Pages(pages);
	if (!has_pages) {
		return;
	}

	auto page_count = pages->PageCount();
	for (decltype(page_count) i = 1; i < page_count + 1; ++i) {
		auto page = pages->Page(i);

		OutputContentsPtr page_contents;
		bool has_contents = page->GetContents(page_contents);
		if (!has_contents) {
			continue;
		}

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
		BufferPtr new_body = make_deferred<Buffer>(string_body.begin(), string_body.end());
		stream_object->SetBody(new_body);
	}
}

void Document::Save(const std::string& path) {
	RecalculatePageContents();

	FilePtr destination = File::Create(path);

	FileWriter writer;
	writer.Write(m_holder, destination);
}

void Document::SaveIncremental(const std::string& path) {
	RecalculatePageContents();

	FilePtr destination = File::Create(path);

	FileWriter writer;
	writer.WriteIncremental(m_holder, destination);
}

CatalogPtr Document::CreateCatalog() {
	auto chain = m_holder->GetXrefChain();
	auto entry = chain->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;

	NameObjectPtr catalog = make_deferred<NameObject>(constant::Name::Catalog);
	raw_dictionary->Insert(constant::Name::Type, catalog);

	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto xref = chain->Begin()->Value();
	auto trailer_dictionary = xref->GetTrailerDictionary();

	IndirectObjectReferencePtr ref = make_deferred<IndirectObjectReference>(raw_dictionary);
	trailer_dictionary->Insert(constant::Name::Root, ref);

	return make_deferred<Catalog>(raw_dictionary);
}

DocumentInfoPtr Document::CreateDocumentInfo() {
	auto chain = m_holder->GetXrefChain();
	auto entry = chain->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto xref = chain->Begin()->Value();
	auto trailer_dictionary = xref->GetTrailerDictionary();

	IndirectObjectReferencePtr ref = make_deferred<IndirectObjectReference>(raw_dictionary);
	trailer_dictionary->Insert(constant::Name::Info, ref);

	return make_deferred<DocumentInfo>(raw_dictionary);
}

PageTreePtr Document::CreatePageTree(CatalogPtr catalog) {
	auto chain = m_holder->GetXrefChain();
	auto entry = chain->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto raw_catalog = catalog->GetObject();

	IndirectObjectReferencePtr ref = make_deferred<IndirectObjectReference>(raw_dictionary);
	raw_catalog->Insert(constant::Name::Pages, ref);

	return make_deferred<PageTree>(raw_dictionary);
}

NamedDestinationsPtr Document::CreateNameDestinations(CatalogPtr catalog) {
	auto chain = m_holder->GetXrefChain();
	auto entry = chain->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto raw_catalog = catalog->GetObject();

	IndirectObjectReferencePtr ref = make_deferred<IndirectObjectReference>(raw_dictionary);
	raw_catalog->Insert(constant::Name::Dests, ref);

	return make_deferred<NamedDestinations>(raw_dictionary);
}

NameDictionaryPtr Document::CreateNameDictionary(CatalogPtr catalog) {
	auto chain = m_holder->GetXrefChain();
	auto entry = chain->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto raw_catalog = catalog->GetObject();

	IndirectObjectReferencePtr ref = make_deferred<IndirectObjectReference>(raw_dictionary);
	raw_catalog->Insert(constant::Name::Names, ref);

	return make_deferred<NameDictionary>(raw_dictionary);
}

InteractiveFormPtr Document::CreateAcroForm(CatalogPtr catalog) {
	auto chain = m_holder->GetXrefChain();
	auto entry = chain->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto raw_catalog = catalog->GetObject();

	IndirectObjectReferencePtr ref = make_deferred<IndirectObjectReference>(raw_dictionary);
	raw_catalog->Insert(constant::Name::AcroForm, ref);

	return make_deferred<InteractiveForm>(raw_dictionary);
}

NameTreePtr<DestinationPtr> Document::CreateStringDestinations(NameDictionaryPtr dictionary) {
	auto chain = m_holder->GetXrefChain();
	auto entry = chain->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto name_dictionary = dictionary->GetObject();

	IndirectObjectReferencePtr ref = make_deferred<IndirectObjectReference>(raw_dictionary);
	name_dictionary->Insert(constant::Name::Dests, ref);

	OutputNameTreePtr<DestinationPtr> result;
	auto found_destinations = dictionary->Dests(result);
	assert(found_destinations); UNUSED(found_destinations);

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

	OutputCatalogPtr original_catalog;
	bool has_original_catalog = GetDocumentCatalog(original_catalog);
	if (!has_original_catalog) {
		assert(!"TODO");
	}

	OutputPageTreePtr original_pages;
	bool has_original_pages = original_catalog->Pages(original_pages);
	if (!has_original_pages) {
		assert(!"TODO");
	}

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

	OutputCatalogPtr other_catalog;
	bool has_other_catalog = other->GetDocumentCatalog(other_catalog);
	if (has_other_catalog) {
		return;
	}

	OutputPageTreePtr other_pages;
	bool has_other_pages = other_catalog->Pages(other_pages);
	if (!has_other_pages) {
		return;
	}

	auto other_page_count = other_pages->PageCount();

	for (decltype(other_page_count) i = 0; i < other_page_count; ++i) {
		auto other_page = other_pages->Page(i + 1);

		// Append new page
		AppendPage(other, other_page);
	}
}

bool Document::IsDestinationReferencingPage(DestinationPtr destination, PageObjectPtr page) {
	auto destination_page_object = destination->GetPage();
	auto other_page_dictionary = page->GetObject();

	assert(ObjectUtils::IsType<IndirectObjectReferencePtr>(destination_page_object)
		|| ObjectUtils::IsType<IntegerObjectPtr>(destination_page_object));
	if (!ObjectUtils::IsType<IndirectObjectReferencePtr>(destination_page_object)
		&& !ObjectUtils::IsType<IntegerObjectPtr>(destination_page_object)) {
		throw GeneralException("Unknown object type");
	}

	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(destination_page_object)) {
		auto cloned_page_reference = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(destination_page_object);
		auto destination_page_dictionary = cloned_page_reference->GetReferencedObjectAs<DictionaryObjectPtr>();

		if (!destination_page_dictionary->Identity(other_page_dictionary)) {
			return false;
		}

		return true;
	}

	if (ObjectUtils::IsType<IntegerObjectPtr>(destination_page_object)) {
		auto cloned_page_index = ObjectUtils::ConvertTo<IntegerObjectPtr>(destination_page_object);

		auto other_root_node = page->GetPageRoot();
		auto other_root_node_obj = other_root_node->GetObject();
		auto other_pages = make_deferred<PageTree>(other_root_node_obj);

		auto index_converted = cloned_page_index->SafeConvert<types::size_type>();
		auto check_page = other_pages->Page(index_converted);
		auto check_page_object = check_page->GetObject();

		if (!check_page_object->Identity(other_page_dictionary)) {
			return false;
		}

		return true;
	}

	// We should not reach this point
	// In case the object is neither reference nor integer
	// we close with an exception. The other cases are
	// treate separately.
	assert(!"I believe this code should not happen");
	return false;
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
	OutputCatalogPtr original_catalog;
	bool has_original_catalog = GetDocumentCatalog(original_catalog);
	if (!has_original_catalog) {
		original_catalog = CreateCatalog();
		assert(!original_catalog.empty() && "CreateCatalog returned empty result");
	}

	OutputNameDictionaryPtr original_name_dictionary;
	bool has_original_name_dictionary = original_catalog->Names(original_name_dictionary);

	if (!has_original_name_dictionary) {
		original_name_dictionary = CreateNameDictionary(original_catalog);
		assert(!original_name_dictionary.empty() && "CreateNameDictionary returned empty result");
	}

	OutputNameTreePtr<DestinationPtr> original_string_destinations;
	bool original_has_string_destinations = original_name_dictionary->Dests(original_string_destinations);
	if (!original_has_string_destinations) {
		original_string_destinations = CreateStringDestinations(original_name_dictionary);
		assert(!original_string_destinations.empty() && "CreateNameTreeDestinations returned empty result");
	}

	// Check for name conflicts
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
	OutputCatalogPtr original_catalog;
	bool has_catalog = GetDocumentCatalog(original_catalog);
	if (!has_catalog) {
		original_catalog = CreateCatalog();
		assert(!original_catalog.empty() && "CreateCatalog returned empty result");
	}

	OutputNamedDestinationsPtr original_destinations;
	bool has_original_destinations = original_catalog->Destinations(original_destinations);

	// Create missing destination objects
	if (!has_original_destinations) {
		original_destinations = CreateNameDestinations(original_catalog);
		assert(!original_destinations.empty() && "CreateNamedDestinations returned empty result");
	}

	// Check for name conflicts
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
	OutputCatalogPtr original_catalog;
	bool has_catalog = GetDocumentCatalog(original_catalog);
	if (!has_catalog) {
		original_catalog = CreateCatalog();
		assert(!original_catalog.empty() && "CreateCatalog returned empty result");
	}

	OutputPageTreePtr original_pages;
	bool has_pages = original_catalog->Pages(original_pages);
	if (!has_pages) {
		original_pages = CreatePageTree(original_catalog);
		assert(!original_pages.empty() && "CreatePageTree returned empty result");
	}

	auto page_object = other_page->GetObject();

	// Optimization
	// The page object contains the reference to his parent - page tree
	// By calling deep copy we copy all pages, becauce of the parent reference
	// The trick is to remove parent before deep copy
	if (page_object->Contains(constant::Name::Parent)) {
		bool removed = page_object->Remove(constant::Name::Parent);
		assert(removed && "Could not remove parent"); UNUSED(removed);
	}

	auto new_objects = m_holder->DeepCopyObject(page_object);
	auto new_page_object = new_objects[0];

	assert(ObjectUtils::IsType<DictionaryObjectPtr>(new_page_object) && "Page object is not dictionary");
	if (!ObjectUtils::IsType<DictionaryObjectPtr>(new_page_object)) {
		throw GeneralException("Cloned page object is not dictionary");
	}

	DictionaryObjectPtr new_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(new_page_object);
	PageObjectPtr new_page = make_deferred<PageObject>(new_dictionary);

	// Insert into our page tree
	// The parent we removed in optimization will be set inside
	original_pages->Append(new_page);

	MergePageDestinations(other, other_page, new_page);
}

void Document::MergePageDestinations(DocumentPtr other, PageObjectPtr other_page, PageObjectPtr merged_page) {
	OutputCatalogPtr other_catalog;
	bool has_other_catalog = other->GetDocumentCatalog(other_catalog);
	if (!has_other_catalog) {
		return;
	}

	OutputNamedDestinationsPtr other_destinations;
	OutputNameDictionaryPtr other_name_dictionary;

	bool has_other_destinations = other_catalog->Destinations(other_destinations);
	bool has_other_name_dictionary = other_catalog->Names(other_name_dictionary);

	if (has_other_destinations) {
		MergeNameDestinations(other_destinations, other_page, merged_page);
	}

	if (has_other_name_dictionary) {
		OutputNameTreePtr<DestinationPtr> other_string_destinations;
		bool other_has_string_destinations = other_name_dictionary->Dests(other_string_destinations);
		if (other_has_string_destinations) {
			MergeStringDestinations(other_string_destinations, other_page, merged_page);
		}
	}
}

void Document::Sign(const std::string& path, DocumentSignatureSettingsPtr options) {

	OutputPointer<ISigningKeyPtr> key;
	OutputPointer<syntax::HexadecimalStringObjectPtr> certificate;
	OutputPointer<syntax::LiteralStringObjectPtr> name;
	OutputPointer<syntax::LiteralStringObjectPtr> location;
	OutputPointer<syntax::LiteralStringObjectPtr> reason;

	bool has_key = options->GetSigningKey(key);
	bool has_certificate = options->GetCertificate(certificate);
	bool has_name = options->GetName(name);
	bool has_location = options->GetLocation(location);
	bool has_reason = options->GetReason(reason);

	auto digest = options->GetDigest();

	if (!has_key) {
		throw GeneralException("Signing key is not set");
	}

	// Create new signature dictionary
	DictionaryObjectPtr signature_dictionary;
	signature_dictionary->Insert(constant::Name::Type, make_deferred<NameObject>("Sig"));

	if (has_certificate) {
		signature_dictionary->Insert(constant::Name::Cert, *certificate);
	}

	if (has_name) {
		signature_dictionary->Insert(constant::Name::Name, *name);
	}

	if (has_location) {
		signature_dictionary->Insert(constant::Name::Location, *location);
	}

	if (has_reason) {
		signature_dictionary->Insert(constant::Name::Reason, *reason);
	}

	// TODO hardcoded value
	std::string byte_range_value(20, ' ');
	SerializationOverrideAttributePtr byte_range_attribute = make_deferred<SerializationOverrideAttribute>(byte_range_value);

	// Leave byte ranges empty for now
	ArrayObjectPtr<IntegerObjectPtr> byte_ranges;
	byte_ranges->AddAttribute(byte_range_attribute);

	// TODO hardcoded value
	std::string contents_value(1024, '0');
	SerializationOverrideAttributePtr contents_attribute = make_deferred<SerializationOverrideAttribute>(contents_value);
	HexadecimalStringObjectPtr signature_contents = make_deferred<HexadecimalStringObject>();
	signature_contents->AddAttribute(contents_attribute);

	signature_dictionary->Insert(constant::Name::ByteRange, byte_ranges);
	signature_dictionary->Insert(constant::Name::Contents, signature_contents);

	// Create new signature field
	DictionaryObjectPtr signature_field;
	signature_field->Insert(constant::Name::FT, make_deferred<NameObject>("Sig"));
	signature_field->Insert(constant::Name::V, signature_dictionary);

	auto chain = m_holder->GetXrefChain();
	auto new_entry = chain->AllocateNewEntry();
	new_entry->SetReference(signature_field);

	OutputCatalogPtr catalog;
	bool has_catalog = GetDocumentCatalog(catalog);
	if (!has_catalog) {
		catalog = CreateCatalog();
		assert(!catalog.empty() && "CreateCatalog returned empty result");
	}

	OuputInteractiveFormPtr interactive_form;
	bool has_interactive_form = catalog->AcroForm(interactive_form);
	if (!has_interactive_form) {
		interactive_form = CreateAcroForm(catalog);
		assert(!interactive_form.empty() && "CreateAcroForm returned empty result");
	}

	auto fields = interactive_form->Fields();
	auto fields_obj = fields->GetObject();

	fields_obj->Append(signature_field);

	DocumentSignerPtr signer = make_deferred<DocumentSigner>(key, digest, signature_dictionary);
	FilePtr destination = File::Create(path);

	FileWriter writer;
	writer.Subscribe(signer);
	writer.WriteIncremental(m_holder, destination);
}

} // semantics
} // gotchangpdf
