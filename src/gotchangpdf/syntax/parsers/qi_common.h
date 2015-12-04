#ifndef _QI_COMMON_H
#define _QI_COMMON_H

#include "file_position.h"
#include "offset_iterator.h"

#include <boost/spirit/home/support/iterators/istream_iterator.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		typedef boost::spirit::istream_iterator base_iterator_type;
		typedef offset_iterator<base_iterator_type, pdf_position> pos_iterator_type;

		template <typename From, typename To>
		void convert(Deferred<From>& from, Deferred<To>& to)
		{
			auto base = from.Content.get();
			auto ptr = static_cast<To*>(base);
			to = Deferred<To>(ptr);
		}
	}
}

#endif /* _QI_COMMON_H */
