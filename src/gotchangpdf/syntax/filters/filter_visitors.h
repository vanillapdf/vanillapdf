#ifndef _FILTER_VISITORS_H
#define _FILTER_VISITORS_H

#include "filter.h"

#include <boost/variant/static_visitor.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		class FilterBaseVisitor : public boost::static_visitor<Filter*>
		{
		public:
			template <typename T>
			inline Filter* operator()(T& obj) const { return obj.Content.get(); }
		};

		template <typename T>
		class FilterVisitor : public boost::static_visitor<T>
		{
		public:
			inline T operator()(T& obj) const { return obj; }

			template <typename U>
			inline T operator()(const U&) const { throw Exception("Type cast error"); }
		};
	}
}

#endif /* _FILTER_VISITORS_H */
