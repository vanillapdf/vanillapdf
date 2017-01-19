#ifndef _OUTPUT_POINTER_H
#define _OUTPUT_POINTER_H

#include "utils/deferred.h"
#include "utils/unknown_interface.h"
#include "utils/util.h"

#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/objects/object.h"

#include <type_traits>
#include <memory>

namespace gotchangpdf {
namespace syntax {

template <typename T>
class OutputPointer {
public:
	static_assert(instantiation_of<Deferred, T>::value ||
		std::is_base_of<syntax::Object, typename T::value_type>::value,
		"Output pointer requires template parameter to be either Deferred instance or derived from Object");

public:
	OutputPointer() = default;
	OutputPointer(const OutputPointer& other) = default;
	OutputPointer(OutputPointer&& other) = default;
	OutputPointer& operator=(const OutputPointer& other) = default;
	OutputPointer& operator=(OutputPointer&& other) = default;

	OutputPointer(T* value) : m_value(value) {}
	OutputPointer(const T& value) { SetValue(value); }
	OutputPointer(T&& value) { SetValue(value); }

	void SetValue(const T& value) { m_value.reset(pdf_new T(value)); }
	void SetValue(T&& value) { m_value.reset(pdf_new T(value)); }

	T GetValue() const {
		if (nullptr == m_value) {
			throw GeneralException("Uninitialized pointer");
		}

		return *m_value;
	}

	bool empty() const noexcept { return (nullptr == m_value); }
	operator T() { return GetValue(); }
	operator T() const { return GetValue(); }
	T operator*() const { return *GetValue(); }

	typename T::value_type* AddRefGet(void) {
		if (nullptr == m_value) {
			throw GeneralException("Uninitialized pointer");
		}

		auto converted = static_cast<typename T::value_type *>(m_value->AddRefGet());
		if (nullptr == m_value) {
			throw ConversionExceptionFactory<Deferred<typename T::value_type>>::Construct(m_value);
		}

		return converted;
	}

	OutputPointer& operator=(const T& value) {
		SetValue(value);
		return *this;
	}

	OutputPointer& operator=(T&& value) {
		SetValue(value);
		return *this;
	}

private:
	std::unique_ptr<T> m_value = nullptr;
};

} // syntax
} // gotchangpdf

#endif /* _OUTPUT_POINTER_H */