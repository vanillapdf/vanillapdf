#include "Buffer.h"

#include <cassert>

namespace Pdf
{
	Buffer::Buffer() : _value() {}
	Buffer::Buffer(const char *chars, int len) : _value(&chars[0], &chars[len - 1]) { assert(_value.size() > 0); }
	void Buffer::Reverse() { std::reverse(_value.begin(), _value.end()); }

	Character Buffer::At(long at) const { return _value[at]; }

	void Buffer::PushBack(const Character& ch) { _value.push_back(ch); }

	void Buffer::Insert(std::vector<Character>::const_iterator it, char* begin, char* end) { _value.insert(it, begin, end); }

	void Buffer::Insert(std::vector<Character>::const_iterator it, Buffer item) { _value.insert(it, item._value.begin(), item._value.end()); }

	long Buffer::Size() const { return _value.size(); }

	const Character* Buffer::Data() const { return _value.data(); }

	std::vector<Character>::const_iterator Buffer::begin() const { return _value.begin(); }

	std::vector<Character>::const_iterator Buffer::end() const { return _value.end(); }

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

	std::string Buffer::ToString(void) const { return std::string(_value.begin(), _value.end()); }
}
