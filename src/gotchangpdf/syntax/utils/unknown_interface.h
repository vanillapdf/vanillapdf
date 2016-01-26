#ifndef _UNKNOWN_INTERFACE_H
#define _UNKNOWN_INTERFACE_H

#include <atomic>

namespace gotchangpdf
{
	class IUnknown
	{
	public:
		IUnknown() noexcept : m_ref_counter(0) {}
		IUnknown(const IUnknown&) noexcept : m_ref_counter(0) {}
		IUnknown& operator= (const IUnknown&) noexcept { return *this; }

		uint32_t UseCount() const noexcept { return m_ref_counter.load(); }
		void AddRef() const noexcept { m_ref_counter++; }
		void Release() const noexcept
		{
			if (--m_ref_counter == 0)
				delete this;
		}

		virtual ~IUnknown() = 0;

	private:
		mutable std::atomic_uint32_t m_ref_counter = 0;
	};

	inline IUnknown::~IUnknown() {}
}

#endif /* _UNKNOWN_INTERFACE_H */
