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
		if (other.size() != size())
			return false;

		auto size = this->size();
		for (decltype(size) i = 0; i < size; ++i) {
			if (at(i) != other[i])
				return false;
		}

		return true;
	}

	bool Buffer::operator<(const Buffer& other) const
	{
		auto size = this->size();
		for (decltype(size) i = 0; i < size; ++i) {
			if (at(i) == other[i])
				continue;

			return at(i) < other[i];
		}

		return false;
	}
}
