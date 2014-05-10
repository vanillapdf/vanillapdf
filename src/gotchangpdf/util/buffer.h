#ifndef _BUFFER_H
#define _BUFFER_H

#include "Character.h"

#include <vector>
#include <string>

namespace gotchangpdf
{
	class Buffer
	{
	public:
		typedef char ValueType;

		Buffer();
		Buffer(const ValueType *chars, int len);
		Buffer(const ValueType *begin, const ValueType *end);

		void Insert(int idx, const Buffer& item);
		void Append(const Character& ch);
		void Append(const Buffer& item);
		void Reverse();

		ValueType* Data();
		const ValueType* Data() const;
		ValueType* Data(unsigned int idx);
		const ValueType* Data(unsigned int idx) const;

		std::string ToString(void) const;

		//virtual bool operator==(const IBuffer& other) const override;
		//virtual bool operator<(const IBuffer& other) const override;
		Character operator[](unsigned int i) const;
		Character At(long at) const;
		int Size() const;

		bool operator==(const Buffer& other) const;
		bool operator!=(const Buffer& other) const;
		bool operator<(const Buffer& other) const;

	private:
		std::vector<ValueType> _value;
	};
}

#endif /* _BUFFER_H */
