#ifndef _UNKNOWN_INTERFACE_H
#define _UNKNOWN_INTERFACE_H

#include "utils/exceptions.h"
#include "utils/log.h"

#include <memory>
#include <atomic>
#include <cassert>
#include <optional>

namespace vanillapdf {

class IUnknown;

class WeakReferenceCounter {
public:
    WeakReferenceCounter() noexcept;
    WeakReferenceCounter(const WeakReferenceCounter&) = delete;
    WeakReferenceCounter(WeakReferenceCounter&&) = delete;
    WeakReferenceCounter& operator= (const WeakReferenceCounter&) = delete;
    WeakReferenceCounter& operator= (WeakReferenceCounter&&) = delete;

    bool IsActive() const noexcept;
    void Deactivate() noexcept;

private:
    std::atomic<bool> m_active;
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
    WeakReference() noexcept = default;

    WeakReference(T* ptr, std::shared_ptr<WeakReferenceCounter> counter) noexcept
        : m_ptr(ptr), m_counter(counter) {
        assert(ptr != nullptr && "Empty object passed to weak reference");
    }

    size_t Hash() const {
        assert(m_ptr && "Trying to hash nullptr");
        return reinterpret_cast<size_t>(m_ptr);
    }

    void Reset() noexcept {
        m_counter.reset();
    }

    bool Identity(const WeakReference<T>& other) const noexcept {
        if (m_ptr == nullptr) {
            return false;
        }

        return (m_ptr == other.m_ptr);
    }

    bool Identity(const T* other) const noexcept {
        if (m_ptr == nullptr) {
            return false;
        }

        return (m_ptr == other);
    }

    bool IsEmpty() const noexcept {
        return (m_ptr == nullptr);
    }

    bool IsActive() const noexcept {
        return (m_counter ? m_counter->IsActive() : false);
    }

    // Upgrade to a strong reference in a single step, reporting failure instead
    // of throwing. Prefer this over the IsActive() + GetReference() pair, which
    // leaves a window for the object to reach the end of its life in between.
    //
    // The single step is still not self-sufficient: TryAddRef operates on the
    // object's own storage, so this call is only safe under the external
    // liveness guarantee described in the contract on IUnknown::TryAddRef.
    std::optional<Deferred<T>> TryGetReference() const {
        if (m_ptr == nullptr || !IsActive() || !m_ptr->TryAddRef()) {
            return std::nullopt;
        }

        // Adopt the reference TryAddRef has already taken
        return Deferred<T>(m_ptr, false);
    }

    Deferred<T> GetReference() const {
        if (IsEmpty()) {
            LOG_ERROR_AND_THROW(InvalidParameterException, "Pointer object was not set");
        }

        auto acquired = TryGetReference();
        if (!acquired.has_value()) {
            LOG_ERROR_AND_THROW(InvalidParameterException, "Object has been already disposed");
        }

        return acquired.value();
    }

    //// The reason why value cannot be const is
    //// because GetWeakReference cannot be const
    //WeakReference& operator=(T& value) {
    //	*this = value.template GetWeakReference<T>();
    //	return *this;
    //}

private:
    // I had to change the layout a little bit.
    // Previously the counter had both active flag
    // and the referenced pointer itself.
    // Unfortunately, I was not able to keep the type
    // information about the pointer.

    // Imagine you have a single object, while having
    // 2 weak pointers to the derived types, which have
    // to share the counter.

    // ----------------- BLEH -------------------------
    // The other solution was run-time checking of
    // type and the dynamic_cast-ing to the correct type
    // ----------------- BLEH -------------------------

    // MSVC shared pointers have similar layout, that
    // the counter and pointers are distinct objects.
    // I have found this solution to suit the best.

    std::shared_ptr<WeakReferenceCounter> m_counter;
    T* m_ptr = nullptr;
};

class IUnknown {
public:
    IUnknown() noexcept;
    IUnknown(const IUnknown&) noexcept;
    IUnknown& operator= (const IUnknown&) noexcept;

    /*
    * clang warning: explicitly defaulted move constructor is implicitly deleted
    * note: move constructor of 'IUnknown' is implicitly deleted because field 'm_ref_counter' has a deleted move constructor
    * note: copy constructor of 'atomic<unsigned int>' is implicitly deleted because base class '__atomic_base<unsigned int>' has a deleted copy constructor
    * note: copy constructor of '__atomic_base<unsigned int, true>' is implicitly deleted because base class '__atomic_base<unsigned int, false>' has a deleted copy constructor
    * note: '__atomic_base' has been explicitly marked deleted here
    * __atomic_base(const __atomic_base&) = delete;
    * 
    * https://learn.microsoft.com/en-us/cpp/cpp/explicitly-defaulted-and-deleted-functions?view=msvc-170
    * 
    * If any constructor is explicitly declared, then no default constructor is automatically generated.
    * If a virtual destructor is explicitly declared, then no default destructor is automatically generated.
    * If a move constructor or move-assignment operator is explicitly declared, then:
    * No copy constructor is automatically generated.
    * No copy-assignment operator is automatically generated.
    * If a copy constructor, copy-assignment operator, move constructor, move-assignment operator, or destructor is explicitly declared, then:
    * No move constructor is automatically generated.
    * No move-assignment operator is automatically generated.
    */

