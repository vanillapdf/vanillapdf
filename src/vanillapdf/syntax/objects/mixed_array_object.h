#ifndef _MIXED_ARRAY_OBJECT_H
#define _MIXED_ARRAY_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

#include <vector>

namespace vanillapdf {
namespace syntax {

class MixedArrayObject : public ContainableObject, public IModifyObserver {
public:
	typedef std::vector<ContainableObjectPtr> list_type;
	typedef list_type::value_type value_type;
	typedef list_type::size_type size_type;
	typedef list_type::iterator iterator;
	typedef list_type::const_iterator const_iterator;
	typedef list_type::reference reference;
	typedef list_type::const_reference const_reference;
	typedef list_type::difference_type difference_type;

public:
	MixedArrayObject() = default;
	MixedArrayObject(const MixedArrayObject&) = delete;

	explicit MixedArrayObject(const list_type& list);
	explicit MixedArrayObject(const std::initializer_list<ContainableObjectPtr>& list);
	MixedArrayObject(const ContainableObject& other, list_type& list);

	virtual void SetFile(WeakReference<File> file) override;
	virtual void SetInitialized(bool initialized = true) override;

	virtual void ObserveeChanged(const IModifyObservable*) override;
	virtual void OnChanged() const override;

	virtual size_t Hash() const override;
	virtual MixedArrayObject* Clone(void) const override;
	virtual bool Equals(ObjectPtr other) const override;

	virtual Object::Type GetObjectType(void) const noexcept override { return Object::Type::Array; }
	size_type GetSize(void) const noexcept { return _list.size(); }
	const ContainableObjectPtr& operator[](size_type i) const { return _list[i]; }
	ContainableObjectPtr& operator[](size_type i) { return _list[i]; }
	const ContainableObjectPtr& GetValue(size_type at) const { return _list.at(at); }
	ContainableObjectPtr& GetValue(size_type at) { return _list.at(at); }
	void SetValue(size_type at, ContainableObjectPtr value);

	void Append(ContainableObjectPtr value);
	void Insert(size_type at, ContainableObjectPtr value);
	bool Remove(size_type at);
	void Clear();

	// stl compatibility
	void push_back(ContainableObjectPtr value);

	iterator begin() noexcept { return _list.begin(); }
	const_iterator begin() const noexcept { return _list.begin(); }
	iterator end() noexcept { return _list.end(); }
	const_iterator end() const noexcept { return _list.end(); }

	template <typename U>
	ArrayObjectPtr<U> Convert(std::function<U(const ContainableObjectPtr& obj)> f) const {
		return ArrayObjectPtr<U>(*this, f);
	}

	virtual std::string ToString(void) const override;
	virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;
	virtual void ToPdfStreamUpdateOffset(IOutputStreamPtr output) override;

	virtual ~MixedArrayObject();

protected:
	list_type _list;
	
	mutable std::size_t m_hash_cache = 0;
};

} // syntax
} // vanillapdf

#endif /* _MIXED_ARRAY_OBJECT_H */
