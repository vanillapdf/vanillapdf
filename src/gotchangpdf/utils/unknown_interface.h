#ifndef _UNKNOWN_INTERFACE_H
#define _UNKNOWN_INTERFACE_H

#include <memory>
#include <atomic>

namespace gotchangpdf {

class IUnknown;

class WeakReferenceCounter {
public:
	WeakReferenceCounter(IUnknown* ptr) noexcept;
	WeakReferenceCounter(const WeakReferenceCounter&) = delete;
	WeakReferenceCounter(WeakReferenceCounter&&) = delete;
	WeakReferenceCounter& operator= (const WeakReferenceCounter&) = delete;
	WeakReferenceCounter& operator= (WeakReferenceCounter&&) = delete;

	bool IsActive() const noexcept;
	void Deactivate() noexcept;
	IUnknown* GetReference() const noexcept;

private:
	std::atomic<bool> m_active;
	IUnknown* m_reference;
};

template <typename T>
class WeakReference {
public:
	// I actually cannot include Document in High level object
	// because HLO is required in Document dependencies
	// creating cycle. Therefore I have to disable this check
	// static_assert(std::is_base_of<IUnknown, T>::value,
	//	 "WeakReference<T>: T must be derived from IUnknown");

public:
	WeakReference() = default;
	WeakReference(std::shared_ptr<WeakReferenceCounter> ptr) : m_ptr(ptr) {
		// Verify type correctness
		assert(nullptr != static_cast<T*>(m_ptr->GetReference()));
	}

	void Reset() noexcept {
		m_ptr.reset();
	}

	bool IsEmpty() const noexcept {
		return (nullptr == m_ptr);
	}

	bool IsActive() const noexcept {
		return (m_ptr ? m_ptr->IsActive() : false);
	}

	Deferred<T> GetReference() const {
		if (!IsActive()) {
			throw GeneralException("Object has been already disposed");
		}

		assert(m_ptr && "Referenced pointer is empty");
		auto result = (m_ptr ? static_cast<T*>(m_ptr->GetReference()) : nullptr);
		return Deferred<T>(result);
	}

	//// The reason why value cannot be const is
	//// because GetWeakReference cannot be const
	//WeakReference& operator=(T& value) {
	//	*this = value.template GetWeakReference<T>();
	//	return *this;
	//}

private:
	std::shared_ptr<WeakReferenceCounter> m_ptr;
};

class IUnknown {
public:
	IUnknown() noexcept;
	IUnknown(const IUnknown&) noexcept;
	IUnknown& operator= (const IUnknown&) noexcept;

	// GetWeakReference cannot be const, because
	// it creates WeakReferenceCounter on demand.
	// Otherwise WeakReferenceCounter would be allocated
	// in constructor even for objects
	// that do not have any weak references
	template <typename T>
	WeakReference<T> GetWeakReference() {
		if (!m_weak_ref) {
			m_weak_ref = std::make_shared<WeakReferenceCounter>(this);
		}

		return WeakReference<T>(m_weak_ref);
	}

	uint32_t UseCount() const noexcept;

	void AddRef() noexcept;
	void Release() noexcept;

	virtual ~IUnknown() = 0;

private:
	std::atomic<uint32_t> m_ref_counter;
	std::shared_ptr<WeakReferenceCounter> m_weak_ref;
};

template<typename T>
class IUnknownDeleter {
	static_assert(std::is_base_of<IUnknown, T>::value,
		"Only types derived from IUnknown are supported");

	void operator()(T* ptr) const noexcept {
		ptr->Release();
	}
};

inline IUnknown::~IUnknown() {}

} // gotchangpdf

#endif /* _UNKNOWN_INTERFACE_H */
