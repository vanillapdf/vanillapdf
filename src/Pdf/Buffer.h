#ifndef _BUFFER_H
#define _BUFFER_H

#include "IBuffer.h"

#include <vector>

namespace Pdf
{
	class Buffer : public IBuffer
	{
	public:
		Buffer();
		Buffer(const char *chars, int len);

		void Buffer::Insert(std::vector<Character>::const_iterator it, char* begin, char* end);
		void Insert(std::vector<Character>::const_iterator it, Buffer item);
		void PushBack(Character& ch);
		void PushBack(const Character& ch);
		void Reverse();
		virtual long Size() const override;
		const Character* Data() const;

		std::vector<Character>::const_iterator begin() const;
		std::vector<Character>::const_iterator end() const;

		virtual bool operator==(const IBuffer& other) const override;
		virtual bool operator<(const IBuffer& other) const override;
		virtual Character operator[](unsigned int i) const override;
		virtual Character At(long at) const override;

		bool operator==(const Buffer& other) const;
		bool operator<(const Buffer& other) const;

	private:
		std::vector<Character> _value;
	};
}

#endif /* _BUFFER_H */
