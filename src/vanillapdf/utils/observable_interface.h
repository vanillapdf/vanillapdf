#ifndef _OBSERVABLE_INTERFACE_H
#define _OBSERVABLE_INTERFACE_H

#include "utils/unknown_interface.h"

#include <unordered_set>

namespace vanillapdf {

template <typename T>
class IObservable {
public:
	virtual void Subscribe(const WeakReference<T>& observer) {
		auto found = GetObservers()->find(observer);

		if (found != GetObservers()->end()) {
			// Already subscribed - Problem?
		}

		if (found == GetObservers()->end()) {
			GetObservers()->insert(observer);
		}
	}

	virtual bool Unsubscribe(const WeakReference<T>& observer) {
		if (!HasObservers()) {
			return false;
		}

		auto observers = GetObservers();
		auto found = observers->find(observer);
		if (found == observers->end()) {
			return false;
		}

		observers->erase(found);
		return true;
	}

	template <
		typename = typename std::enable_if<
			!std::is_convertible<T*, WeakReference<T>>::value
		>::type
	>
	void Subscribe(T* observer) {
		auto weak_ref = observer->template GetWeakReference<T>();
		Subscribe(weak_ref);
	}

	template <
		typename = typename std::enable_if<
		!std::is_convertible<T*, WeakReference<T>>::value
		>::type
	>
	bool Unsubscribe(T* observer) {
		auto weak_ref = observer->template GetWeakReference<T>();
		return Unsubscribe(weak_ref);
	}

	virtual ~IObservable() = 0;

protected:
	bool HasObservers() const noexcept {
		return (m_observers != nullptr);
	}

	std::shared_ptr<std::unordered_set<WeakReference<T>>> GetObservers() const {
		if (!m_observers) {
			m_observers = std::make_shared<std::unordered_set<WeakReference<T>>>();
		}

		return m_observers;
	}

private:
	mutable std::shared_ptr<std::unordered_set<WeakReference<T>>> m_observers;
};

template <typename T>
IObservable<T>::~IObservable() {

}

} // vanillapdf

#endif /* _OBSERVABLE_INTERFACE_H */
