#ifndef _OUTPUT_POINTER_H
#define _OUTPUT_POINTER_H

#include "utils/deferred.h"
#include "utils/unknown_interface.h"

#include "syntax/objects/object.h"

#include <type_traits>
#include <memory>

namespace vanillapdf {
namespace syntax {

template <typename T>
class OutputPointer {
public:
	static_assert(
		instantiation_of<Deferred, T>::value ||
		instantiation_of<DeferredContainer, T>::value ||
		instantiation_of<DeferredIterator, T>::value ||
		std::is_base_of<syntax::Object, typename T::deferred_ptr_type>::value,
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

	T* GetValue() const {
		if (m_value == nullptr) {
			throw GeneralException("Uninitialized pointer");
		}

		return m_value.get();
	}

	bool empty() const noexcept { return (nullptr == m_value); }
	operator T&() { return *GetValue(); }
	operator const T&() const { return *GetValue(); }

	T& operator*() { return *GetValue(); }
	const T& operator*() const { return *GetValue(); }

	T& operator->() { return *GetValue(); }
	const T& operator->() const { return *GetValue(); }

	auto AddRefGet(void) -> typename std::result_of<decltype(&T::AddRefGet)(T)>::type {
		auto value = GetValue();
		return value->AddRefGet();
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
	// Switched to shared pointer because
	// unique pointer is missing copy constructor
	std::shared_ptr<T> m_value = nullptr;
};

} // syntax
} // vanillapdf

#endif /* _OUTPUT_POINTER_H */