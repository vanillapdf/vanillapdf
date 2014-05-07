#include "exception.h"

namespace gotchangpdf
{
	namespace exceptions
	{
		Exception::Exception(const char * const & msg) : exception(msg) {}
		Exception::Exception(const char * const & msg, int value) : exception(msg, value) {}
		Exception::Exception() : exception() {}
		Exception::Exception(const std::string& msg) : exception(msg.c_str()) {}

		/*
		Exception& Exception::operator=(const Exception& ex)
		{
		return *this = ex;
		}
		*/
	}
}

