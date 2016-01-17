#ifndef _BUFFER_H
#define _BUFFER_H

#include "syntax_fwd.h"
#include "unknown_interface.h"

#include <vector>
#include <string>
#include <sstream>

namespace gotchangpdf
{
	class Buffer : public IUnknown, public std::vector<char>
	{
	public:
		using base_type = std::vector<char>;
		using base_type::base_type;

	public:
		Buffer() = default;
		explicit Buffer(size_type count);
		explicit Buffer(const value_type * chars);
		Buffer(const value_type * chars, int len);
		Buffer(const value_type * begin, const value_type * end);
		Buffer(size_type count, const value_type& val);

		std::string ToString(void) const { return std::string(begin(), end()); }
		std::stringstream ToStringStream(void) const { return std::stringstream(ToString()); }
		bool Equals(const Buffer& other) const;

		virtual ~Buffer() {}
	};
}

#endif /* _BUFFER_H */
