#ifndef _XREF_UTILS_H
#define _XREF_UTILS_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/files/xref.h"

namespace gotchangpdf {
namespace syntax {

class XrefUtils {
public:
	template <typename T>
	static T ConvertTo(const XrefEntryBasePtr& obj) {
		auto ptr = obj.get();
		auto converted = dynamic_cast<typename T::value_type *>(ptr);
		if (nullptr == converted) {
			throw ConversionExceptionFactory<T>::Construct(obj);
		}

		return T(converted);
	}

	template <typename T>
	static bool IsType(const XrefEntryBasePtr& obj) {
		auto ptr = obj.get();
		auto converted = dynamic_cast<typename T::value_type *>(ptr);
		return (nullptr != converted);
	}
};

} // syntax
} // gotchangpdf

#endif /* _XREF_UTILS_H */