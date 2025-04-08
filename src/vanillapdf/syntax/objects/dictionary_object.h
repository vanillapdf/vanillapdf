#ifndef _DICTIONARY_OBJECT_H
#define _DICTIONARY_OBJECT_H

#include "syntax/objects/object.h"
#include "syntax/objects/containable.h"

#include "syntax/utils/syntax_fwd.h"
#include "syntax/utils/output_pointer.h"

#include "utils/base_iterator.h"

#include <map>
#include <vector>
#include <mutex>

namespace vanillapdf {
namespace syntax {

template <typename KeyT, typename ValueT, typename MapT = std::map<KeyT, ValueT>>
class DictionaryObjectBase : public ContainableObject {
public:
    //typedef std::unordered_map<NameObjectPtr, ContainableObject, std::hash<NameObjectPtr>> list_type;
    typedef MapT list_type;

    typedef typename list_type::value_type value_type;
    typedef typename list_type::iterator iterator;
    typedef typename list_type::const_iterator const_iterator;
    typedef typename list_type::size_type size_type;
    typedef typename list_type::reference reference;
    typedef typename list_type::const_reference const_reference;
    typedef typename list_type::difference_type difference_type;

public:
    class Iterator : public BaseIterator<const_iterator> {
    public:
        using BaseIterator<const_iterator>::BaseIterator;

        const Iterator& operator++() {
            ++BaseIterator<const_iterator>::m_current;
            return *this;
        }

        const Iterator operator++(int) {
            Iterator temp(BaseIterator<const_iterator>::m_current, BaseIterator<const_iterator>::m_invalid);
            ++BaseIterator<const_iterator>::m_current;
            return temp;
        }

        const KeyT First() const {
            return BaseIterator<const_iterator>::m_current->first;
        }

        ValueT Second() const {
            return BaseIterator<const_iterator>::m_current->second;
        }
    };

    using IteratorPtr = DeferredIterator<Iterator>;

    // std container
    iterator begin(void) { return _list.begin(); }
    const_iterator begin(void) const { return _list.begin(); }

    iterator end(void) { return _list.end(); }
    const_iterator end(void) const { return _list.end(); }

    iterator insert(const_iterator pos, const value_type & value) { return _list.insert(pos, value); }
    size_type size() const { return _list.size(); }

protected:
    list_type _list;
};

class DictionaryObject : public DictionaryObjectBase<NameObjectPtr, ContainableObjectPtr>, public IModifyObserver {
public:
    DictionaryObject();
    DictionaryObject(const DictionaryObject&) = delete;

    virtual std::string ToString(void) const override;
    virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;
    virtual void ToPdfStreamUpdateOffset(IOutputStreamPtr output) override;
    virtual Object::Type GetObjectType(void) const noexcept override { return Object::Type::Dictionary; }

    virtual void SetFile(WeakReference<File> file) override;
    virtual void SetInitialized(bool initialized = true) override;

    virtual void ObserveeChanged(const IModifyObservable*) override;
    virtual void OnChanged() override;

    virtual size_t Hash() const override;
    virtual DictionaryObject* Clone(void) const override;
    virtual bool Equals(ObjectPtr other) const override;

    template <typename U>
    U FindAs(const NameObject& name) const {
        NameObjectPtr temp = make_deferred<NameObject>(name);
        return FindAs<U>(temp);
    }

    template <typename U>
    U FindAs(const NameObjectPtr name) const {
        auto result = Find(name);
        return ObjectUtils::ConvertTo<U>(result);
    }

    ContainableObjectPtr Find(const NameObjectPtr name) const;
    ContainableObjectPtr Find(const NameObject& name) const;

    template <typename U>
    bool TryFindAs(const NameObject& name, OutputPointer<U>& result) const {
        NameObjectPtr temp = make_deferred<NameObject>(name);
        return TryFindAs<U>(temp, result);
    }

    template <typename U>
    bool TryFindAs(const NameObjectPtr name, OutputPointer<U>& result) const {
        OutputContainableObjectPtr containable_result;
        bool found = TryFind(name, containable_result);

        if (!found) {
            return false;
        }

        result = ObjectUtils::ConvertTo<U>(*containable_result);
        return true;
    }

    bool TryFind(const NameObjectPtr name, OutputContainableObjectPtr& result) const;
    bool TryFind(const NameObject& name, OutputContainableObjectPtr& result) const;

    bool Contains(const NameObjectPtr name) const;
    bool Contains(const NameObject& name) const;

    void Insert(NameObjectPtr name, ContainableObjectPtr value, bool overwrite = false);
    void Insert(const NameObject& name, ContainableObjectPtr value, bool overwrite = false);

    bool Remove(const NameObjectPtr name);
    bool Remove(const NameObject& name);

    void Merge(const DictionaryObject& other);
    void Clear();

    size_type GetSize() const noexcept;

    virtual ~DictionaryObject();

private:
    mutable size_t m_hash_cache = 0;

    // The library interface wants to be thread-safe as much as possible
    // Even though the are currently no cases for multi-thread access
    // to the dictonary, let's try to be visionary and prepare for this
    std::shared_ptr<std::recursive_mutex> m_access_lock;
};

} // syntax
} // vanillapdf

#endif /* _DICTIONARY_OBJECT_H */
