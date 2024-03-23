#include "precompiled.h"

#include "utils/library_info.h"

#include "syntax/files/file_writer.h"

#include "syntax/utils/name_constants.h"
#include "syntax/utils/serialization_override_object_attribute.h"

#include "semantics/objects/document.h"
#include "semantics/objects/page_contents.h"
#include "semantics/objects/annotations.h"
#include "semantics/objects/name_dictionary.h"

#include "semantics/utils/semantic_utils.h"
#include "semantics/utils/document_signer.h"
#include "semantics/utils/document_encryption_settings.h"

#include <sstream>

namespace vanillapdf {
namespace semantics {

using namespace syntax;

DocumentPtr Document::Open(const std::string& path) {
	FilePtr file = File::Open(path);
	return OpenFile(file);
}

DocumentPtr Document::OpenFile(syntax::FilePtr holder) {
	if (SemanticUtils::HasMappedDocument(holder)) {
		auto result = SemanticUtils::GetMappedDocument(holder);

		return result.GetReference();
	}

	return DocumentPtr(pdf_new Document(holder));
}

DocumentPtr Document::Create(const std::string& path) {
	FilePtr file = File::Create(path);
	return CreateFile(file);
}

DocumentPtr Document::CreateFile(syntax::FilePtr holder) {

	if (SemanticUtils::HasMappedDocument(holder)) {
		auto log_scope = holder->GetFilenameString();

		std::stringstream error_stream;
		error_stream << "Trying to create new document for file ";
		error_stream << log_scope;
		error_stream << ", but the file instance was already opened";

		LOG_ERROR(log_scope) << error_stream.str();

		throw GeneralException(error_stream.str());
	}

	HeaderPtr header = holder->GetHeader();
	header->SetVersion(Version::PDF17);

	XrefFreeEntryPtr initial_entry = XrefFreeEntry::Create(0, constant::MAX_GENERATION_NUMBER);

	XrefTablePtr xref_table;
	xref_table->Add(initial_entry);

	// Generate a unique document ID
	//
	// To help ensure the uniqueness of file identifiers, they should be computed by means of a message digest algorithm
	// such as MD5 (described in Internet RFC 1321, The MD5 Message-Digest Algorithm; see the Bibliography),
	// using the following information:
	//   - The current time
	//   - A string representation of the file's location, usually a pathname
	//   - The size of the file in bytes
	//   - The values of all entries in the file's document information dictionary(see 14.3.3, "Document Information Dictionary")

	// We are going to use random data as for example we don't know the path, neither file size
	BufferPtr document_id_value = EncryptionUtils::GenerateRandomData(16);

	// An array of two byte-strings constituting a file identifier (see 14.4, "File Identifiers") for the file.
	// If there is an Encrypt entry this array and the two byte-strings shall be direct objects and shall be unencrypted.
	ArrayObjectPtr<HexadecimalStringObjectPtr> document_ids;
	document_ids->Append(HexadecimalStringObject::CreateFromDecoded(document_id_value));
	document_ids->Append(HexadecimalStringObject::CreateFromDecoded(document_id_value));

	// Insert the ID into the trailer dictionary
	auto trailer_dictionary = xref_table->GetTrailerDictionary();
	trailer_dictionary->Insert(constant::Name::ID, document_ids);

	XrefChainPtr chain = holder->GetXrefChain();
	chain->Append(xref_table);

	// Create an instace of the resulting document
	DocumentPtr document = DocumentPtr(pdf_new Document(holder));

	CatalogPtr catalog = document->CreateCatalog();
	catalog->CreatePages();

	DocumentInfoPtr document_info = document->CreateDocumentInfo();

	std::stringstream producer_string;
	producer_string << LibraryInfo::Author();
	producer_string << ' ';
	producer_string << LibraryInfo::MajorVersion();
	producer_string << '.';
	producer_string << LibraryInfo::MinorVersion();
	producer_string << '.';
	producer_string << LibraryInfo::PatchVersion();

	LiteralStringObjectPtr producer = LiteralStringObject::CreateFromDecoded(producer_string.str());
	document_info->SetProducer(producer);

	DatePtr creation_date = Date::GetCurrentDate();
	document_info->SetCreationDate(creation_date);

	return document;
}

Document::Document(syntax::FilePtr holder) : m_holder(holder) {
	SemanticUtils::AddDocumentMapping(m_holder, GetWeakReference<Document>());

	m_holder->Initialize();
}

syntax::FilePtr Document::GetFile() const {
	return m_holder;
}

Document::~Document() {
	SemanticUtils::ReleaseMapping(m_holder);
}

bool Document::GetDocumentCatalog(OutputCatalogPtr& result) const {
	if (!m_catalog.empty()) {
		result = m_catalog;
		return true;
	}

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
	m_catalog = make_deferred<Catalog>(root);
	result = m_catalog;
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

void Document::Save(const std::string& path) {
	FilePtr destination = File::Create(path);
	Save(destination);
}

void Document::Save(syntax::FilePtr destination) {
	FileWriter writer;
	writer.Write(m_holder, destination);
}

void Document::SaveIncremental(const std::string& path) {
	FilePtr destination = File::Create(path);
	SaveIncremental(destination);
}

void Document::SaveIncremental(syntax::FilePtr destination) {
	FileWriter writer;
	writer.WriteIncremental(m_holder, destination);
}

CatalogPtr Document::CreateCatalog() {
	auto chain = m_holder->GetXrefChain();
	auto entry = m_holder->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;

	NameObjectPtr catalog = make_deferred<NameObject>(constant::Name::Catalog);
	raw_dictionary->Insert(constant::Name::Type, catalog);

	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto xref = chain->Begin()->Value();
	auto trailer_dictionary = xref->GetTrailerDictionary();

	IndirectReferenceObjectPtr ref = make_deferred<IndirectReferenceObject>(raw_dictionary);
	trailer_dictionary->Insert(constant::Name::Root, ref);

	return make_deferred<Catalog>(raw_dictionary);
}

DocumentInfoPtr Document::CreateDocumentInfo() {
	auto chain = m_holder->GetXrefChain();
	auto entry = m_holder->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto xref = chain->Begin()->Value();
	auto trailer_dictionary = xref->GetTrailerDictionary();

	IndirectReferenceObjectPtr ref = make_deferred<IndirectReferenceObject>(raw_dictionary);
	trailer_dictionary->Insert(constant::Name::Info, ref);

	return make_deferred<DocumentInfo>(raw_dictionary);
}

PageTreePtr Document::CreatePageTree(CatalogPtr catalog) {
	auto entry = m_holder->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto raw_catalog = catalog->GetObject();

	IndirectReferenceObjectPtr ref = make_deferred<IndirectReferenceObject>(raw_dictionary);
	raw_catalog->Insert(constant::Name::Pages, ref);

	return make_deferred<PageTree>(raw_dictionary);
}

NamedDestinationsPtr Document::CreateNameDestinations(CatalogPtr catalog) {
	auto entry = m_holder->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto raw_catalog = catalog->GetObject();

	IndirectReferenceObjectPtr ref = make_deferred<IndirectReferenceObject>(raw_dictionary);
	raw_catalog->Insert(constant::Name::Dests, ref);

	return make_deferred_container<NamedDestinations>(raw_dictionary);
}

NameDictionaryPtr Document::CreateNameDictionary(CatalogPtr catalog) {
	auto entry = m_holder->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto raw_catalog = catalog->GetObject();

	IndirectReferenceObjectPtr ref = make_deferred<IndirectReferenceObject>(raw_dictionary);
	raw_catalog->Insert(constant::Name::Names, ref);

	return make_deferred<NameDictionary>(raw_dictionary);
}

InteractiveFormPtr Document::CreateAcroForm(CatalogPtr catalog) {
	auto entry = m_holder->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto raw_catalog = catalog->GetObject();

	IndirectReferenceObjectPtr ref = make_deferred<IndirectReferenceObject>(raw_dictionary);
	raw_catalog->Insert(constant::Name::AcroForm, ref);

	return make_deferred<InteractiveForm>(raw_dictionary);
}

NameTreePtr<DestinationPtr> Document::CreateStringDestinations(NameDictionaryPtr dictionary) {
	auto entry = m_holder->AllocateNewEntry();

	DictionaryObjectPtr raw_dictionary;
	raw_dictionary->SetFile(m_holder);
	raw_dictionary->SetInitialized();
	entry->SetReference(raw_dictionary);
	entry->SetInitialized();

	auto name_dictionary = dictionary->GetObject();

	IndirectReferenceObjectPtr ref = make_deferred<IndirectReferenceObject>(raw_dictionary);
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

	assert(ObjectUtils::IsType<IndirectReferenceObjectPtr>(cloned_page)
		|| ObjectUtils::IsType<IntegerObjectPtr>(cloned_page));
	if (!ObjectUtils::IsType<IndirectReferenceObjectPtr>(cloned_page)
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

	if (ObjectUtils::IsType<IndirectReferenceObjectPtr>(cloned_page)) {
		auto cloned_page_reference = ObjectUtils::ConvertTo<IndirectReferenceObjectPtr>(cloned_page);

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

	// All strings and streams needs to be initialized before cloning
	other->ForceObjectInitialization();

	OutputCatalogPtr other_catalog;
	bool has_other_catalog = other->GetDocumentCatalog(other_catalog);
	if (!has_other_catalog) {
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

	assert(ObjectUtils::IsType<IndirectReferenceObjectPtr>(destination_page_object)
		|| ObjectUtils::IsType<IntegerObjectPtr>(destination_page_object));
	if (!ObjectUtils::IsType<IndirectReferenceObjectPtr>(destination_page_object)
		&& !ObjectUtils::IsType<IntegerObjectPtr>(destination_page_object)) {
		throw GeneralException("Unknown object type");
	}

	if (ObjectUtils::IsType<IndirectReferenceObjectPtr>(destination_page_object)) {
		auto cloned_page_reference = ObjectUtils::ConvertTo<IndirectReferenceObjectPtr>(destination_page_object);
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
	if (ObjectUtils::IsType<IndirectReferenceObjectPtr>(value_object)) {
		auto destination_reference = ObjectUtils::ConvertTo<IndirectReferenceObjectPtr>(value_object);
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
	if (ObjectUtils::IsType<IndirectReferenceObjectPtr>(value_object)) {
		auto destination_reference = ObjectUtils::ConvertTo<IndirectReferenceObjectPtr>(value_object);
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

void Document::Sign(FilePtr destination, DocumentSignatureSettingsPtr options) {

	OutputPointer<ISigningKeyPtr> key;
	OutputPointer<syntax::HexadecimalStringObjectPtr> certificate;
	OutputPointer<syntax::LiteralStringObjectPtr> name;
	OutputPointer<syntax::LiteralStringObjectPtr> location;
	OutputPointer<syntax::LiteralStringObjectPtr> reason;
	OutputPointer<DatePtr> signing_time;

	bool has_key = options->GetSigningKey(key);
	bool has_time = options->GetSigningTime(signing_time);
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
	signature_dictionary->Insert(constant::Name::Type, NameObject::CreateFromDecoded("Sig"));

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

	if (has_time) {
		signature_dictionary->Insert(constant::Name::M, signing_time->GetObject());
	}

	// TODO
	signature_dictionary->Insert(constant::Name::Filter, constant::Name::AdobePPKLite.Clone());
	signature_dictionary->Insert(constant::Name::SubFilter, NameObject::CreateFromDecoded("adbe.pkcs7.detached"));

	// TODO hardcoded value
	std::string byte_range_value(100, ' ');
	SerializationOverrideAttributePtr byte_range_attribute = make_deferred<SerializationOverrideAttribute>(byte_range_value);

	// Leave byte ranges empty for now
	ArrayObjectPtr<IntegerObjectPtr> byte_ranges;
	byte_ranges->AddAttribute(byte_range_attribute);

	// TODO hardcoded value
	std::string contents_value(32768, ' ');
	SerializationOverrideAttributePtr contents_attribute = make_deferred<SerializationOverrideAttribute>(contents_value);
	HexadecimalStringObjectPtr signature_contents = make_deferred<HexadecimalStringObject>();
	signature_contents->AddAttribute(contents_attribute);

	signature_dictionary->Insert(constant::Name::ByteRange, byte_ranges);
	signature_dictionary->Insert(constant::Name::Contents, signature_contents);

	auto dictionary_entry = m_holder->AllocateNewEntry();
	dictionary_entry->SetReference(signature_dictionary);
	dictionary_entry->SetFile(m_holder);
	dictionary_entry->SetInitialized();

	// Create signature annotation
	OutputCatalogPtr catalog;
	bool has_catalog = GetDocumentCatalog(catalog);
	if (!has_catalog) {
		catalog = CreateCatalog();
		assert(!catalog.empty() && "CreateCatalog returned empty result");
	}

	OutputPageTreePtr page_tree;
	bool has_pages = catalog->Pages(page_tree);
	if (!has_pages) {
		throw GeneralException("Cannot sign document without pages");
	}

	auto page_count = page_tree->PageCount();
	if (page_count == 0) {
		throw GeneralException("Cannot sign document without pages");
	}

	auto first_page = page_tree->Page(1);
	auto first_page_object = first_page->GetObject();

	if (!first_page_object->Contains(constant::Name::Annots)) {
		MixedArrayObjectPtr first_page_annotations;
		first_page_object->Insert(constant::Name::Annots, first_page_annotations);
	}

	auto first_page_annotations = first_page_object->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Annots);

	MixedArrayObjectPtr annotation_rectangle;
	annotation_rectangle->Append(make_deferred<IntegerObject>(0));
	annotation_rectangle->Append(make_deferred<IntegerObject>(0));
	annotation_rectangle->Append(make_deferred<IntegerObject>(0));
	annotation_rectangle->Append(make_deferred<IntegerObject>(0));

	DictionaryObjectPtr signature_annotation;
	signature_annotation->Insert(constant::Name::Type, NameObject::CreateFromDecoded("Annot"));
	signature_annotation->Insert(constant::Name::Subtype, NameObject::CreateFromDecoded("Widget"));
	signature_annotation->Insert(constant::Name::Rect, annotation_rectangle);

	auto first_page_reference = make_deferred<syntax::IndirectReferenceObject>(first_page_object);
	signature_annotation->Insert(constant::Name::P, first_page_reference);

	auto annotation_entry = m_holder->AllocateNewEntry();
	annotation_entry->SetReference(signature_annotation);
	annotation_entry->SetFile(m_holder);
	annotation_entry->SetInitialized();

	auto signature_annotation_reference = make_deferred<syntax::IndirectReferenceObject>(signature_annotation);
	first_page_annotations->Append(signature_annotation_reference);

	// Create new signature field
	signature_annotation->Insert(constant::Name::FT, NameObject::CreateFromDecoded("Sig"));
	signature_annotation->Insert(constant::Name::T, LiteralStringObject::CreateFromDecoded("Signature1"));

	auto signature_dictionary_reference = make_deferred<syntax::IndirectReferenceObject>(signature_dictionary);
	signature_annotation->Insert(constant::Name::V, signature_dictionary_reference);

	OuputInteractiveFormPtr interactive_form;
	bool has_interactive_form = catalog->AcroForm(interactive_form);
	if (!has_interactive_form) {
		interactive_form = CreateAcroForm(catalog);
		assert(!interactive_form.empty() && "CreateAcroForm returned empty result");
	}

	// Update signature flags
	auto signature_flags = interactive_form->CreateSignatureFlags();
	signature_flags->SetSignaturesExist(true);
	signature_flags->SetAppendOnly(true);

	// Create signature field dictionary within AcroForm
	auto fields = interactive_form->CreateFields();
	auto fields_obj = fields->GetObject();
	auto fields_array = fields_obj->Data();

	auto signature_fields_reference = make_deferred<syntax::IndirectReferenceObject>(signature_annotation);
	fields_array->Append(signature_fields_reference);

	DocumentSignerPtr signer = make_deferred<DocumentSigner>(key, digest, signature_dictionary);

	FileWriter writer;
	writer.Subscribe(signer);
	writer.Write(m_holder, destination);
}

void Document::AddEncryption(DocumentEncryptionSettingsPtr settings) {

	// Terminate in case the document is already encrypted
	if (m_holder->IsEncrypted()) {
		throw GeneralException("Cannot encrypt an encrypted document, please remove the encryption first");
	}

	// Initialize all entries, to load them into cache
	// Once loaded, the objects won't ask for decryption when accessing from file save
	ForceObjectInitialization();

	// Find the proper xref section
	auto xref_chain = m_holder->GetXrefChain();
	auto xref_iterator = xref_chain->Begin();
	auto xref = xref_iterator->Value();

	// Find the trailer dictionary
	auto trailer_dictionary = xref->GetTrailerDictionary();

	if (!trailer_dictionary->Contains(constant::Name::ID)) {
		// TODO: Generate document ID
	}

	auto document_ids = trailer_dictionary->FindAs<MixedArrayObjectPtr>(constant::Name::ID);
	if (document_ids->GetSize() < 2) {
		// TODO: Generate document ID
	}

	// Identify the document ID object - it could be other types than string
	auto document_id_obj = document_ids[0];

	BufferPtr document_id_buffer;
	if (ObjectUtils::IsType<StringObjectPtr>(document_id_obj)) {
		auto document_id_string = ObjectUtils::ConvertTo<StringObjectPtr>(document_id_obj);
		document_id_buffer = document_id_string->GetValue();
	}

	if (document_id_buffer->empty()) {
		throw GeneralException("Could not encrypt document with empty document ID");
	}

	auto key_length = settings->GetKeyLength();

	auto permissions_flags = settings->GetUserPermissions();
	auto permission_value = DocumentEncryptionSettings::GetPermissionInteger(permissions_flags);

	// (PDF 1.4) "Algorithm 1: Encryption of data using the RC4 or AES algorithms"in 7.6.2,
	// "General Encryption Algorithm," but permitting encryption key lengths greater than 40 bits.
	int32_t algorithm_version = 2;

	// if the document is encrypted with a V value of 2 or 3,
	// or has any "Security handlers of revision 3 or greater" access permissions set to 0
	int32_t security_revision = 2;

	// Calculate the encryption entries
	auto owner_key_buffer = EncryptionUtils::GenerateOwnerEncryptionKey(
		document_id_buffer,
		settings->GetOwnerPassword(),
		settings->GetUserPassword(),
		settings->GetEncryptionAlgorithm(),
		key_length,
		permission_value,
		security_revision);

	auto user_key_buffer = EncryptionUtils::GenerateUserEncryptionKey(
		document_id_buffer,
		settings->GetUserPassword(),
		owner_key_buffer,
		settings->GetEncryptionAlgorithm(),
		key_length,
		permission_value,
		security_revision);

	// Create new encryption dictionary
	DictionaryObjectPtr encryption_dictionary;

	// Table 20 - Entries common to all encryption dictionaries
	encryption_dictionary->Insert(constant::Name::Filter, NameObject::CreateFromDecoded("Standard"));
	//encryption_dictionary->Insert(constant::Name::SubFilter, make_deferred<IntegerObject>(2));
	encryption_dictionary->Insert(constant::Name::V, make_deferred<IntegerObject>(algorithm_version));
	encryption_dictionary->Insert(constant::Name::Length, make_deferred<IntegerObject>(key_length));

	// Table 21 - Additional encryption dictionary entries for the standard security handler
	encryption_dictionary->Insert(constant::Name::R, make_deferred<IntegerObject>(security_revision));
	encryption_dictionary->Insert(constant::Name::O, HexadecimalStringObject::CreateFromDecoded(owner_key_buffer));
	encryption_dictionary->Insert(constant::Name::U, HexadecimalStringObject::CreateFromDecoded(user_key_buffer));
	encryption_dictionary->Insert(constant::Name::P, make_deferred<IntegerObject>(permission_value));
	//encryption_dictionary->Insert(constant::Name::EncryptMetadata, make_deferred<IntegerObject>(2));

	// Mark this entry as encryption exempted
	encryption_dictionary->SetEncryptionExempted();

	// Create new xref entry for the encryption dictionary
	auto encryption_entry = m_holder->AllocateNewEntry();
	encryption_entry->SetReference(encryption_dictionary);
	encryption_entry->SetFile(m_holder);
	encryption_entry->SetInitialized();

	// Insert indirect reference to the as the document encryption dictionary
	auto encryption_dictionary_reference = make_deferred<syntax::IndirectReferenceObject>(encryption_dictionary);

	// Add association to the trailer dictionary
	trailer_dictionary->Insert(constant::Name::Encrypt, encryption_dictionary_reference);

	// The encryption dictionary needs to be set explicitly as it is only initialized during the parse process
	m_holder->SetEncryptionDictionary(encryption_dictionary);

	// The document needs to have encryption/decryption key initialized
	bool password_set = m_holder->SetEncryptionPassword(settings->GetUserPassword());

	if (!password_set) {
		throw GeneralException("Could not verify the encryption password");
	}
}

void Document::RemoveEncryption() {

	// Terminate in case the document is not encrypted, treat as success
	if (!m_holder->IsEncrypted()) {
		return;
	}

	// TODO: Check if the password is owner only

	// Initialize all entries, to decrypt them forcefully
	ForceObjectInitialization();

	auto xref_chain = m_holder->GetXrefChain();
	auto xref_iterator = xref_chain->Begin();
	auto xref = xref_iterator->Value();

	// Remove the encryption dictionary
	auto trailer_dictionary = xref->GetTrailerDictionary();
	trailer_dictionary->Remove(constant::Name::Encrypt);
}

void Document::ForceObjectInitialization() {

	auto xref_chain = m_holder->GetXrefChain();

	// Initialize all entries, to decrypt them forcefully
	for (auto& xref : xref_chain) {
		for (auto& xref_entry : xref) {

			// Accept only used entries
			if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryBasePtr>(xref_entry)) {
				continue;
			}

			// Convert to used entry
			auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(xref_entry);
			auto used_entry_object = used_entry->GetReference();

			ForceObjectInitialization(used_entry_object);

			// The object with all references has been initialized by now
		}
	}
}

void Document::ForceObjectInitialization(ObjectPtr obj) {

	// The stream content has to be handled specifically
	if (ConvertUtils<ObjectPtr>::IsType<StreamObjectPtr>(obj)) {

		// Force the object body initialization
		auto used_stream_object = ConvertUtils<ObjectPtr>::ConvertTo<StreamObjectPtr>(obj);
		used_stream_object->GetBody();

		// Initialize stream dictionary entries
		ForceObjectInitialization(used_stream_object->GetHeader());
	}

	// Dictionary may contain other strings and references
	if (ConvertUtils<ObjectPtr>::IsType<DictionaryObjectPtr>(obj)) {

		// Force the object body initialization
		auto used_dictionary_object = ConvertUtils<ObjectPtr>::ConvertTo<DictionaryObjectPtr>(obj);

		// Initialize also dictionary entries
		for (auto& item : used_dictionary_object) {
			ForceObjectInitialization(item.second);
		}
	}

	// Array may contain other strings and references
	if (ConvertUtils<ObjectPtr>::IsType<MixedArrayObjectPtr>(obj)) {

		auto used_array_object = ConvertUtils<ObjectPtr>::ConvertTo<MixedArrayObjectPtr>(obj);

		// Initialize also array entries
		for (auto& item : used_array_object) {
			ForceObjectInitialization(item);
		}
	}

	// The string values needs to be initialized as well before settings the encryption dictionary
	if (ConvertUtils<ObjectPtr>::IsType<StringObjectPtr>(obj)) {

		// Force the string value initialization
		auto used_string_object = ConvertUtils<ObjectPtr>::ConvertTo<StringObjectPtr>(obj);
		used_string_object->GetValue();
	}
}

} // semantics
} // vanillapdf
