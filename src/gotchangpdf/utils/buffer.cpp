#include "precompiled.h"

#include "utils/buffer.h"

#include <cstring>

namespace gotchangpdf {

Buffer::Buffer(size_type count) : m_data(count) {}
Buffer::Buffer(const char * chars) : Buffer(chars, std::strlen(chars)) {}
Buffer::Buffer(const value_type * begin, const value_type * end) : m_data(begin, end) { assert(m_data.size() > 0); }
Buffer::Buffer(size_type count, const value_type& val) : m_data(count, val) {}

size_t Buffer::Hash() const {
	size_t result = 0;

	std::hash<value_type> hasher;
	auto size = m_data.size();
	for (decltype(size) i = 0; i < size; ++i) {
		result ^= hasher(m_data[i]);
	}

	return result;
}

std::shared_ptr<std::stringstream> Buffer::ToStringStream(void) const {
	return std::make_shared<std::stringstream>(ToString());
}

bool Buffer::Equals(const Buffer& other) const {
	return std::operator==(m_data, other.m_data);
}

bool Buffer::LessThan(const Buffer& other) const {
	return std::operator<(m_data, other.m_data);
}

bool operator==(const char * left, const Buffer& right) {
	assert(nullptr != left);
	return (0 == std::strncmp(left, reinterpret_cast<const char*>(right.data()), right.size()));
}

bool operator==(const Buffer& left, const char * right) {
	assert(nullptr != right);
	return (0 == std::strncmp(right, reinterpret_cast<const char*>(left.data()), left.size()));
}

bool operator!=(const char * left, const Buffer& right) {
	assert(nullptr != left);
	return (0 != std::strncmp(left, reinterpret_cast<const char*>(right.data()), right.size()));
}

bool operator!=(const Buffer& left, const char * right) {
	assert(nullptr != right);
	return (0 != std::strncmp(right, reinterpret_cast<const char*>(left.data()), left.size()));
}

std::ostream& operator<<(std::ostream& os, const Buffer& value) {
	os.write(value.data(), value.size());
	return os;
}

} // gotchangpdf
