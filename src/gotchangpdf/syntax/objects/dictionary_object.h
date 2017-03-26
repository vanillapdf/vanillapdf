#ifndef _DICTIONARY_OBJECT_H
#define _DICTIONARY_OBJECT_H

#include "syntax/objects/object.h"
#include "syntax/objects/containable.h"

#include "syntax/utils/syntax_fwd.h"
#include "utils/base_iterator.h"

#include <map>
#include <vector>

namespace gotchangpdf {
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

public:
	class Iterator : public BaseIterator<const_iterator>, public IWeakReferenceable {
	public:
		using BaseIterator<const_iterator>::BaseIterator;

		const Iterator& operator++() {
			++BaseIterator<const_iterator>::m_it;
			return *this;
		}

		const Iterator operator++(int) {
			Iterator temp(BaseIterator<const_iterator>::m_it);
			++BaseIterator<const_iterator>::m_it;
			return temp;
		}

		KeyT First() const {
			return BaseIterator<const_iterator>::m_it->first;
		}

		ValueT Second() const {
			return BaseIterator<const_iterator>::m_it->second;
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
	virtual std::string ToString(void) const override;
	virtual std::string ToPdf(void) const override;
	virtual Object::Type GetType(void) const noexcept override { return Object::Type::Dictionary; }

	virtual void SetFile(WeakReference<File> file) noexcept override;
	virtual void SetInitialized(bool initialized = true) noexcept override;

	virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

	virtual size_t Hash() const override;
	virtual DictionaryObject* Clone(void) const override;
	virtual bool Equals(ObjectPtr other) const override;

	template <typename U>
	U FindAs(const NameObject& name) const {
		auto result = Find(name);
		return ObjectUtils::ConvertTo<U>(result);
	}

	ContainableObjectPtr Find(const NameObjectPtr name) const;
	ContainableObjectPtr Find(const NameObject& name) const;

	bool TryFind(const NameObjectPtr name, OutputContainableObjectPtr& result) const;
	bool TryFind(const NameObject& name, OutputContainableObjectPtr& result) const;

	bool Contains(const NameObjectPtr name) const;
	bool Contains(const NameObject& name) const;

	bool Insert(NameObjectPtr name, ContainableObjectPtr value);
	bool Insert(const NameObject& name, ContainableObjectPtr value);

	bool Remove(const NameObjectPtr name);
	bool Remove(const NameObject& name);

	virtual ~DictionaryObject();
};

} // syntax
} // gotchangpdf

#endif /* _DICTIONARY_OBJECT_H */
