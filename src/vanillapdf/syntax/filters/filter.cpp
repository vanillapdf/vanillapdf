#include "precompiled.h"

#include "syntax/filters/filter.h"
#include "syntax/filters/flate_decode_filter.h"
#include "syntax/filters/ascii_85_decode_filter.h"
#include "syntax/filters/ascii_hex_decode_filter.h"
#include "syntax/filters/dct_decode_filter.h"
#include "syntax/filters/lzw_decode_filter.h"

#include "syntax/utils/name_constants.h"

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
	} else if (name.Equals(constant::Name::LZWDecode)) {
		return LZWDecodeFilterPtr();
	}

	throw GeneralException("Unknown filter type: " + name.GetValue()->ToString());
}

} // syntax
} // vanillapdf
