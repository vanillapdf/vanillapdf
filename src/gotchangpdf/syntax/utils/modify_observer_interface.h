#ifndef _MODIFY_OBSERVER_INTERFACE_H
#define _MODIFY_OBSERVER_INTERFACE_H

#include <vector>

namespace gotchangpdf
{
	// Note: it's caller responsibility to guarantee,
	// that observer lifetime is superior to observable
	// Observer needs to make sure to keep observable alive
	// while it is subscribed, otherwise undefined behavior may happen
	class IModifyObserver
	{
	public:
		virtual void ObserveeChanged(IModifyObservable* observee) = 0;
		virtual ~IModifyObserver() = 0;
	};

	class IModifyObservable
	{
	public:
		void Subscribe(IModifyObserver* observer)
		{
			m_observers.push_back(observer);
		}

		void Unsubscribe(IModifyObserver* observer)
		{
			for (auto it = m_observers.begin(); it != m_observers.end(); ++it) {
				if (observer == *it) {
					m_observers.erase(it);
					break;
				}
			}
		}

		virtual void OnChanged()
		{
			if (!m_enabled) {
				return;
			}

			auto size = m_observers.size();
			for (decltype(size) i = 0; i < size; ++i) {
				m_observers[i]->ObserveeChanged(this);
			}
		}

		void EnableNotifications(void) { m_enabled = true; }
		void DisableNotifications(void) { m_enabled = false; }

		virtual ~IModifyObservable() = 0;

	private:
		std::vector<IModifyObserver*> m_observers;
		bool m_enabled = true;
	};

	inline IModifyObservable::~IModifyObservable() {}
	inline IModifyObserver::~IModifyObserver() {}
}

#endif /* _MODIFY_OBSERVER_INTERFACE_H */
