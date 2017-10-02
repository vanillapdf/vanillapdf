#ifndef _ATTRIBUTE_LIST_H
#define _ATTRIBUTE_LIST_H

#include "utils/attribute_interface.h"

#include <unordered_map>

namespace vanillapdf {

class AttributeList {
public:
	typedef IAttribute::Type key_type;
	typedef IAttributePtr value_type;
	typedef std::unordered_map<key_type, value_type> map_type;

	typedef map_type::iterator iterator;
	typedef map_type::const_iterator const_iterator;

public:
	bool Contains(IAttribute::Type type) const;
	void Add(IAttributePtr attribute);
	bool Remove(IAttributePtr attribute);
	IAttributePtr Get(IAttribute::Type type);
	void Clear();

	template <typename T>
	T GetAs(IAttribute::Type type) {
		IAttributePtr attribute = Get(type);
		return ConvertUtils<IAttributePtr>::ConvertTo<T>(attribute);
	}

	// STL compatibility
	iterator begin() noexcept;
	const_iterator begin() const noexcept;

	iterator end() noexcept;
	const_iterator end() const noexcept;

private:
	map_type m_attributes;
};

} // vanillapdf

#endif /* _ATTRIBUTE_LIST_H */
