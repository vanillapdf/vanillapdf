#ifndef _OBJECT_VISITORS_H
#define _OBJECT_VISITORS_H

#include "object.h"
#include "direct_object.h"
#include "exception.h"

#include <boost/variant/static_visitor.hpp>

namespace gotchangpdf
{
	class ObjectBaseVisitor : public boost::static_visitor<ObjectPtr>
	{
	public:
		template <typename T>
		ObjectPtr operator()(T obj) const
		{
			return obj;
		}

		ObjectPtr operator()(StringObjectPtr obj) const
		{
			return obj.apply_visitor(*this);
		}
	};

	template <typename T>
	class ObjectVisitor : public boost::static_visitor<T>
	{
	public:
		T operator()(T obj) const
		{
			return obj;
		}

		template <typename U>
		T operator()(U obj) const
		{
			throw exceptions::Exception("Type cast error");
		}
	};
}

#endif /* _OBJECT_VISITORS_H */
