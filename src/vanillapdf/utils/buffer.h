#ifndef _BUFFER_H
#define _BUFFER_H

#include "utils/character.h"
#include "utils/conversion_utils.h"
#include "utils/unknown_interface.h"
#include "utils/modify_observer_interface.h"

#include "utils/streams/input_stream_interface.h"

#include <vector>
#include <string>
#include <ostream>
#include <cassert>

namespace vanillapdf {

class Buffer : public IUnknown, public IWeakReferenceable<Buffer>, public IModifyObservable {
public:
	using storage_type = std::vector<char>;

public:
	using value_type = storage_type::value_type;
	using allocator_type = storage_type::allocator_type;
	using iterator = storage_type::iterator;
	using size_type = storage_type::size_type;
	using const_iterator = storage_type::const_iterator;
	using reference = storage_type::reference;
	using const_reference = storage_type::const_reference;
	using difference_type = storage_type::difference_type;

	static_assert(sizeof(size_t) <= sizeof(storage_type::size_type), "Buffer size type is too small");
	static_assert(std::is_unsigned<storage_type::size_type>::value, "Buffer size type should be unsigned");

public:
	Buffer() = default;

	explicit Buffer(const char * chars);
	explicit Buffer(const std::string& data);
	Buffer(const value_type * begin, const value_type * end);
	Buffer(size_type count, const value_type& val);

	template <
		typename T,
		typename = typename std::enable_if<std::is_integral<T>::value>::type
	>
	explicit Buffer(T count) {
		auto count_converted = ValueConvertUtils::SafeConvert<uint32_t>(count);
		m_data.resize(count_converted);
	}

	template <typename InputIterator>
	Buffer(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()) : m_data(first, last, alloc) {}

	template <
		typename T,
		typename = typename std::enable_if<sizeof(T) == sizeof(value_type)>
	>
	Buffer(const T * chars, size_type len) : m_data(
		reinterpret_cast<const value_type *>(&chars[0]),
		reinterpret_cast<const value_type *>(&chars[len])
		) {
		assert(size() > 0);
	}

	size_t Hash() const;
	BufferPtr Clone(void) const { return make_deferred_container<Buffer>(begin(), end()); }
	std::string ToString(void) const { return std::string(begin(), end()); }
	std::string ToHexString(void) const;
	IInputStreamPtr ToInputStream(void) const;
	bool Equals(const Buffer& other) const;
	bool LessThan(const Buffer& other) const;

	bool ValueEqualLessThan(const Buffer& other) const;

	// stl compatibility
	bool empty(void) const noexcept { return m_data.empty(); }
	value_type * data(void) noexcept { return m_data.data(); }
	const value_type * data(void) const noexcept { return m_data.data(); }
	reference at(size_type pos) { return m_data.at(pos); }
	const_reference at(size_type pos) const { return m_data.at(pos); }
	size_type size(void) const noexcept { return m_data.size(); }
	iterator begin(void) noexcept { return m_data.begin(); }
	const_iterator begin(void) const noexcept { return m_data.begin(); }
	iterator end(void) noexcept { return m_data.end(); }
	const_iterator end(void) const noexcept { return m_data.end(); }
	reference front(void) { return m_data.front(); }
	const_reference front(void) const { return m_data.front(); }
	reference back(void) { return m_data.back(); }
	const_reference back(void) const { return m_data.back(); }
	reference operator[](size_type pos) { return m_data[pos]; }
	const_reference operator[](size_type pos) const { return m_data[pos]; }

	// size_type might not be of type size_t
	// size_t is often needed from other standard functions
	size_t std_size(void) const { return ValueConvertUtils::SafeConvert<size_t>(m_data.size()); }

	// Modifying operations
	void resize(size_type new_size) { m_data.resize(new_size); OnChanged(); }
	void reserve(size_type count) { m_data.reserve(count); OnChanged(); }
	void push_back(const_reference val) { m_data.push_back(val); OnChanged(); }
	void push_back(value_type&& val) { m_data.push_back(val); OnChanged(); }
	void push_back(WhiteSpace val) { push_back(static_cast<char>(val)); }
	void push_back(Delimiter val) { push_back(static_cast<char>(val)); }

	iterator insert(const_iterator where, const value_type& val) {
		auto result = m_data.insert(where, val);
		OnChanged();
		return result;
	}

	iterator insert(const_iterator where, value_type&& val) {
		auto result = m_data.insert(where, val);
		OnChanged();
		return result;
	}

	iterator insert(iterator pos, size_type count, const_reference val) {
		auto result = m_data.insert(pos, count, val);
		OnChanged();
		return result;
	}

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last) {
		m_data.assign(first, last);
		OnChanged();
	}

	template <class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last) {
		m_data.insert(position, first, last);
		OnChanged();
	}

private:
	storage_type m_data;
};

inline bool operator==(const Buffer& left, const Buffer& right) { return left.Equals(right); }
inline bool operator!=(const Buffer& left, const Buffer& right) { return !left.Equals(right); }
inline bool operator<(const Buffer& left, const Buffer& right) { return left.LessThan(right); }

bool operator==(const char * left, const Buffer& right);
bool operator==(const Buffer& left, const char * right);
bool operator!=(const char * left, const Buffer& right);
bool operator!=(const Buffer& left, const char * right);

std::ostream& operator<<(std::ostream& os, const Buffer& value);

} // vanillapdf

#endif /* _BUFFER_H */
