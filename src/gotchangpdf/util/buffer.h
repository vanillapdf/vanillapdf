#ifndef _BUFFER_H
#define _BUFFER_H

#include "fwd.h"
#include "unknown_interface.h"

#include <vector>
#include <string>
#include <sstream>

namespace gotchangpdf
{
	class Buffer : public IUnknown
	{
	public:
		typedef std::vector<char> base_type;
		typedef base_type::value_type value_type;
		typedef base_type::size_type size_type;
		typedef base_type::difference_type difference_type;
		typedef base_type::pointer pointer;
		typedef base_type::const_pointer const_pointer;
		typedef base_type::reference reference;
		typedef base_type::const_reference const_reference;

		typedef base_type::iterator iterator;
		typedef base_type::const_iterator const_iterator;

	public:
		Buffer() = default;
		explicit Buffer(size_type count);
		explicit Buffer(const value_type * chars);
		Buffer(const value_type * chars, int len);
		Buffer(const value_type * begin, const value_type * end);
		Buffer(size_type count, const value_type& val);

		std::string ToString(void) const { return std::string(_value.begin(), _value.end()); }
		std::stringstream ToStringStream(void) const { return std::stringstream(ToString()); }

		iterator begin() _NOEXCEPT { return _value.begin(); }
		const_iterator begin() const _NOEXCEPT { return _value.begin(); }
		iterator end() _NOEXCEPT { return _value.end(); }
		const_iterator end() const _NOEXCEPT { return _value.end(); }
		reference front() { return _value.front(); }
		const_reference front() const { return _value.front(); }

		iterator insert(const_iterator wher, const value_type& val) { return _value.insert(wher, val); }
		void push_back(const value_type& val) { _value.push_back(val); }
		void resize(size_type newsize) { _value.resize(newsize); }

		size_type size() const _NOEXCEPT { return _value.size(); }
		bool empty() const _NOEXCEPT { return _value.empty(); }
		pointer data() _NOEXCEPT { return _value.data(); }
		const_pointer data() const _NOEXCEPT { return _value.data(); }

		reference operator[](size_type i) { return _value[i]; }
		const_reference operator[](size_type i) const { return _value[i]; }

		inline bool Equals(Buffer other) const { return _value == other._value; }
		inline bool operator==(const Buffer& other) const { return Equals(other); }
		inline bool operator!=(const Buffer& other) const { return !Equals(other); }
		inline bool operator<(const Buffer& other) const { return _value < other._value; }

		template<class _Iter>
		typename std::enable_if<std::_Is_iterator<_Iter>::value, iterator>::type
			insert(const_iterator wher, _Iter first, _Iter last) { return _value.insert(wher, first, last); }

		//template<class _Iter,
		//class = typename std::enable_if<std::_Is_iterator<_Iter>::value,
		//	void>::type>
		//	Buffer(_Iter begin, _Iter end) : _value(begin, end) {}

		//template <typename U>
		//Buffer(const std::vector<U> & other) : _Mybase(other.begin(), other.end()) {}

		virtual ~Buffer() {}

	private:
		std::vector<value_type> _value;
	};
}

#endif /* _BUFFER_H */
