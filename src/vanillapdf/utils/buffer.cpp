#include "precompiled.h"

#include "utils/buffer.h"
#include "utils/streams/input_stream.h"

#include <iomanip>
#include <cstring>
#include <sstream>

namespace vanillapdf {

Buffer::Buffer(const char * chars) : Buffer(chars, std::strlen(chars)) {}
Buffer::Buffer(const std::string& data) : Buffer(data.begin(), data.end()) {}
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

std::string Buffer::ToHexString(void) const {
	std::stringstream result;

	for (const auto& v : m_data) {
		result << std::setfill('0') << std::setw(sizeof(v) * 2) << std::hex << (int) v;
	}

	return result.str();
}

IInputStreamPtr Buffer::ToInputStream(void) const {
	auto result = std::make_shared<std::stringstream>();
	result->write(m_data.data(), m_data.size());
	return make_deferred<InputStream>(result);
}

bool Buffer::Equals(const Buffer& other) const {
	return (m_data == other.m_data);
}

bool Buffer::LessThan(const Buffer& other) const {
	return (m_data < other.m_data);
}

bool Buffer::ValueEqualLessThan(const Buffer& other) const {
	auto src_size = size();
	auto dest_size = other.size();

	if (src_size != dest_size) {
		return (src_size < dest_size);
	}

	for (decltype(src_size) i = 0; i < src_size; ++i) {
		auto src_byte = (*this)[i];
		auto dest_byte = other[i];

		using unsigned_src_type = std::make_unsigned<decltype(src_byte)>::type;
		using unsigned_dest_type = std::make_unsigned<decltype(dest_byte)>::type;

		auto unsigned_src = reinterpret_cast<unsigned_src_type&>(src_byte);
		auto unsigned_dest = reinterpret_cast<unsigned_dest_type&>(dest_byte);

		if (unsigned_src != unsigned_dest) {
			return (unsigned_src < unsigned_dest);
		}
	}

	return true;
}

bool operator==(const char * left, const Buffer& right) {
	assert(nullptr != left);
	return (0 == std::strncmp(left, reinterpret_cast<const char*>(right.data()), right.std_size()));
}

bool operator==(const Buffer& left, const char * right) {
	assert(nullptr != right);
	return (0 == std::strncmp(right, reinterpret_cast<const char*>(left.data()), left.std_size()));
}

bool operator!=(const char * left, const Buffer& right) {
	assert(nullptr != left);
	return (0 != std::strncmp(left, reinterpret_cast<const char*>(right.data()), right.std_size()));
}

bool operator!=(const Buffer& left, const char * right) {
	assert(nullptr != right);
	return (0 != std::strncmp(right, reinterpret_cast<const char*>(left.data()), left.std_size()));
}

std::ostream& operator<<(std::ostream& os, const Buffer& value) {
	os.write(value.data(), value.size());
	return os;
}

} // vanillapdf
