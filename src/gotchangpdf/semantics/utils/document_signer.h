#ifndef _DOCUMENT_SIGNER_H
#define _DOCUMENT_SIGNER_H

#include "semantics/utils/semantics_fwd.h"

#include "syntax/files/file_writer.h"
#include "syntax/utils/output_pointer.h"

#include "utils/signing_key_interface.h"
#include "utils/unknown_interface.h"

namespace gotchangpdf {
namespace semantics {

class SigningOptions : public virtual IUnknown {
public:
	MessageDigestAlgorithm GetDigest() const;
	void SetDigest(MessageDigestAlgorithm digest);

	bool GetSigningKey(OutputPointer<ISigningKeyPtr>& result) const;
	bool GetCertificate(OutputPointer<syntax::HexadecimalStringObjectPtr>& result) const;
	bool GetName(OutputPointer<syntax::LiteralStringObjectPtr>& result) const;
	bool GetLocation(OutputPointer<syntax::LiteralStringObjectPtr>& result) const;
	bool GetReason(OutputPointer<syntax::LiteralStringObjectPtr>& result) const;

	void SetSigningKey(ISigningKeyPtr value);
	void SetCertificate(syntax::HexadecimalStringObjectPtr value);
	void SetName(syntax::LiteralStringObjectPtr value);
	void SetLocation(syntax::LiteralStringObjectPtr value);
	void SetReason(syntax::LiteralStringObjectPtr value);

private:
	OutputPointer<ISigningKeyPtr> m_key;
	MessageDigestAlgorithm m_digest;

	OutputPointer<syntax::HexadecimalStringObjectPtr> m_certificate;
	OutputPointer<syntax::LiteralStringObjectPtr> m_name;
	OutputPointer<syntax::LiteralStringObjectPtr> m_location;
	OutputPointer<syntax::LiteralStringObjectPtr> m_reason;
};

class DocumentSigner : public virtual IUnknown, public syntax::IFileWriterObserver {
public:
	DocumentSigner(ISigningKeyPtr key, MessageDigestAlgorithm digest, syntax::DictionaryObjectPtr signature_dictionary);
	void OnBeforeOutputFlush(IInputOutputStreamPtr output) override;

private:
	syntax::DictionaryObjectPtr m_dictionary;
	OutputPointer<ISigningKeyPtr> m_key;
	MessageDigestAlgorithm m_digest;
};

} // semantics
} // gotchangpdf

#endif /* _DOCUMENT_SIGNER_H */