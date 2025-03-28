#ifndef _PAGE_LABEL_H
#define _PAGE_LABEL_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/tree.h"

#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/integer_object.h"
#include "syntax/objects/string_object.h"

namespace vanillapdf {
namespace semantics {

class PageLabel : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    enum class NumberingStyle : char {
        Undefined = 0,
        Decimal = 'D',
        UpperRoman = 'R',
        LowerRoman = 'r',
        UpperLetters = 'A',
        LowerLetters = 'a'
    };

public:
    explicit PageLabel(syntax::DictionaryObjectPtr obj);

    bool Prefix(syntax::StringObjectPtr& result) const;
    bool Start(syntax::IntegerObjectPtr& result) const;
    bool Style(NumberingStyle& result) const;
};

class PageLabels : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    explicit PageLabels(const syntax::DictionaryObjectPtr& obj);

    bool Contains(types::size_type page_number) const;
    PageLabelPtr Find(types::size_type page_number) const;

private:
    NumberTreePtr<PageLabelPtr> _tree;

    static PageLabelPtr ConvertFunction(const syntax::ContainableObjectPtr& item);
};

} // semantics

bool operator==(const semantics::PageLabelPtr& left, const semantics::PageLabelPtr& right);
bool operator!=(const semantics::PageLabelPtr& left, const semantics::PageLabelPtr& right);
bool operator<(const semantics::PageLabelPtr& left, const semantics::PageLabelPtr& right);

} // vanillapdf

#endif /* _PAGE_LABEL_H */
