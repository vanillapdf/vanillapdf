#ifndef _SIGNATURE_FLAGS_H
#define _SIGNATURE_FLAGS_H

#include "semantics/objects/high_level_object.h"

#include "syntax/objects/integer_object.h"

namespace vanillapdf {
namespace semantics {

class SignatureFlags : public HighLevelObject<syntax::IntegerObjectPtr> {
public:
	explicit SignatureFlags(syntax::IntegerObjectPtr value) : HighLevelObject(value) {}

	bool GetSignaturesExist() const { return _obj->IsBitSet((uint32_t) SignatureFlagBit::SignaturesExist); }
	bool GetAppendOnly() const { return _obj->IsBitSet((uint32_t) SignatureFlagBit::AppendOnly); }

	void SetSignaturesExist(bool value) {
		_obj->ToggleBit((uint32_t) SignatureFlagBit::SignaturesExist, value);
	}

	void SetAppendOnly(bool value) {
		_obj->ToggleBit((uint32_t) SignatureFlagBit::AppendOnly, value);
	}

private:
	// 
	enum class SignatureFlagBit : uint32_t {
		SignaturesExist = 0,
		AppendOnly = 1
	};
};

} // semantics
} // vanillapdf

#endif /* _SIGNATURE_FLAGS_H */
