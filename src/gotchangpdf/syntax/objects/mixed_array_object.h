#ifndef _MIXED_ARRAY_OBJECT_H
#define _MIXED_ARRAY_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

#include <vector>

namespace gotchangpdf {
namespace syntax {

class MixedArrayObject : public ContainableObject, public IModifyObserver {
public:
	typedef std::vector<ContainableObjectPtr> list_type;
	typedef list_type::value_type value_type;
	typedef list_type::iterator iterator;
	typedef list_type::const_iterator const_iterator;
	typedef list_type::size_type size_type;
	typedef list_type::reference reference;
	typedef list_type::const_reference const_reference;

public:
	MixedArrayObject() = default;
	explicit MixedArrayObject(const list_type& list);
	explicit MixedArrayObject(const std::initializer_list<ContainableObjectPtr>& list);
	MixedArrayObject(const ContainableObject& other, list_type& list);

	virtual void SetFile(WeakReference<File> file) noexcept override;
	virtual void SetInitialized(bool initialized = true) noexcept override;

	virtual void ObserveeChanged(IModifyObservable*) override;

	virtual MixedArrayObject* Clone(void) const override;
	virtual bool Equals(ObjectPtr other) const override;

	virtual Object::Type GetType(void) const noexcept override { return Object::Type::Array; }
	size_t Size(void) const noexcept { return _list.size(); }
	const ContainableObjectPtr& operator[](size_t i) const { return _list[i]; }
	ContainableObjectPtr& operator[](size_t i) { return _list[i]; }
	const ContainableObjectPtr& At(size_t at) const { return _list.at(at); }
	ContainableObjectPtr& At(size_t at) { return _list.at(at); }

	void Append(const ContainableObjectPtr& value);
	void Insert(const ContainableObjectPtr& value, size_t at);
	void Remove(size_t at);

	// stl compatibility
	void push_back(const value_type& value);

	iterator begin() noexcept { return _list.begin(); }
	const_iterator begin() const noexcept { return _list.begin(); }
	iterator end() noexcept { return _list.end(); }
	const_iterator end() const noexcept { return _list.end(); }

	template <typename U>
	ArrayObjectPtr<U> Convert(std::function<U(const ContainableObjectPtr& obj)> f) const {
		return ArrayObjectPtr<U>(*this, f);
	}

	virtual std::string ToString(void) const override;
	virtual std::string ToPdf(void) const override;

	virtual ~MixedArrayObject();

protected:
	list_type _list;
};

} // syntax
} // gotchangpdf

#endif /* _MIXED_ARRAY_OBJECT_H */
