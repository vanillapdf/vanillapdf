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
			if (!m_initialized) {
				return;
			}

			auto size = m_observers.size();
			for (decltype(size) i = 0; i < size; ++i) {
				m_observers[i]->ObserveeChanged(this);
			}
		}

		bool IsInitialized(void) const noexcept { return m_initialized; }
		void SetInitialized(bool initialized = true) noexcept { m_initialized = initialized; }

		virtual ~IModifyObservable() = 0;

	protected:
		std::vector<IModifyObserver*> m_observers;
		bool m_initialized = false;
	};

	inline IModifyObservable::~IModifyObservable() {}
	inline IModifyObserver::~IModifyObserver() {}
}

#endif /* _MODIFY_OBSERVER_INTERFACE_H */
