#ifndef _MODIFY_OBSERVER_INTERFACE_H
#define _MODIFY_OBSERVER_INTERFACE_H

#include "utils/observable_interface.h"
#include "utils/unknown_interface.h"

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
	IModifyObservable(IModifyObservable&& other) = default;
	IModifyObservable& operator=(IModifyObservable&& other) = default;

public:
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

private:
	static bool CheckReferenceActive(const WeakReference<IModifyObserver>& ref);
};

} // vanillapdf

#endif /* _MODIFY_OBSERVER_INTERFACE_H */
