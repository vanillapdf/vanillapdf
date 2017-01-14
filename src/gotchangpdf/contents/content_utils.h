#ifndef _CONTENT_UTILS_H
#define _CONTENT_UTILS_H

#include "syntax/utils/syntax_fwd.h"
#include "utils/util.h"

namespace gotchangpdf {
namespace contents {

class ContentUtils {
public:
	template <typename T>
	static T ConvertTo(const OperationBasePtr& obj) { return ConvertUtils<OperationBasePtr>::ConvertTo<T>(obj); }

	template <typename T>
	static bool IsType(const OperationBasePtr& obj) { return ConvertUtils<OperationBasePtr>::IsType<T>(obj); }
};

} // contents
} // gotchangpdf

#endif /* _CONTENT_UTILS_H */
