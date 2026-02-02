#ifndef _MODIFY_OBSERVER_INTERFACE_H
#define _MODIFY_OBSERVER_INTERFACE_H

#include "utils/observable_interface.h"
#include "utils/unknown_interface.h"

#include <mutex>

namespace vanillapdf {

class IModifyObserver : public virtual IUnknown, public IWeakReferenceable<IModifyObserver> {
public:
    virtual void ObserveeChanged(const IModifyObservable* observee) = 0;
    virtual ~IModifyObserver() = 0;
};

class IModifyObservable : public IObservable<IModifyObserver> {
public:
    IModifyObservable() = default;
    IModifyObservable(const IModifyObservable& other);
    IModifyObservable& operator=(const IModifyObservable& other);
    IModifyObservable(IModifyObservable&& other) noexcept;
    IModifyObservable& operator=(IModifyObservable&& other) noexcept;

    // Bring base class template overloads into scope
    // (overriding Subscribe/Unsubscribe below would otherwise hide them)
    using IObservable<IModifyObserver>::Subscribe;
    using IObservable<IModifyObserver>::Unsubscribe;

public:
    void Subscribe(const WeakReference<IModifyObserver>& observer) override {
        std::lock_guard<std::recursive_mutex> lock(m_observer_mutex);
        IObservable<IModifyObserver>::Subscribe(observer);
    }

    bool Unsubscribe(const WeakReference<IModifyObserver>& observer) override {
        std::lock_guard<std::recursive_mutex> lock(m_observer_mutex);
        return IObservable<IModifyObserver>::Unsubscribe(observer);
    }

    virtual void OnChanged();

    bool IsInitialized(void) const noexcept {
        return m_initialized;
    }

    virtual void SetInitialized(bool initialized = true) {
        m_initialized = initialized;
    }

    virtual ~IModifyObservable() = 0;

protected:
    bool m_initialized = false;
    mutable std::recursive_mutex m_observer_mutex;

private:
    static bool CheckReferenceActive(const WeakReference<IModifyObserver>& ref);
};

} // vanillapdf

#endif /* _MODIFY_OBSERVER_INTERFACE_H */
