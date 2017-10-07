#ifndef _OBSERVABLE_INTERFACE_H
#define _OBSERVABLE_INTERFACE_H

#include "utils/unknown_interface.h"

#include <list>

namespace vanillapdf {

template <typename T>
class IObservable {
public:
	void Subscribe(const WeakReference<T>& observer);
	bool Unsubscribe(const WeakReference<T>& observer);

	void Subscribe(T* observer);
	bool Unsubscribe(T* observer);

	virtual ~IObservable() = 0;

protected:
	std::list<WeakReference<T>> m_observers;
};

template <typename T>
void IObservable<T>::Subscribe(const WeakReference<T>& observer) {
	m_observers.push_back(observer);
}

template <typename T>
bool IObservable<T>::Unsubscribe(const WeakReference<T>& observer) {
	for (auto it = m_observers.begin(); it != m_observers.end(); ++it) {
		if (observer == *it) {
			m_observers.erase(it);
			return true;
		}
	}

	return false;
}

template <typename T>
void IObservable<T>::Subscribe(T* observer) {
	auto weak_ref = observer->template GetWeakReference<T>();
	Subscribe(weak_ref);
}

template <typename T>
bool IObservable<T>::Unsubscribe(T* observer) {
	auto weak_ref = observer->template GetWeakReference<T>();
	return Unsubscribe(weak_ref);
}

template <typename T>
IObservable<T>::~IObservable() {

}

} // vanillapdf

#endif /* _OBSERVABLE_INTERFACE_H */
