#ifndef _QI_COMMON_H
#define _QI_COMMON_H

#include "file_position.h"
#include "offset_iterator.h"

namespace gotchangpdf
{
	namespace lexical
	{
		typedef boost::spirit::istream_iterator base_iterator_type;
		typedef offset_iterator<base_iterator_type, pdf_position> pos_iterator_type;
	}
}

#endif /* _QI_COMMON_H */
