#include "Buffer.h"

#include <cassert>

namespace gotchangpdf
{
	#pragma region Constructors

	Buffer::Buffer() : _value() {}
	Buffer::Buffer(const ValueType *chars, int len) : _value(&chars[0], &chars[len - 1]) { assert(_value.size() > 0); }
	Buffer::Buffer(const ValueType *begin, const ValueType *end) : _value(begin, end) { assert(_value.size() > 0); }

	#pragma endregion

	#pragma region Operators
	/*
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
	*/
	bool Buffer::operator==(const Buffer& other) const { return _value == other._value; }
	bool Buffer::operator!=(const Buffer& other) const { return _value != other._value; }
	bool Buffer::operator<(const Buffer& other) const { return _value < other._value; }

	Character Buffer::operator[](unsigned int i) const { return Character(_value[i]); }

	#pragma endregion

	void Buffer::Reverse() { std::reverse(_value.begin(), _value.end()); }
	Character Buffer::At(long at) const { return Character(_value[at]); }
	void Buffer::Append(const Character& ch) { _value.push_back(ch); }
	void Buffer::Append(const Buffer& item) { Insert(0, item); }
	void Buffer::Insert(int idx, const Buffer& item) { _value.insert(_value.begin() + idx, item._value.begin(), item._value.end()); }
	int Buffer::Size() const { return _value.size(); }
	std::string Buffer::ToString(void) const { return std::string(_value.begin(), _value.end()); }
	Buffer::ValueType* Buffer::Data() { return _value.data(); }
	const Buffer::ValueType* Buffer::Data() const { return _value.data(); }
	Buffer::ValueType* Buffer::Data(unsigned int idx) { return _value.data() + sizeof(ValueType) * idx; }
	const Buffer::ValueType* Buffer::Data(unsigned int idx) const { return _value.data() + sizeof(ValueType) * idx; }

	#pragma region DllInterface

	bool IBuffer::operator==(const IBuffer& other) const
	{
		auto removed = const_cast<IBuffer*>(this);
		auto obj = reinterpret_cast<Buffer*>(removed);
		auto other_parsed = reinterpret_cast<const Buffer*>(&other);

		return *obj == *other_parsed;
	}

	bool IBuffer::operator!=(const IBuffer& other) const
	{
		auto removed = const_cast<IBuffer*>(this);
		auto obj = reinterpret_cast<Buffer*>(removed);
		auto other_parsed = reinterpret_cast<const Buffer*>(&other);

		return *obj != *other_parsed;
	}

	bool IBuffer::operator<(const IBuffer& other) const
	{
		auto removed = const_cast<IBuffer*>(this);
		auto obj = reinterpret_cast<Buffer*>(removed);
		auto other_parsed = reinterpret_cast<const Buffer*>(&other);

		return *obj < *other_parsed;
	}

	IBuffer::~IBuffer() {}
	/*
	Character Buffer::operator[](unsigned int i) const { return Character(_value[i]); }

	void Buffer::Reverse() { std::reverse(_value.begin(), _value.end()); }
	Character Buffer::At(long at) const { return Character(_value[at]); }
	void Buffer::Append(const Character& ch) { _value.push_back(ch); }
	void Buffer::Append(const Buffer& item) { Insert(0, item); }
	void Buffer::Insert(int idx, const Buffer& item) { _value.insert(_value.begin() + idx, item._value.begin(), item._value.end()); }
	int Buffer::Size() const { return _value.size(); }
	Buffer::ValueType* Buffer::Data() { return _value.data(); }
	const Buffer::ValueType* Buffer::Data() const { return _value.data(); }
	Buffer::ValueType* Buffer::Data(unsigned int idx) { return _value.data() + sizeof(ValueType)* idx; }
	const Buffer::ValueType* Buffer::Data(unsigned int idx) const { return _value.data() + sizeof(ValueType)* idx; }
	*/

	#pragma endregion
}
