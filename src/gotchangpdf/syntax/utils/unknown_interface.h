#ifndef _UNKNOWN_INTERFACE_H
#define _UNKNOWN_INTERFACE_H

#include <atomic>

namespace gotchangpdf
{
	class IUnknown;

	class WeakReferenceCounter
	{
	public:
		WeakReferenceCounter(IUnknown* ptr) noexcept : m_active(true), m_reference(ptr) { assert(nullptr != ptr); }
		WeakReferenceCounter(const WeakReferenceCounter&) = delete;
		WeakReferenceCounter(WeakReferenceCounter&&) = delete;
		WeakReferenceCounter& operator= (const WeakReferenceCounter&) = delete;
		WeakReferenceCounter& operator= (WeakReferenceCounter&&) = delete;

		bool IsActive() const noexcept { return m_active.load(); }
		void Deactivate() noexcept { m_active = false; }
		IUnknown* GetReference() const noexcept { return m_reference; }

	private:
		std::atomic<bool> m_active;
		IUnknown* m_reference;
	};

	template <typename T>
	class WeakReference
	{
	public:
		// I actually cannot include Document in High level object
		// because HLO is required in Document dependencies
		// creating cycle. Therefore I have to disable this check
		// static_assert(std::is_base_of<IUnknown, T>::value,
		//	 "WeakReference<T>: T must be derived from IUnknown");

	public:
		WeakReference() = default;
		WeakReference(std::shared_ptr<WeakReferenceCounter> ptr) : m_ptr(ptr)
		{
			// Verify type correctness
			assert(nullptr != static_cast<T*>(m_ptr->GetReference()));
		}

		void Reset() noexcept { m_ptr.reset(); }
		bool IsEmpty() const noexcept { return (nullptr == m_ptr); }
		bool IsActive() const noexcept { return m_ptr ? m_ptr->IsActive() : false; }

		T* GetReference() const
		{
			if (!IsActive()) {
				throw GeneralException("Object has been already disposed");
			}

			assert(m_ptr && "Referenced pointer is empty");
			return m_ptr ? static_cast<T*>(m_ptr->GetReference()) : nullptr;
		}

		// The reason why value cannot be const is
		// because GetWeakReference cannot be const
		WeakReference& operator=(T& value)
		{
			*this = value.GetWeakReference<T>();
			return *this;
		}

	private:
		std::shared_ptr<WeakReferenceCounter> m_ptr;
	};

	class IUnknown
	{
	public:
		IUnknown() noexcept : m_ref_counter(0) {}
		IUnknown(const IUnknown&) noexcept : m_ref_counter(0) {}
		IUnknown& operator= (const IUnknown&) noexcept { return *this; }

		// GetWeakReference cannot be const, because
		// it creates WeakReferenceCounter on demand.
		// Otherwise WeakReferenceCounter would be allocated
		// in constructor even for objects
		// that do not have any weak references
		template <typename T>
		WeakReference<T> GetWeakReference()
		{
			if (!m_weak_ref) {
				m_weak_ref = std::make_shared<WeakReferenceCounter>(this);
			}

			return WeakReference<T>(m_weak_ref);
		}

		uint32_t UseCount() const noexcept { return m_ref_counter.load(); }
		void AddRef() noexcept { m_ref_counter++; }
		void Release() noexcept
		{
			if (--m_ref_counter == 0) {
				if (m_weak_ref) m_weak_ref->Deactivate();
				delete this;
			}
		}

		virtual ~IUnknown() = 0;

	private:
		std::atomic<uint32_t> m_ref_counter;
		std::shared_ptr<WeakReferenceCounter> m_weak_ref;
	};

	inline IUnknown::~IUnknown() {}
}

#endif /* _UNKNOWN_INTERFACE_H */
