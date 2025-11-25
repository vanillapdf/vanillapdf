#ifndef _DIGITAL_SIGNATURE_H
#define _DIGITAL_SIGNATURE_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/date.h"

namespace vanillapdf {
namespace semantics {

class DigitalSignature : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    explicit DigitalSignature(syntax::DictionaryObjectPtr root);

    syntax::HexadecimalStringObjectPtr Contents();
    syntax::NameObjectPtr Filter();

    bool ByteRange(OuputByteRangeCollectionPtr& result);
    bool Reason(syntax::OutputStringObjectPtr& result);
    bool Location(syntax::OutputStringObjectPtr& result);
    bool ContactInfo(syntax::OutputStringObjectPtr& result);
    bool SigningTime(OutputDatePtr& result);
    bool Name(syntax::OutputStringObjectPtr& result);
    bool Revision(syntax::OutputIntegerObjectPtr& result);
    bool SubFilter(syntax::OutputNameObjectPtr& result);
    bool Certificate(syntax::OutputStringObjectPtr& result);
};

} // semantics
} // vanillapdf

#endif /* _DIGITAL_SIGNATURE_H */