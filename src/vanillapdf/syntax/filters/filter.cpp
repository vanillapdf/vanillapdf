#include "precompiled.h"

#include "syntax/filters/filter.h"
#include "syntax/filters/flate_decode_filter.h"
#include "syntax/filters/ascii_85_decode_filter.h"
#include "syntax/filters/ascii_hex_decode_filter.h"
#include "syntax/filters/dct_decode_filter.h"
#include "syntax/filters/lzw_decode_filter.h"
#include "syntax/filters/jpx_decode_filter.h"

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
    } else if (name.Equals(constant::Name::JPXDecode)) {
        return JPXDecodeFilterPtr();
    }

    LOG_ERROR_AND_THROW(NotSupportedException, "Unknown filter type: {}", name.GetValue()->ToString());
}

const NameObject& FilterBase::GetFullFilterName(const NameObject& filter_name) {
    if (filter_name.Equals(constant::Name::AHx)) {
        return constant::Name::ASCIIHexDecode;
    } else if (filter_name.Equals(constant::Name::A85)) {
        return constant::Name::ASCII85Decode;
    } else if (filter_name.Equals(constant::Name::LZW)) {
        return constant::Name::LZWDecode;
    } else if (filter_name.Equals(constant::Name::Fl)) {
        return constant::Name::FlateDecode;
    } else if (filter_name.Equals(constant::Name::DCT)) {
        return constant::Name::DCTDecode;
    }

    return filter_name;
}

} // syntax
} // vanillapdf
