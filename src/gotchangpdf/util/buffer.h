#ifndef _BUFFER_H
#define _BUFFER_H

#include "fwd.h"
#include "unknown_interface.h"

#include <vector>
#include <string>

namespace gotchangpdf
{
	class Buffer : public std::vector<char>, public IUnknown
	{
	public:
		typedef std::vector<char> base_type;

	public:
		Buffer() = default;
		Buffer(const Buffer& other) = default;
		Buffer(Buffer && other);
		explicit Buffer(size_type count);

		template <typename U>
		Buffer(const std::vector<U> & other) : _Mybase(other.begin(), other.end()) {}

		Buffer(const value_type * chars, int len);
		Buffer(const value_type * begin, const value_type * end);
		Buffer(size_type _Count, const value_type& _Val);

		template<class _Iter,
		class = typename std::enable_if<std::_Is_iterator<_Iter>::value,
			void>::type>
			Buffer(_Iter begin, _Iter end) : base_type(begin, end) {}

		virtual ~Buffer() = default;
		//Buffer(const std::vector<value_type>& data);

		bool Equals(Buffer other) const { return *this == other; }

		std::string ToString(void) const { return std::string(begin(), end()); }
	};
}

#endif /* _BUFFER_H */
