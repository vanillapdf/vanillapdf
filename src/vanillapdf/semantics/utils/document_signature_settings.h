#ifndef _DOCUMENT_SIGNATURE_SETTINGS_H
#define _DOCUMENT_SIGNATURE_SETTINGS_H

#include "semantics/objects/date.h"
#include "semantics/utils/semantics_fwd.h"

#include "syntax/utils/output_pointer.h"

#include "utils/signing_key_interface.h"
#include "utils/unknown_interface.h"

namespace vanillapdf {
namespace semantics {

class DocumentSignatureSettings : public IUnknown {
public:
    MessageDigestAlgorithm GetDigest() const;
    void SetDigest(MessageDigestAlgorithm digest);

    bool GetSigningKey(OutputPointer<ISigningKeyPtr>& result) const;
    bool GetCertificate(OutputPointer<syntax::HexadecimalStringObjectPtr>& result) const;
    bool GetName(OutputPointer<syntax::LiteralStringObjectPtr>& result) const;
    bool GetLocation(OutputPointer<syntax::LiteralStringObjectPtr>& result) const;
    bool GetReason(OutputPointer<syntax::LiteralStringObjectPtr>& result) const;
    bool GetSigningTime(OutputPointer<DatePtr>& result) const;

    // Bytes reserved for the signature. The /Contents placeholder lies inside
    // the signed range, so it cannot be measured afterwards - a signature that
    // does not fit fails at the end of signing. The placeholder derived from
    // this is value * 2 + 2 characters, hex encoded plus the delimiters.
    types::size_type GetReservedSignatureSize() const;
    void SetReservedSignatureSize(types::size_type value);

    void SetSigningKey(ISigningKeyPtr value);
    void SetCertificate(syntax::HexadecimalStringObjectPtr value);
    void SetName(syntax::LiteralStringObjectPtr value);
    void SetLocation(syntax::LiteralStringObjectPtr value);
    void SetReason(syntax::LiteralStringObjectPtr value);
    void SetSigningTime(DatePtr value);

private:
    OutputPointer<ISigningKeyPtr> m_key;
    MessageDigestAlgorithm m_digest = MessageDigestAlgorithm::Undefined;

    OutputPointer<syntax::HexadecimalStringObjectPtr> m_certificate;
    OutputPointer<syntax::LiteralStringObjectPtr> m_name;
    OutputPointer<syntax::LiteralStringObjectPtr> m_location;
    OutputPointer<syntax::LiteralStringObjectPtr> m_reason;

    OutputPointer<DatePtr> m_time;

    // A detached CMS is dominated by the certificates and timestamp token it
    // embeds, not by the signature value. Long-term validation embeds OCSP
    // responses and CRLs on top and has to raise this.
    types::size_type m_reserved_signature_size = 16384;
};

} // semantics
} // vanillapdf

#endif /* _DOCUMENT_SIGNATURE_SETTINGS_H */