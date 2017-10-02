#ifndef _DIGITAL_SIGNATURE_H
#define _DIGITAL_SIGNATURE_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/date.h"

namespace vanillapdf {
namespace semantics {

class ByteRange : public virtual IUnknown, public IWeakReferenceable<ByteRange> {
public:
	explicit ByteRange(syntax::IntegerObjectPtr byte_offset, syntax::IntegerObjectPtr length);

	syntax::IntegerObjectPtr ByteOffset(void) const;
	syntax::IntegerObjectPtr Length(void) const;

private:
	syntax::IntegerObjectPtr m_byte_offset;
	syntax::IntegerObjectPtr m_length;
};

class ByteRangeCollection : public HighLevelObject<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>> {
public:
	explicit ByteRangeCollection(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> obj);

	types::size_type Size(void) const;
	ByteRangePtr At(types::size_type at) const;
};

class DigitalSignature : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	explicit DigitalSignature(syntax::DictionaryObjectPtr root);

	syntax::HexadecimalStringObjectPtr Contents();
	syntax::NameObjectPtr Filter();

	bool ByteRange(OuputByteRangeCollectionPtr& result);
	bool Reason(syntax::OutputStringObjectPtr& result);
	bool Location(syntax::OutputStringObjectPtr& result);
	bool ContactInfo(syntax::OutputStringObjectPtr& result);
	bool Date(OutputDatePtr& result);
	bool Name(syntax::OutputStringObjectPtr& result);
	bool Revision(syntax::OutputIntegerObjectPtr& result);
	bool SubFilter(syntax::OutputNameObjectPtr& result);
	bool Certificate(syntax::OutputStringObjectPtr& result);
};

} // semantics
} // vanillapdf

#endif /* _DIGITAL_SIGNATURE_H */