#ifndef _OUTPUT_POINTER_H
#define _OUTPUT_POINTER_H

#include "deferred.h"
#include "exception.h"
#include "util.h"

#include <type_traits>

namespace gotchangpdf
{
	template <typename T>
	class OutputPointer
	{
	public:
		OutputPointer() : m_value(reinterpret_cast<T*>(nullptr)) {}
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
			if (!other.empty()) {
				SetValue(other.GetValue());
				other.SetValue(nullptr);
			}
		}

		OutputPointer& operator=(const OutputPointer& other)
		{
			SetValue(other.GetValue());
			return *this;
		}

		OutputPointer& operator=(OutputPointer&& other)
		{
			SetValue(other.GetValue());
			other.SetValue(nullptr);
			return *this;
		}

		void SetValue(T* value) { m_value = value; }
		void SetValue(const T& value) { m_value = pdf_new T(value); }
		void SetValue(T&& value) { m_value = pdf_new T(value); }

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

		template <typename = std::enable_if_t<instantiation_of<Deferred, T>::value || std::is_base_of<Deferred<T::value_type>, T>::value>>
		typename T::value_type* AddRefGet(void)
		{
			if (nullptr == m_value)
				throw GeneralException("Uninitialized pointer");

			auto converted = dynamic_cast<typename T::value_type *>(m_value->AddRefGet());
			if (nullptr == m_value)
				throw syntax::ConversionExceptionFactory<Deferred<typename T::value_type>>::Construct(m_value);

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

		virtual ~OutputPointer() { if (m_value) delete m_value; }

	private:
		T* m_value = nullptr;
	};
}

#endif /* _OUTPUT_POINTER_H */