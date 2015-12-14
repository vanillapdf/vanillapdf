#ifndef _CONTENT_UTILS_H
#define _CONTENT_UTILS_H

#include "semantics_fwd.h"
#include "content_stream_operation_generic.h"

#include "util.h"

namespace gotchangpdf
{
	namespace semantics
	{
		namespace contents
		{
			class ContentUtils
			{
			public:
				static syntax::contents::OperationBasePtr ConvertGenericOperation(syntax::contents::OperationGenericPtr obj);

				template <typename T>
				static T ConvertTo(const syntax::contents::OperationBasePtr& obj) { return ConvertUtils<syntax::contents::OperationBasePtr>::ConvertTo<T>(obj); }

				template <typename T>
				static bool IsType(const syntax::contents::OperationBasePtr& obj) { return ConvertUtils<syntax::contents::OperationBasePtr>::IsType<T>(obj); }
			};
		}
	}
}

#endif /* _CONTENT_UTILS_H */
