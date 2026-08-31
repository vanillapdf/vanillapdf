#include "precompiled.h"

#include "semantics/utils/byte_range.h"
#include "semantics/utils/semantic_exceptions.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

ByteRangeCollection::ByteRangeCollection() {
}

ByteRangeCollection::ByteRangeCollection(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> obj) : HighLevelObject(obj) {
    if (obj->GetSize() % 2 != 0) {
        throw SemanticContextExceptionFactory::Construct<syntax::ArrayObject<syntax::IntegerObjectPtr>, ByteRangeCollection>(obj);
    }
}

ByteRange::ByteRange() {
}

ByteRange::ByteRange(syntax::IntegerObjectPtr byte_offset, syntax::IntegerObjectPtr length)
    : m_byte_offset(byte_offset), m_length(length) {
}

syntax::IntegerObjectPtr ByteRange::ByteOffset(void) const {
    return m_byte_offset;
}

syntax::IntegerObjectPtr ByteRange::Length(void) const {
    return m_length;
}

void ByteRange::SetByteOffset(syntax::IntegerObjectPtr byte_offset) {
    m_byte_offset = byte_offset;
}

void ByteRange::SetLength(syntax::IntegerObjectPtr length) {
    m_length = length;
}

types::size_type ByteRangeCollection::GetSize(void) const {
    return _obj->GetSize() / 2;
}

ByteRangePtr ByteRangeCollection::GetValue(types::size_type at) const {
    return make_deferred<ByteRange>(_obj->GetValue(at * 2), _obj->GetValue(at * 2 + 1));
}

void ByteRangeCollection::Append(ByteRangePtr byte_range) {
    _obj->Append(byte_range->ByteOffset());
    _obj->Append(byte_range->Length());
}

void ByteRangeCollection::Insert(types::size_type at, ByteRangePtr byte_range) {
    _obj->Insert(at * 2, byte_range->ByteOffset());
    _obj->Insert(at * 2 + 1, byte_range->Length());
}

void ByteRangeCollection::Remove(types::size_type at) {
    _obj->Remove(at * 2 + 1);
    _obj->Remove(at * 2);
}

void ByteRangeCollection::Clear(void) {
    _obj->Clear();
}

} // semantics
} // vanillapdf
