#ifndef _BUFFER_H
#define _BUFFER_H

#include <vector>
#include <string>

namespace gotchangpdf
{
	class Buffer : public std::vector<char>
	{
	public:
		typedef std::vector<char> _Mybase;

		typedef _Mybase::value_type value_type;
		typedef _Mybase::size_type size_type;
		typedef _Mybase::difference_type difference_type;
		typedef _Mybase::pointer pointer;
		typedef _Mybase::const_pointer const_pointer;
		typedef _Mybase::reference reference;
		typedef _Mybase::const_reference const_reference;

	public:
		Buffer();
		Buffer(const Buffer & other);
		Buffer(Buffer && other);

		explicit Buffer(size_type count);

		template <typename U>
		Buffer(const std::vector<U> & other);

		Buffer(const value_type * chars, int len);
		Buffer(const value_type * begin, const value_type * end);
		//Buffer(const std::vector<value_type>& data);

		//void Insert(int idx, const Buffer& item);
		//void Append(const Character& ch);
		//void Append(const Buffer& item);

		bool Equals(const Buffer& other) const;

		std::string ToString(void) const;
	};

	template <typename U>
	Buffer::Buffer(const std::vector<U> & other)
		: _Mybase(other.begin(), other.end()) {}

	inline std::string Buffer::ToString(void) const
	{
		return std::string(begin(), end());
	}

	inline bool Buffer::Equals(const Buffer& other) const
	{
		return *this == other;
	}
}

#endif /* _BUFFER_H */
