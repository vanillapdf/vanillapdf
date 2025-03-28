#include "precompiled.h"

#include "utils/modify_observer_interface.h"

namespace vanillapdf {

IModifyObservable::~IModifyObservable() {

    // Check if all references are active
    // All deactivated references shall remove themselves
    if (!HasObservers()) {
        return;
    }

    std::for_each(GetObservers()->begin(), GetObservers()->end(), CheckReferenceActive);
}

IModifyObserver::~IModifyObserver() {}

IModifyObservable::IModifyObservable(const IModifyObservable&) {
    // intentionally empty
}

IModifyObservable& IModifyObservable::operator=(const IModifyObservable&) {
    // intentionally empty
    return *this;
}

void IModifyObservable::OnChanged() {
    // Skip for uninitialized objects
    if (!m_initialized) {
        return;
    }

    if (!HasObservers()) {
        return;
    }

    // This iteration does not increment iterator in the loop
    for (auto current = GetObservers()->begin(); current != GetObservers()->end();) {

        // Remove deactivated elements
        if (!CheckReferenceActive(*current)) {

            // Erase will return next iterator
            current = GetObservers()->erase(current);
            continue;
        }

        // Get our notification observer
        auto observer = current->GetReference();

        // Send the notification
        observer->ObserveeChanged(this);

        // We increment iterator here, so there is no special handling
        // for erasing the deactivated elements
        current++;
    }
}

bool IModifyObservable::CheckReferenceActive(const WeakReference<IModifyObserver>& ref) {

    // All references shall unsubscribe properly from the notifications
    assert(ref.IsActive() && "Found deactivated reference");
    if (!ref.IsActive()) {
        spdlog::error("Found deactivated reference");
    }

    return ref.IsActive();
}

} // vanillapdf
