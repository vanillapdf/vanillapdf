#include "precompiled.h"

#include "semantics/utils/semantic_utils.h"

#include <regex>
#include <unordered_map>

namespace gotchangpdf {
namespace semantics {

Version SemanticUtils::GetVersionFromName(const syntax::NameObjectPtr& name) {
	auto ver_str = name->GetValue()->ToString();

	std::smatch sm;
	std::regex header_regex("([0-9])\\.([0-9])");
	if (!std::regex_match(ver_str, sm, header_regex))
		throw NotSupportedException("Unsupported pdf version: " + ver_str);

	assert(sm.size() == 3);
	if (1 != stoi(sm[1]))
		throw NotSupportedException("Unsupported pdf version: " + ver_str);

	switch (stoi(sm[2])) {
		case 0: return Version::PDF10;
		case 1: return Version::PDF11;
		case 2: return Version::PDF12;
		case 3: return Version::PDF13;
		case 4: return Version::PDF14;
		case 5: return Version::PDF15;
		case 6: return Version::PDF16;
		case 7: return Version::PDF17;
		default: throw NotSupportedException("Unsupported pdf version: " + ver_str);
	}
}

using document_map_type = std::unordered_map<syntax::File*, WeakReference<Document>>;
using document_map_ptr_type = std::shared_ptr<document_map_type>;

static std::mutex document_map_lock;
document_map_ptr_type GetDocumentMapInstance() {
	static document_map_ptr_type document_map;

	if (!document_map) {
		document_map = std::make_shared<document_map_type>();
	}

	return document_map;
}

WeakReference<Document> SemanticUtils::GetMappedDocument(WeakReference<syntax::File> file) {
	if (!file.IsActive()) {
		throw syntax::FileDisposedException();
	}

	std::lock_guard<std::mutex> locker(document_map_lock);
	auto document_map = GetDocumentMapInstance();

	auto shared = file.GetReference();
	auto weak_ref = (*document_map)[shared.get()];
	assert(weak_ref.IsActive() && !weak_ref.IsEmpty());

	return weak_ref;
}

void SemanticUtils::AddDocumentMapping(WeakReference<syntax::File> file, WeakReference<Document> value) {
	if (!file.IsActive()) {
		throw syntax::FileDisposedException();
	}

	std::lock_guard<std::mutex> locker(document_map_lock);
	auto document_map = GetDocumentMapInstance();

	auto shared = file.GetReference();
	(*document_map)[shared.get()] = value;
}

void SemanticUtils::ReleaseMapping(WeakReference<syntax::File> file) {
	if (!file.IsActive()) {
		throw syntax::FileDisposedException();
	}

	std::lock_guard<std::mutex> locker(document_map_lock);
	auto document_map = GetDocumentMapInstance();

	auto shared = file.GetReference();
	document_map->erase(shared.get());
}

} // semantics
} // gotchangpdf
