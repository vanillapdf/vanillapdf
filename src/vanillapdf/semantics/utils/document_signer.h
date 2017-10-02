#ifndef _DOCUMENT_SIGNER_H
#define _DOCUMENT_SIGNER_H

#include "semantics/utils/semantics_fwd.h"

#include "syntax/files/file_writer.h"
#include "syntax/utils/output_pointer.h"

#include "utils/signing_key_interface.h"
#include "utils/unknown_interface.h"

namespace vanillapdf {
namespace semantics {

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
} // vanillapdf

#endif /* _DOCUMENT_SIGNER_H */