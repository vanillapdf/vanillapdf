#include "precompiled.h"
#include "syntax/filters/filter.h"
#include "flate_decode_filter.h"
#include "ascii_85_decode_filter.h"
#include "ascii_hex_decode_filter.h"
#include "dct_decode_filter.h"

namespace vanillapdf {
namespace syntax {

FilterBasePtr FilterBase::GetFilterByName(const NameObject& name) {
	if (name.Equals(constant::Name::FlateDecode)) {
		return FlateDecodeFilterPtr();
	} else if (name.Equals(constant::Name::ASCII85Decode)) {
		return ASCII85DecodeFilterPtr();
	} else if (name.Equals(constant::Name::ASCIIHexDecode)) {
		return ASCIIHexDecodeFilterPtr();
	} else if (name.Equals(constant::Name::DCTDecode)) {
		return DCTDecodeFilterPtr();
	}

	throw GeneralException("Unknown filter type: " + name.GetValue()->ToString());
}

} // syntax
} // vanillapdf
