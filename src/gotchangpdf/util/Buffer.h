#ifndef _BUFFER_H
#define _BUFFER_H

#include "IBuffer.h"

#include <vector>
#include <string>

namespace gotchangpdf
{
	class Buffer : public IBuffer
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

		virtual bool operator==(const IBuffer& other) const override;
		virtual bool operator<(const IBuffer& other) const override;
		virtual Character operator[](unsigned int i) const override;
		virtual Character At(long at) const override;
		virtual int Size() const override;

		bool operator==(const Buffer& other) const;
		bool operator<(const Buffer& other) const;

	private:
		std::vector<ValueType> _value;
	};
}

#endif /* _BUFFER_H */
