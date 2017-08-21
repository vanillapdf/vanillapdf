#include "precompiled.h"

#include "semantics/utils/document_signer.h"

#include "syntax/utils/serialization_override_attribute.h"

#include "utils/math_utils.h"

namespace gotchangpdf {
namespace semantics {

using namespace syntax;

DocumentSigner::DocumentSigner(ISigningKeyPtr key, MessageDigestAlgorithm digest, DictionaryObjectPtr signature_dictionary) {
	m_key = key;
	m_digest = digest;
	m_dictionary = signature_dictionary;
}

void DocumentSigner::OnBeforeOutputFlush(IInputOutputStreamPtr output) {

	output->SetInputPosition(0, std::ios_base::end);
	auto output_size = output->GetInputPosition();

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

	byte_ranges->Append(make_deferred<IntegerObject>(0));
	byte_ranges->Append(make_deferred<IntegerObject>(signature_contents_offset));
	byte_ranges->Append(make_deferred<IntegerObject>(after_signature_contents_offset));
	byte_ranges->Append(make_deferred<IntegerObject>(after_signature_contents_length));

	// Make sure byte ranges are valid
	assert(byte_ranges->Size() % 2 == 0);

	output->SetOutputPosition(byte_ranges_offset);
	output->Write(byte_range_raw->ToPdf());

	m_key->SignInitialize(m_digest);

	auto ranges_length = byte_ranges->Size();
	for (decltype(ranges_length) i = 0; i < ranges_length; i += 2) {
		auto offset = byte_ranges[i];
		auto bytes_length = byte_ranges[i + 1];

		auto offset_value = offset->GetIntegerValue();
		auto bytes_length_value = bytes_length->GetIntegerValue();

		// Iterate over byte ranges
		output->SetInputPosition(offset_value);
		m_key->SignUpdate(output, bytes_length_value);
	}

	auto signature = m_key->SignFinal();

	// Seek to the contents and write new signature value
	output->SetOutputPosition(signature_contents_offset);
	output->Write(signature);
}

} // semantics
} // gotchangpdf
