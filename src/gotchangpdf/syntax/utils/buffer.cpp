#include "precompiled.h"
#include "buffer.h"

#include <cassert>

namespace gotchangpdf
{
	Buffer::Buffer(size_type count) : base_type(count) {}
	Buffer::Buffer(const value_type * chars) : Buffer(chars, strlen(chars) + 1) {}
	Buffer::Buffer(const value_type * chars, int len) : base_type(&chars[0], &chars[len - 1]) { assert(size() > 0); }
	Buffer::Buffer(const value_type * begin, const value_type * end) : base_type(begin, end) { assert(size() > 0); }
	Buffer::Buffer(size_type count, const value_type& val) : base_type(count, val) {}

	bool Buffer::Equals(const Buffer& other) const
	{
		auto my_base = static_cast<const base_type&>(*this);
		auto other_base = static_cast<const base_type&>(other);
		return std::operator==(my_base, other_base);
	}

	bool operator==(const char * left, const Buffer& right)
	{
		assert(nullptr != left);
		return (0 == strncmp(left, right.data(), right.size()));
	}

	bool operator==(const Buffer& left, const char * right)
	{
		assert(nullptr != right);
		return (0 == strncmp(right, left.data(), left.size()));
	}

	bool operator!=(const char * left, const Buffer& right)
	{
		assert(nullptr != left);
		return (0 != strncmp(left, right.data(), right.size()));
	}

	bool operator!=(const Buffer& left, const char * right)
	{
		assert(nullptr != right);
		return (0 != strncmp(right, left.data(), left.size()));
	}
}
