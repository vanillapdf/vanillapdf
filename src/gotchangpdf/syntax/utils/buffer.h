#ifndef _BUFFER_H
#define _BUFFER_H

#include "syntax_fwd.h"
#include "unknown_interface.h"

#include <vector>
#include <string>
#include <sstream>
#include <cassert>

namespace gotchangpdf
{
	class Buffer : public IUnknown, public ::std::vector<char>
	{
	public:
		using base_type = std::vector<char>;
		using base_type::base_type;

	public:
		Buffer() = default;
		explicit Buffer(size_type count);
		explicit Buffer(const char * chars);
		Buffer(const value_type * begin, const value_type * end);
		Buffer(size_type count, const value_type& val);

		template <typename T, typename = std::enable_if<sizeof(T) == sizeof(value_type)>>
		Buffer(const T * chars, int len) : base_type(
			reinterpret_cast<const value_type *>(&chars[0]),
			reinterpret_cast<const value_type *>(&chars[len])
		) { assert(size() > 0); }

		std::string ToString(void) const { return std::string(begin(), end()); }
		std::stringstream ToStringStream(void) const { return std::stringstream(ToString()); }
		bool Equals(const Buffer& other) const;

		virtual ~Buffer() {}
	};

	inline bool operator==(const Buffer& left, const Buffer& right) { return left.Equals(right); }
	inline bool operator!=(const Buffer& left, const Buffer& right) { return !left.Equals(right); }

	bool operator==(const char * left, const Buffer& right);
	bool operator==(const Buffer& left, const char * right);
	bool operator!=(const char * left, const Buffer& right);
	bool operator!=(const Buffer& left, const char * right);
}

#endif /* _BUFFER_H */
