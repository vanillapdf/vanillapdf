#ifndef _MODIFY_OBSERVER_INTERFACE_H
#define _MODIFY_OBSERVER_INTERFACE_H

#include <vector>

namespace gotchangpdf {

// Note: it's caller responsibility to guarantee,
// that observer lifetime is superior to observable
// Observer needs to make sure to keep observable alive
// while it is subscribed, otherwise undefined behavior may happen
class IModifyObserver {
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
	void Subscribe(IModifyObserver* observer);
	bool Unsubscribe(IModifyObserver* observer);
	virtual void OnChanged();

	bool IsInitialized(void) const noexcept { return m_initialized; }
	virtual void SetInitialized(bool initialized = true) noexcept { m_initialized = initialized; }

	virtual ~IModifyObservable() = 0;

protected:
	std::vector<IModifyObserver*> m_observers;
	bool m_initialized = false;
};

} // gotchangpdf

#endif /* _MODIFY_OBSERVER_INTERFACE_H */
