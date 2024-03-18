#ifndef _ATTRIBUTE_LIST_H
#define _ATTRIBUTE_LIST_H

#include "utils/unknown_interface.h"
#include "utils/base_attribute.h"

#include <memory>
#include <unordered_map>

namespace vanillapdf {

class AttributeList : public IUnknown {
public:
	typedef BaseAttribute::Type key_type;
	typedef BaseAttributePtr value_type;
	typedef std::unordered_map<key_type, value_type> map_type;

	// DeferredContainer
	//typedef typename T::value_type value_type;
	typedef map_type::iterator iterator;
	typedef map_type::const_iterator const_iterator;
	typedef map_type::size_type size_type;
	typedef map_type::reference reference;
	typedef map_type::const_reference const_reference;
	typedef map_type::difference_type difference_type;

public:
	bool Contains(BaseAttribute::Type type) const;
	void Add(BaseAttributePtr attribute);
	bool Remove(BaseAttribute::Type type);
	BaseAttributePtr Get(BaseAttribute::Type type) const;
	void Clear();

	template <typename T>
	T GetAs(BaseAttribute::Type type) const {
		BaseAttributePtr attribute = Get(type);
		return ConvertUtils<BaseAttributePtr>::ConvertTo<T>(attribute);
	}

	// STL compatibility
	iterator begin();
	const_iterator begin() const;

	iterator end();
	const_iterator end() const;

private:
	mutable std::shared_ptr<map_type> m_attributes;

	std::shared_ptr<map_type> GetAttributes() const;
};

} // vanillapdf

#endif /* _ATTRIBUTE_LIST_H */
