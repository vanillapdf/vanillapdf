#ifndef _DESTINATIONS_H
#define _DESTINATIONS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/objects/dictionary_object.h"

namespace vanillapdf {
namespace semantics {

class DestinationBase : public HighLevelObject<syntax::ObjectPtr> {
public:
    enum class Type {
        Undefined = 0,
        XYZ,
        Fit,
        FitHorizontal,
        FitVertical,
        FitRectangle,
        FitBoundingBox,
        FitBoundingBoxHorizontal,
        FitBoundingBoxVertical
    };

public:
    explicit DestinationBase(syntax::MixedArrayObjectPtr root) : HighLevelObject(root) {}
    explicit DestinationBase(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

    static DestinationPtr Create(syntax::MixedArrayObjectPtr root);
    static DestinationPtr Create(syntax::DictionaryObjectPtr root);
    static DestinationPtr Create(syntax::ObjectPtr root);

    // Helper method to resolve destination from any object (array, dictionary, string name, or name object)
    // Handles named destination lookup when needed
    static DestinationPtr ResolveDestination(syntax::ObjectPtr dest_obj);

    syntax::ObjectPtr GetPage() const;

    virtual Type GetType() const noexcept = 0;

protected:
    // Helper method to get the destination array regardless of storage format (array or dictionary)
    syntax::MixedArrayObjectPtr GetDestinationArray() const;
};

class XYZDestination : public DestinationBase {
public:
    explicit XYZDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
    explicit XYZDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
    virtual Type GetType() const noexcept override { return DestinationBase::Type::XYZ; }

    // XYZ destination parameters: [page left top zoom]
    // Returns false if parameter is null (meaning no change)
    bool GetLeft(syntax::ObjectPtr& result) const;
    bool GetTop(syntax::ObjectPtr& result) const;
    bool GetZoom(syntax::ObjectPtr& result) const;
};

class FitDestination : public DestinationBase {
public:
    explicit FitDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
    explicit FitDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
    virtual Type GetType() const noexcept override { return DestinationBase::Type::Fit; }
};

class FitHorizontalDestination : public DestinationBase {
public:
    explicit FitHorizontalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
    explicit FitHorizontalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
    virtual Type GetType() const noexcept override { return DestinationBase::Type::FitHorizontal; }

    // FitH destination parameters: [page /FitH top]
    // Returns false if parameter is null (meaning no change)
    bool GetTop(syntax::ObjectPtr& result) const;
};

class FitVerticalDestination : public DestinationBase {
public:
    explicit FitVerticalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
    explicit FitVerticalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
    virtual Type GetType() const noexcept override { return DestinationBase::Type::FitVertical; }

    // FitV destination parameters: [page /FitV left]
    // Returns false if parameter is null (meaning no change)
    bool GetLeft(syntax::ObjectPtr& result) const;
};

class FitRectangleDestination : public DestinationBase {
public:
    explicit FitRectangleDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
    explicit FitRectangleDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
    virtual Type GetType() const noexcept override { return DestinationBase::Type::FitRectangle; }

    // FitR destination parameters: [page /FitR left bottom right top]
    // Returns false if parameter is null (meaning no change)
    bool GetLeft(syntax::ObjectPtr& result) const;
    bool GetBottom(syntax::ObjectPtr& result) const;
    bool GetRight(syntax::ObjectPtr& result) const;
    bool GetTop(syntax::ObjectPtr& result) const;
};

class FitBoundingBoxDestination : public DestinationBase {
public:
    explicit FitBoundingBoxDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
    explicit FitBoundingBoxDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
    virtual Type GetType() const noexcept override { return DestinationBase::Type::FitBoundingBox; }
};

class FitBoundingBoxHorizontalDestination : public DestinationBase {
public:
    explicit FitBoundingBoxHorizontalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
    explicit FitBoundingBoxHorizontalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
    virtual Type GetType() const noexcept override { return DestinationBase::Type::FitBoundingBoxHorizontal; }

    // FitBH destination parameters: [page /FitBH top]
    // Returns false if parameter is null (meaning no change)
    bool GetTop(syntax::ObjectPtr& result) const;
};

class FitBoundingBoxVerticalDestination : public DestinationBase {
public:
    explicit FitBoundingBoxVerticalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
    explicit FitBoundingBoxVerticalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
    virtual Type GetType() const noexcept override { return DestinationBase::Type::FitBoundingBoxVertical; }

    // FitBV destination parameters: [page /FitBV left]
    // Returns false if parameter is null (meaning no change)
    bool GetLeft(syntax::ObjectPtr& result) const;
};

class NamedDestinations : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    class Iterator : public BaseIterator<syntax::DictionaryObjectPtr::const_iterator> {
    public:
        using BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::BaseIterator;

    public:
        const Iterator& operator++() {
            ++BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::m_current;
            return *this;
        }

        const Iterator operator++(int) {
            Iterator temp(BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::m_current, BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::m_invalid);
            ++BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::m_current;
            return temp;
        }

        std::pair<syntax::NameObjectPtr, DestinationPtr> operator*() {
            return std::pair<syntax::NameObjectPtr, DestinationPtr>(First(), Second());
        }

        syntax::NameObjectPtr First() const {
            return BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::m_current->first;
        }

        DestinationPtr Second() const {
            auto containable = BaseIterator<syntax::DictionaryObjectPtr::const_iterator>::m_current->second;
            return DestinationBase::Create(containable);
        }
    };

public:
    typedef typename std::pair<syntax::NameObjectPtr, DestinationPtr> value_type;
    typedef Iterator iterator;
    typedef Iterator const_iterator;
    typedef typename syntax::DictionaryObjectPtr::size_type size_type;
    typedef typename syntax::DictionaryObjectPtr::reference reference;
    typedef typename syntax::DictionaryObjectPtr::const_reference const_reference;
    typedef typename syntax::DictionaryObjectPtr::difference_type difference_type;

public:
    explicit NamedDestinations(syntax::DictionaryObjectPtr root);

    void Insert(const syntax::NameObject& name, DestinationPtr value);
    bool Contains(const syntax::NameObject& name) const;
    DestinationPtr Find(const syntax::NameObject& name) const;

    // stl compatibility
    iterator begin() {
        return Iterator(_obj->begin(), _obj->end());
    }

    const_iterator begin() const {
        return Iterator(_obj->begin(), _obj->end());
    }

    iterator end() {
        return Iterator(_obj->end(), _obj->end());
    }

    const_iterator end() const {
        return Iterator(_obj->end(), _obj->end());
    }
};

} // semantics
} // vanillapdf

#endif /* _NAME_DICTIONARY_H */
