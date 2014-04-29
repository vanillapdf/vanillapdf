#include "BaseObjects/CharacterSet.h"

#include <cassert>

namespace Pdf
{
	CharacterSet::CharacterSet() : _value() {}
	CharacterSet::CharacterSet(const char *chars, int len) : _value(&chars[0], &chars[len - 1]) { assert(_value.size() > 0); }
	void CharacterSet::Reverse() { std::reverse(_value.begin(), _value.end()); }

	Character CharacterSet::At(long at) const { return _value[at]; }

	void CharacterSet::PushBack(Character& ch) { _value.push_back(ch); }
	void CharacterSet::PushBack(const Character& ch) { _value.push_back(ch); }

	void CharacterSet::Insert(std::vector<Character>::const_iterator it, char* begin, char* end)
	{
		_value.insert(it, begin, end);
	}

	void CharacterSet::Insert(std::vector<Character>::const_iterator it, CharacterSet item)
	{
		_value.insert(it, item._value.begin(), item._value.end());
	}

	long CharacterSet::Size() const
	{
		return _value.size();
	}

	const Character* CharacterSet::Data() const
	{
		return _value.data();
	}

	std::vector<Character>::const_iterator CharacterSet::begin() const
	{
		return _value.begin();
	}

	std::vector<Character>::const_iterator CharacterSet::end() const
	{
		return _value.end();
	}

	bool CharacterSet::operator==(const CharacterSet& other) const
	{
		return _value == other._value;
	}

	bool CharacterSet::operator==(const Character& other) const
	{
		return _value.size() == 1 && _value[0] == other;
	}

	bool CharacterSet::operator<(const CharacterSet& other) const
	{
		return _value < other._value;
	}

	Character CharacterSet::operator[](unsigned int i) const
	{
		return _value[i];
	}
}
