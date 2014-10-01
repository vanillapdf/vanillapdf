#include "Buffer.h"

#include <cassert>

namespace gotchangpdf
{
	#pragma region Constructors

	Buffer::Buffer() {}
	Buffer::Buffer(const Buffer & other) : MyBase(other) {}
	Buffer::Buffer(Buffer && other) : MyBase(std::move(other)) {}
	Buffer::Buffer(const char * chars, int len) : MyBase(&chars[0], &chars[len - 1]) { assert(size() > 0); }
	Buffer::Buffer(const char * begin, const char * end) : MyBase(begin, end) { assert(size() > 0); }
	//Buffer::Buffer(const std::vector<ValueType>& data) : _value(data) { assert(_value.size() > 0); }

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

	//Buffer::ValueType Buffer::operator[](unsigned int i) const { return _value[i]; }

	#pragma endregion

	
	//Buffer::ValueType Buffer::At(long at) const { return _value[at]; }
	//void Buffer::Append(const Character& ch) { _value.push_back(ch); }
	//void Buffer::Append(const Buffer& item) { Insert(0, item); }
	//void Buffer::Insert(int idx, const Buffer& item) { _value.insert(_value.begin() + idx, item._value.begin(), item._value.end()); }
	//int Buffer::Size() const { return _value.size(); }
	//Buffer::ValueType* Buffer::Data() { return _value.data(); }
	//const Buffer::ValueType* Buffer::Data() const { return _value.data(); }
	//Buffer::ValueType* Buffer::Data(unsigned int idx) { return _value.data() + sizeof(ValueType) * idx; }
	//const Buffer::ValueType* Buffer::Data(unsigned int idx) const { return _value.data() + sizeof(ValueType) * idx; }
}