    //IUnknown(IUnknown&&) noexcept = default;
    //IUnknown& operator= (IUnknown&&) noexcept = default;

    uint32_t UseCount() const noexcept;

    void AddRef() noexcept;
    void Release() noexcept;

    // Raise the counter only if it has not reached zero yet. At zero the object
    // has committed to destruction and reviving it would create a second owner
    // of storage that is already being torn down.
    //
    // CONTRACT: the counter lives inside the object, so the caller must
    // guarantee the storage stays allocated for the whole call - against an
    // unsynchronized final Release the compare-exchange itself reads freed
    // memory, and if the allocator has already recycled the storage it can
    // even succeed against a foreign object's bytes. Upgrade only under a lock
    // that the destructor acquires before the storage can be freed; the
    // document registry in SemanticUtils is the canonical pattern (~Document
    // takes the registry lock as its first statement, and every upgrade stays
    // inside that lock). Removing this precondition by pinning the storage
    // from the weak reference control block is tracked in
    // https://github.com/vanillapdf/vanillapdf/issues/505.
    bool TryAddRef() noexcept;

    virtual ~IUnknown() = 0;

    // Object diagnostics - thread-safe counters for tracking live objects
    static int64_t ActiveObjectCount() noexcept;
    static int64_t PeakObjectCount() noexcept;
    static int64_t TotalObjectsCreated() noexcept;
    static void ResetDiagnosticCounters() noexcept;

    // Since we moved IUnknown to virtual inheritance
    // we can no longer use:
    //T* converted = static_cast<T*>(this);
    // Therefore I have created new interface to
    // allow weak references to be counted

private:
    std::atomic<uint32_t> m_ref_counter;

    static std::atomic<int64_t> s_active_object_count;
    static std::atomic<int64_t> s_peak_object_count;
    static std::atomic<int64_t> s_total_objects_created;
};

template <typename T>
class IWeakReferenceable {
    // This class provides weak reference counter interface.
    // The only reason it is template is to avoid virtual
    // inheritance. There are many class hierachies, which
    // derive from this interface multiple times.
    // If I would have used virtual inheritance,
    // the method GetWeakReference could not have existed,
    // because it would not be able to verify the casted type
    // using static_cast - static_cast is prohibited on virtual
    // base classes.

    // The new solution creates additional weak_reference counter
    // for each inherited interface. It should be negligible overhead.
    // At the end of the day, all of the reference counters are
    // deactivated in the destructor.

public:
    IWeakReferenceable() noexcept = default;
    IWeakReferenceable(const IWeakReferenceable&) noexcept = default;
    IWeakReferenceable& operator= (const IWeakReferenceable&) noexcept = default;

    IWeakReferenceable(IWeakReferenceable&&) noexcept = default;
    IWeakReferenceable& operator= (IWeakReferenceable&&) noexcept = default;

    ~IWeakReferenceable() {
        if (m_weak_ref) {
            m_weak_ref->Deactivate();
        }
    }

    // GetWeakReference cannot be const, because
    // it creates WeakReferenceCounter on demand.
    // Otherwise WeakReferenceCounter would be allocated
    // in constructor even for objects
    // that do not have any weak references
    WeakReference<T> GetWeakReference() {
        return GetWeakReference<T>();
    }

    template <typename U>
    WeakReference<U> GetWeakReference() {
        if (!m_weak_ref) {
            m_weak_ref = std::make_shared<WeakReferenceCounter>();
        }

        T* converted = static_cast<U*>(this);
        if (converted == nullptr) {
            LOG_ERROR_AND_THROW(InvalidParameterException, "Pointer object was not set");
        }

        return WeakReference<T>(converted, m_weak_ref);
    }

private:
    std::shared_ptr<WeakReferenceCounter> m_weak_ref;
};

template<typename T>
class IUnknownDeleter {
public:
    static_assert(std::is_base_of<IUnknown, T>::value,
        "Only types derived from IUnknown are supported");

    void operator()(T* ptr) const noexcept {
        ptr->Release();
    }
};

template <typename T>
inline bool operator==(const WeakReference<T>& left, const WeakReference<T>& right) {
    return left.Identity(right);
}

template <typename T>
inline bool operator!=(const WeakReference<T>& left, const WeakReference<T>& right) {
    return !left.Identity(right);
}

} // vanillapdf

namespace std {

template <typename T>
struct hash<vanillapdf::WeakReference<T>> {
    size_t operator()(vanillapdf::WeakReference<T> value) const {
        return value.Hash();
    }
};

} // std

#endif /* _UNKNOWN_INTERFACE_H */
