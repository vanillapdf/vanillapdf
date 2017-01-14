#ifndef _PAGE_NODE_UTILS_H
#define _PAGE_NODE_UTILS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/utils/semantic_exceptions.h"

namespace gotchangpdf
{
	namespace semantics
	{
		PageNodeBasePtr CreatePageNode(syntax::DictionaryObjectPtr obj);

		class PageNodeUtils
		{
		public:
			template <typename T>
			static T ConvertTo(const PageNodeBasePtr& obj)
			{
				auto ptr = obj.get();
				auto converted = dynamic_cast<typename T::value_type *>(ptr);
				if (nullptr == converted)
					throw ConversionExceptionFactory<T>::Construct(obj);

				return T(converted);
			}

			template <typename T>
			static bool IsType(const PageNodeBasePtr& obj)
			{
				auto ptr = obj.get();
				auto converted = dynamic_cast<typename T::value_type *>(ptr);
				return (nullptr != converted);
			}
		};
	}
}

#endif /* _PAGE_NODE_UTILS_H */
