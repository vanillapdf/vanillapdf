#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <exception>
#include <string>

namespace Pdf
{
	class Exception : public std::exception
	{
	public:
		Exception();
		explicit Exception(const char * const & msg);
		explicit Exception(const std::string& msg);
		Exception(const char * const & msg, int value);
		//Exception& operator=(const Exception& ex);
	};
}

#endif /* _EXCEPTION_H */
