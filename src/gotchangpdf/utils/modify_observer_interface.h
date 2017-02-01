#ifndef _MODIFY_OBSERVER_INTERFACE_H
#define _MODIFY_OBSERVER_INTERFACE_H

#include "utils/unknown_interface.h"

#include <vector>

namespace gotchangpdf {

class IModifyObserver : public virtual IUnknown, public IWeakReferenceable<IModifyObserver> {
public:
	virtual void ObserveeChanged(IModifyObservable* observee) = 0;
	virtual ~IModifyObserver() = 0;
};

class IModifyObservable {
public:
	IModifyObservable() = default;
	IModifyObservable(const IModifyObservable& other);
	IModifyObservable& operator=(const IModifyObservable& other);
	IModifyObservable(IModifyObservable&& other) = default;
	IModifyObservable& operator=(IModifyObservable&& other) = default;

public:
	void Subscribe(WeakReference<IModifyObserver> observer);
	bool Unsubscribe(WeakReference<IModifyObserver> observer);

	void Subscribe(IModifyObserver* observer);
	bool Unsubscribe(IModifyObserver* observer);

	virtual void OnChanged();

	bool IsInitialized(void) const noexcept {
		return m_initialized;
	}

	virtual void SetInitialized(bool initialized = true) noexcept {
		m_initialized = initialized;
	}

	virtual ~IModifyObservable() = 0;

protected:
	std::vector<WeakReference<IModifyObserver>> m_observers;
	bool m_initialized = false;

private:
	static bool IsReferenceDeactivated(const WeakReference<IModifyObserver>& ref);
};

} // gotchangpdf

#endif /* _MODIFY_OBSERVER_INTERFACE_H */
