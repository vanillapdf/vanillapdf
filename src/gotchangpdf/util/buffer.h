#ifndef _BUFFER_H
#define _BUFFER_H

#include "character.h"

#include <vector>
#include <string>

namespace gotchangpdf
{
	class Buffer : public std::vector<char>
	{
		typedef std::vector<char> MyBase;

	public:
		Buffer();
		Buffer(const Buffer & other);
		Buffer(Buffer && other);

		template <typename U>
		Buffer(const std::vector<U> & other);

		Buffer(const char * chars, int len);
		Buffer(const char *begin, const char *end);
		//Buffer(const std::vector<value_type>& data);

		//void Insert(int idx, const Buffer& item);
		//void Append(const Character& ch);
		//void Append(const Buffer& item);
		void reverse();

		std::string ToString(void) const;
	};

	template <typename U>
	Buffer::Buffer(const std::vector<U> & other)
		: MyBase(other.begin(), other.end()) {}

	inline std::string Buffer::ToString(void) const
	{
		return std::string(begin(), end());
	}

	inline void Buffer::reverse()
	{
		std::reverse(begin(), end());
	}
}

#endif /* _BUFFER_H */
