#ifndef _OUTPUT_POINTER_H
#define _OUTPUT_POINTER_H

#include "deferred.h"
#include "exception.h"
#include "util.h"

#include <type_traits>
#include <memory>

namespace gotchangpdf
{
	template <typename T>
	class OutputPointer
	{
	public:
		OutputPointer() : m_value(nullptr) {}
		OutputPointer(T* value) : m_value(value) {}
		OutputPointer(const T& value) { SetValue(value); }
		OutputPointer(T&& value) { SetValue(value); }

		OutputPointer(const OutputPointer& other)
		{
			if (!other.empty()) {
				SetValue(other.GetValue());
			}
		}

		OutputPointer(OutputPointer&& other)
		{
			m_value = std::move(other.m_value);
		}

		OutputPointer& operator=(const OutputPointer& other)
		{
			if (!other.empty()) {
				SetValue(other.GetValue());
			}

			return *this;
		}

		OutputPointer& operator=(OutputPointer&& other)
		{
			m_value = std::move(other.m_value);
			return *this;
		}

		void SetValue(const T& value) { m_value.reset(pdf_new T(value)); }
		void SetValue(T&& value) { m_value.reset(pdf_new T(value)); }

		T GetValue() const
		{
			if (nullptr == m_value)
				throw GeneralException("Uninitialized pointer");

			return *m_value;
		}

		bool empty() const noexcept { return (nullptr == m_value); }
		operator T() { return GetValue(); }
		operator T() const { return GetValue(); }
		T operator*() const { return *GetValue(); }

		template <
			typename = typename std::enable_if<instantiation_of<Deferred, T>::value ||
			std::is_base_of<Deferred<typename T::value_type>, T>::value>::type
		>
		typename T::value_type* AddRefGet(void)
		{
			if (nullptr == m_value)
				throw GeneralException("Uninitialized pointer");

			auto converted = dynamic_cast<typename T::value_type *>(m_value->AddRefGet());
			if (nullptr == m_value)
				throw ConversionExceptionFactory<Deferred<typename T::value_type>>::Construct(m_value);

			return converted;
		}

		OutputPointer& operator=(const T& value)
		{
			SetValue(value);
			return *this;
		}

		OutputPointer& operator=(T&& value)
		{
			SetValue(value);
			return *this;
		}

	private:
		std::unique_ptr<T> m_value = nullptr;
	};
}

#endif /* _OUTPUT_POINTER_H */