#ifndef _ATTRIBUTE_INTERFACE_H
#define _ATTRIBUTE_INTERFACE_H

#include "unknown_interface.h"

#include <unordered_map>

namespace gotchangpdf {

class IAttribute : public virtual IUnknown {
public:
	enum class Type {
		Empty = 0,
		SerializationOverride
	};

	virtual bool Equals(const IAttribute& other) const;
	virtual Type GetType(void) const noexcept = 0;
	virtual ~IAttribute() = 0;
};

class EmptyAttribute : public virtual IAttribute {
public:
	virtual Type GetType(void) const noexcept;
};

class IAttributePtr : public Deferred<IAttribute> {
public:
	using Deferred<IAttribute>::Deferred;

	IAttributePtr();
};

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

	// STL compatibility
	iterator begin() noexcept;
	const_iterator begin() const noexcept;

	iterator end() noexcept;
	const_iterator end() const noexcept;

private:
	map_type m_attributes;
};

bool operator==(const Deferred<IAttribute>& left, const Deferred<IAttribute>& right);
bool operator!=(const Deferred<IAttribute>& left, const Deferred<IAttribute>& right);

inline IAttribute::Type EmptyAttribute::GetType() const noexcept {
	return Type::Empty;
}

} // gotchangpdf

namespace std {

template <> struct hash<gotchangpdf::IAttribute> {
	size_t operator()(const gotchangpdf::IAttribute& attribute) const;
};

} // std

#endif /* _ATTRIBUTE_INTERFACE_H */
