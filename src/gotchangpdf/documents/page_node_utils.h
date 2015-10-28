#ifndef _PAGE_NODE_UTILS_H
#define _PAGE_NODE_UTILS_H

#include "fwd.h"

#include <boost/variant/variant.hpp>
#include <boost/variant/static_visitor.hpp>

namespace gotchangpdf
{
	namespace documents
	{
		typedef boost::variant<
			PageObjectPtr,
			PageTreeNodePtr
		> PageNodePtr;

		PageNodePtr CreatePageNode(DictionaryObjectPtr obj);

		class PageNodeBaseVisitor : public boost::static_visitor<PageNodeBase*>
		{
		public:
			template <typename T>
			inline PageNodeBase* operator()(T& obj) const { return obj.Content.get(); }
		};

		template <typename T>
		class PageNodeVisitor : public boost::static_visitor<T>
		{
		public:
			inline T operator()(T& obj) const { return obj; }

			template <typename U>
			inline T operator()(const U&) const { throw exceptions::Exception("Type cast error"); }
		};
	}
}

#endif /* _PAGE_NODE_UTILS_H */
