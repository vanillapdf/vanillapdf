#ifndef _DOCUMENT_SIGNATURE_SETTINGS_H
#define _DOCUMENT_SIGNATURE_SETTINGS_H

#include "semantics/objects/date.h"
#include "semantics/utils/semantics_fwd.h"

#include "syntax/utils/output_pointer.h"

#include "utils/signing_key_interface.h"
#include "utils/unknown_interface.h"

namespace vanillapdf {
namespace semantics {

class DocumentSignatureSettings : public virtual IUnknown {
public:
	MessageDigestAlgorithm GetDigest() const;
	void SetDigest(MessageDigestAlgorithm digest);

	bool GetSigningKey(OutputPointer<ISigningKeyPtr>& result) const;
	bool GetCertificate(OutputPointer<syntax::HexadecimalStringObjectPtr>& result) const;
	bool GetName(OutputPointer<syntax::LiteralStringObjectPtr>& result) const;
	bool GetLocation(OutputPointer<syntax::LiteralStringObjectPtr>& result) const;
	bool GetReason(OutputPointer<syntax::LiteralStringObjectPtr>& result) const;
	bool GetSigningTime(OutputPointer<DatePtr>& result) const;

	void SetSigningKey(ISigningKeyPtr value);
	void SetCertificate(syntax::HexadecimalStringObjectPtr value);
	void SetName(syntax::LiteralStringObjectPtr value);
	void SetLocation(syntax::LiteralStringObjectPtr value);
	void SetReason(syntax::LiteralStringObjectPtr value);
	void SetSigningTime(DatePtr value);

private:
	OutputPointer<ISigningKeyPtr> m_key;
	MessageDigestAlgorithm m_digest;

	OutputPointer<syntax::HexadecimalStringObjectPtr> m_certificate;
	OutputPointer<syntax::LiteralStringObjectPtr> m_name;
	OutputPointer<syntax::LiteralStringObjectPtr> m_location;
	OutputPointer<syntax::LiteralStringObjectPtr> m_reason;

	OutputPointer<DatePtr> m_time;
};

} // semantics
} // vanillapdf

#endif /* _DOCUMENT_SIGNATURE_SETTINGS_H */