#ifndef _BYTE_RANGE_H
#define _BYTE_RANGE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

namespace vanillapdf {
namespace semantics {

class ByteRange : public IUnknown {
public:
    ByteRange();
    explicit ByteRange(syntax::IntegerObjectPtr byte_offset, syntax::IntegerObjectPtr length);

    syntax::IntegerObjectPtr ByteOffset(void) const;
    syntax::IntegerObjectPtr Length(void) const;

    void SetByteOffset(syntax::IntegerObjectPtr byte_offset);
    void SetLength(syntax::IntegerObjectPtr length);

private:
    syntax::IntegerObjectPtr m_byte_offset;
    syntax::IntegerObjectPtr m_length;
};

class ByteRangeCollection : public HighLevelObject<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>> {
public:
    ByteRangeCollection();
    explicit ByteRangeCollection(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> obj);

    types::size_type GetSize(void) const;
    ByteRangePtr GetValue(types::size_type at) const;

    void Append(ByteRangePtr byte_range);
    void Insert(types::size_type at, ByteRangePtr byte_range);
    void Remove(types::size_type at);
    void Clear(void);
};

} // semantics
} // vanillapdf

#endif /* _BYTE_RANGE_H */
