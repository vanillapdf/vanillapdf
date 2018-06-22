#include "precompiled.h"

#include "utils/buffer_array.h"

namespace vanillapdf {

void BufferArray::Append(BufferPtr value) {
	m_data.push_back(value);
}

void BufferArray::Insert(BufferPtr value, size_type at) {
	m_data.insert(m_data.begin() + at, value);
}

bool BufferArray::Remove(size_type at) {
	auto item = m_data.begin() + at;
	if (item == m_data.end()) {
		return false;
	}

	m_data.erase(item);
	return true;
}

} // vanillapdf
