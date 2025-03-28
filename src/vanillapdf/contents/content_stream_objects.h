#ifndef _CONTENT_STREAM_OBJECTS_H
#define _CONTENT_STREAM_OBJECTS_H

#include "contents/content_fwd.h"
#include "contents/content_stream_operation_base.h"

#include "syntax/utils/syntax_fwd.h"

#include "utils/modify_observer_interface.h"
#include "utils/constants.h"

#include <vector>

namespace vanillapdf {
namespace contents {

class ContentObjectBase : public InstructionBase {
public:
    enum class Type {
        Undefined = 0,
        TextObject,
        InlineImageObject
    };

    virtual Type GetObjectType(void) const noexcept = 0;
    virtual InstructionBase::Type GetInstructionType(void) const noexcept override { return InstructionBase::Type::Object; }
};

class TextObject : public ContentObjectBase {
public:
    explicit TextObject(const BaseOperationCollection& ops);

    virtual Type GetObjectType(void) const noexcept override { return Type::TextObject; }
    virtual std::string ToPdf() const override;

    types::size_type GetOperationsSize(void) const { return _operations.size(); }
    OperationBasePtr GetOperationAt(types::size_type at) const { return _operations.at(at); }

private:
    BaseOperationCollection _operations;
};

class InlineImageObject : public ContentObjectBase {
public:
    InlineImageObject(syntax::DictionaryObjectPtr dictionary, BufferPtr data)
        : m_dictionary(dictionary), m_data(data) {
    }

    virtual Type GetObjectType(void) const noexcept override { return Type::InlineImageObject; }
    virtual std::string ToPdf() const override;

    syntax::DictionaryObjectPtr GetDictionary() const { return m_dictionary; }
    BufferPtr GetData() const { return m_data; }

private:
    syntax::DictionaryObjectPtr m_dictionary;
    BufferPtr m_data;
};

} // contents
} // vanillapdf

#endif /* _CONTENT_STREAM_OBJECTS_H */
