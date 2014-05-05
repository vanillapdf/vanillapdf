#ifndef _BUFFER_H
#define _BUFFER_H

#include "IBuffer.h"

#include <vector>
#include <string>

namespace Pdf
{
	class Buffer : public IBuffer
	{
	public:
		Buffer();
		Buffer(const char *chars, int len);
		Buffer(const char *begin, const char *end);

		void Insert(int idx, const Buffer& item);
		void PushBack(const Character& ch);
		void Reverse();

		std::string ToString(void) const;

		virtual bool operator==(const IBuffer& other) const override;
		virtual bool operator<(const IBuffer& other) const override;
		virtual Character operator[](unsigned int i) const override;
		virtual Character At(long at) const override;
		virtual int Size() const override;

		bool operator==(const Buffer& other) const;
		bool operator<(const Buffer& other) const;

	private:
		std::vector<Character> _value;
	};
}

#endif /* _BUFFER_H */
