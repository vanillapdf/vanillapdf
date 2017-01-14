#include "precompiled.h"

#include "utils/modify_observer_interface.h"

namespace gotchangpdf {

IModifyObservable::~IModifyObservable() {}
IModifyObserver::~IModifyObserver() {}

IModifyObservable::IModifyObservable(const IModifyObservable&) {
	// intentionally empty
}

IModifyObservable& IModifyObservable::operator=(const IModifyObservable&) {
	// intentionally empty
	return *this;
}

void IModifyObservable::Subscribe(IModifyObserver* observer) {
	m_observers.push_back(observer);
}

bool IModifyObservable::Unsubscribe(IModifyObserver* observer) {
	for (auto it = m_observers.begin(); it != m_observers.end(); ++it) {
		if (observer == *it) {
			m_observers.erase(it);
			return true;
		}
	}

	return false;
}

void IModifyObservable::OnChanged() {
	if (!m_initialized) {
		return;
	}

	auto size = m_observers.size();
	for (decltype(size) i = 0; i < size; ++i) {
		m_observers[i]->ObserveeChanged(this);
	}
}

} // gotchangpdf
