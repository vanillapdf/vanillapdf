#ifndef _SIGNATURE_FLAGS_H
#define _SIGNATURE_FLAGS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/objects/integer_object.h"

namespace vanillapdf {
namespace semantics {

class SignatureFlags : public HighLevelObject<syntax::IntegerObjectPtr> {
public:
    explicit SignatureFlags(syntax::IntegerObjectPtr value) : HighLevelObject(value) {}

    // Creates flags with every bit cleared. The value is attached as a direct
    // object through InteractiveForm::SetSignatureFlags.
    static SignatureFlagsPtr Create() {
        syntax::IntegerObjectPtr value;
        return make_deferred<SignatureFlags>(value);
    }

    bool GetSignaturesExist() const { return _obj->IsBitSet(SignaturesExistBit); }
    bool GetAppendOnly() const { return _obj->IsBitSet(AppendOnlyBit); }

    void SetSignaturesExist(bool value) {
        _obj->ToggleBit(SignaturesExistBit, value);
    }

    void SetAppendOnly(bool value) {
        _obj->ToggleBit(AppendOnlyBit, value);
    }

private:
    const uint32_t SignaturesExistBit = 0;
    const uint32_t AppendOnlyBit = 1;
};

} // semantics
} // vanillapdf

#endif /* _SIGNATURE_FLAGS_H */
