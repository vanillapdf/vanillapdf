#ifndef _DEVELOPER_EXTENSIONS_H
#define _DEVELOPER_EXTENSIONS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"
#include "utils/pdf_version.h"

namespace vanillapdf {
namespace semantics {

class DeveloperExtension : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    explicit DeveloperExtension(syntax::DictionaryObjectPtr root);
    Version BaseVersion(void) const;
    syntax::IntegerObjectPtr ExtensionLevel(void) const;
};

class DeveloperExtensions : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    class Iterator : public BaseIterator<syntax::DictionaryObject::const_iterator> {
    public:
        using BaseIterator<syntax::DictionaryObject::const_iterator>::BaseIterator;
        typedef syntax::DictionaryObject::const_iterator IteratorT;

        const Iterator& operator++() {
            ++BaseIterator<syntax::DictionaryObject::const_iterator>::m_current;
            return *this;
        }

        const Iterator operator++(int) {
            Iterator temp(BaseIterator<syntax::DictionaryObject::const_iterator>::m_current, BaseIterator<syntax::DictionaryObject::const_iterator>::m_invalid);
            ++BaseIterator<syntax::DictionaryObject::const_iterator>::m_current;
            return temp;
        }

        syntax::NameObjectPtr First() const {
            return BaseIterator<syntax::DictionaryObject::const_iterator>::m_current->first;
        }

        DeveloperExtensionPtr Second() const;
    };

    using IteratorPtr = DeferredIterator<Iterator>;

public:
    explicit DeveloperExtensions(syntax::DictionaryObjectPtr root);

    bool TryFind(const syntax::NameObjectPtr& name, OutputDeveloperExtensionPtr& result) const;
    bool Contains(const syntax::NameObjectPtr& name) const;
    void Insert(const syntax::NameObjectPtr& name, const DeveloperExtension& value);
    bool Remove(const syntax::NameObjectPtr& name);
    IteratorPtr Begin(void) const;
    IteratorPtr End(void) const;
};

} // semantics
} // vanillapdf

#endif /* _DEVELOPER_EXTENSIONS_H */
