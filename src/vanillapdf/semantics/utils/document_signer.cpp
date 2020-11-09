#include "precompiled.h"

#include "semantics/utils/document_signer.h"

#include "syntax/utils/name_constants.h"
#include "syntax/utils/serialization_override_attribute.h"

#include "utils/math_utils.h"

namespace vanillapdf {
namespace semantics {

using namespace syntax;

DocumentSigner::DocumentSigner(ISigningKeyPtr key, MessageDigestAlgorithm digest, DictionaryObjectPtr signature_dictionary) {
	m_key = key;
	m_digest = digest;
	m_dictionary = signature_dictionary;
}

void DocumentSigner::OnAfterObjectWrite(ObjectPtr obj) {
	if (obj->GetObjectNumber() != m_dictionary->GetObjectNumber()) {
		return;
	}

	if (obj->GetGenerationNumber() != m_dictionary->GetGenerationNumber()) {
		return;
	}

	m_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(obj);
}

void DocumentSigner::OnBeforeOutputFlush(IInputOutputStreamPtr output) {

	output->SetOutputPosition(0, SeekDirection::End);
	auto output_size = output->GetOutputPosition();

	auto byte_ranges = m_dictionary->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::ByteRange);
	auto signature_contents = m_dictionary->FindAs<HexadecimalStringObjectPtr>(constant::Name::Contents);

	auto byte_range_raw = byte_ranges->Data();
	auto byte_ranges_offset = byte_range_raw->GetOffset();
	auto signature_contents_offset = signature_contents->GetOffset();

	auto signature_contents_attribute = signature_contents->GetAttributeAs<SerializationOverrideAttributePtr>(IAttribute::Type::SerializationOverride);
	auto signature_contents_overriden_value = signature_contents_attribute->GetValue();
	auto after_signature_contents_offset = SafeAddition<types::stream_offset>(signature_contents_offset, signature_contents_overriden_value.size());
	auto after_signature_contents_length = output_size - after_signature_contents_offset;

	assert(signature_contents_offset < after_signature_contents_offset);
	assert(after_signature_contents_length > 0);

	ArrayObjectPtr<IntegerObjectPtr> new_ranges;
	new_ranges->Append(make_deferred<IntegerObject>(0));
	new_ranges->Append(make_deferred<IntegerObject>(signature_contents_offset));
	new_ranges->Append(make_deferred<IntegerObject>(after_signature_contents_offset));
	new_ranges->Append(make_deferred<IntegerObject>(after_signature_contents_length));

	// Make sure byte ranges are valid
	assert(new_ranges->GetSize() % 2 == 0);

	output->SetOutputPosition(byte_ranges_offset);
	output->Write(new_ranges->ToPdf());

	m_key->SignInitialize(m_digest);

	auto ranges_length = new_ranges->GetSize();
	for (decltype(ranges_length) i = 0; i < ranges_length; i += 2) {
		auto offset = new_ranges[i];
		auto bytes_length = new_ranges[i + 1];

		auto offset_value = offset->GetIntegerValue();
		auto bytes_length_value = bytes_length->GetIntegerValue();

		// Iterate over byte ranges
		output->SetInputPosition(offset_value);
		m_key->SignUpdate(output, bytes_length_value);
	}

	auto signature = m_key->SignFinal();

	HexadecimalStringObjectPtr new_signature_contents;
	new_signature_contents->SetValue(signature);
	new_signature_contents->SetInitialized();

	std::string signature_encoded = new_signature_contents->ToPdf();
	if (signature_encoded.size() > signature_contents_overriden_value.size()) {
		throw GeneralException("Pre-allocated signature size is not sufficient");
	}

	auto padding_length = signature_contents_overriden_value.size() - signature_encoded.size();
	std::string padded_signature = signature_encoded.insert(signature_encoded.size() - 1, padding_length, '0');

	// Seek to the contents and write new signature value
	output->SetOutputPosition(signature_contents_offset);
	output->Write(padded_signature);
}

} // semantics
} // vanillapdf
