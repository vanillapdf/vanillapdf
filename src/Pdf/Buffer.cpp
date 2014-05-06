#include "Buffer.h"

#include <cassert>

namespace Pdf
{
	#pragma region Constructors

	Buffer::Buffer() : _value() {}
	Buffer::Buffer(const char *chars, int len) : _value(&chars[0], &chars[len - 1]) { assert(_value.size() > 0); }
	Buffer::Buffer(const char *begin, const char *end) : _value(begin, end) { assert(_value.size() > 0); }

	#pragma endregion

	#pragma region Operators

	bool Buffer::operator==(const IBuffer& other) const
	{
		const Buffer *o = dynamic_cast<const Buffer*>(&other);
		if (nullptr == o)
			return false;

		return _value == o->_value;
	}

	bool Buffer::operator<(const IBuffer& other) const
	{
		const Buffer *o = dynamic_cast<const Buffer*>(&other);
		if (nullptr == o)
			return false;

		return _value < o->_value;
	}

	bool Buffer::operator==(const Buffer& other) const { return _value == other._value; }
	bool Buffer::operator<(const Buffer& other) const { return _value < other._value; }

	Character Buffer::operator[](unsigned int i) const { return _value[i]; }

	#pragma endregion

	void Buffer::Reverse() { std::reverse(_value.begin(), _value.end()); }
	Character Buffer::At(long at) const { return _value[at]; }
	void Buffer::Append(const Character& ch) { _value.push_back(ch); }
	void Buffer::Append(const Buffer& item) { Insert(0, item); }
	void Buffer::Insert(int idx, const Buffer& item) { _value.insert(_value.begin() + idx, item._value.begin(), item._value.end()); }
	int Buffer::Size() const { return _value.size(); }
	std::string Buffer::ToString(void) const { return std::string(_value.begin(), _value.end()); }
}
