#ifndef _GENERAL_VISITORS_H
#define _GENERAL_VISITORS_H

#include "dictionary_object.h"
#include "array_object.h"

#include <memory>

#include <boost/variant/static_visitor.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		template <typename T>
		class BaseVisitor : public boost::static_visitor<T>
		{
		public:
			template <typename U>
			inline T operator()(const Deferred<U>& obj) const { return obj.Content.get(); }
		};

		template <typename T>
		class BaseAddRefVisitor : public boost::static_visitor<T>
		{
		public:
			template <typename U>
			inline T operator()(const Deferred<U>& obj) const
			{
				auto result = obj.Content.get();
				result->AddRef();
				return result;
			}
		};
	}
}

#endif /* _GENERAL_VISITORS_H */
