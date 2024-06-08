#ifndef _UTIL_H
#define _UTIL_H

#include <memory>

namespace vanillapdf {

template <typename FunctionType>
class ScopeGuard {
public:
	explicit ScopeGuard(FunctionType deleter) : _deleter(deleter) {
	}

	ScopeGuard(const ScopeGuard& other) = delete;
	ScopeGuard& operator=(const ScopeGuard& other) = delete;
	ScopeGuard(ScopeGuard&& other) : _deleter(other._deleter) {
		other.Release();
	}

	ScopeGuard& operator=(ScopeGuard&& other) {
		_deleter = other._deleter;
		other.Release();
		return *this;
	}

	void Release() {
		_released = true;
	}

	~ScopeGuard() {
		if (!_released) {
			_deleter();
		}
	}

private:
	bool _released = false;
	FunctionType _deleter;
};

class ScopeGuardFactory {
public:
	template <typename FunctionType>
	static ScopeGuard<FunctionType> CreateGuard(FunctionType&& f) {
		return ScopeGuard<FunctionType>(std::forward<FunctionType>(f));
	}

private:
	ScopeGuardFactory();
};

// The AutoSubscribe concept is something I was thinking about for a long time.
// Unfortunately I am still not able to create a working POC, however I want to keep the code.
// In case I will be thinking about such functionality in the future, this could be the starting point.

template <
	typename T,
	typename U,
	typename = typename std::enable_if<instantiation_of<Deferred, T>::value ||
	std::is_base_of<IModifyObservable, typename T::deferred_ptr_type>::value>::type,
	typename = typename std::enable_if<std::is_base_of<IModifyObservable, U>::value>::type>
class AutoSubscribe {
public:
	explicit AutoSubscribe(const T& observable, U* observer) : _observable(observable), _observer(observer) {
		_observable->Subscribe(_observer);
	}

	AutoSubscribe(const AutoSubscribe& rhs) = delete;

	AutoSubscribe(AutoSubscribe&& rhs) {

		// Unsubscribe the original objects
		_observable->Unsubscribe(_observer);

		// Move the data from the object that is being moved
		_observable = rhs._observable;
		_observer = rhs._observer;

		// Subscribe to new objects
		_observable->Subscribe(_observer);
	}

	AutoSubscribe& operator=(const AutoSubscribe& rhs) = delete;

	AutoSubscribe& operator=(AutoSubscribe&& rhs) {
		AutoSubscribe(rhs).swap(*this);
		return *this;
	}

	~AutoSubscribe() {
		_observable->Unsubscribe(_observer);
	}

private:
	T _observable;
	U* _observer;
};

#if (__cplusplus < 201402L) && !defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
	// Use custom implementation if not
	// supporting c++14

template<class T> struct _Unique_if {
	typedef std::unique_ptr<T> _Single_object;
};

template<class T> struct _Unique_if<T[]> {
	typedef std::unique_ptr<T[]> _Unknown_bound;
};

template<class T, size_t N> struct _Unique_if<T[N]> {
	typedef void _Known_bound;
};

template<class T, class... Args>
typename _Unique_if<T>::_Single_object
make_unique(Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<class T>
typename _Unique_if<T>::_Unknown_bound
make_unique(size_t n) {
	typedef typename std::remove_extent<T>::type U;
	return std::unique_ptr<T>(new U[n]());
}

template<class T, class... Args>
typename _Unique_if<T>::_Known_bound
make_unique(Args&&...) = delete;

#else
using std::make_unique;
#endif

// Some variables are only used for assertion purposes
// They need to be marked as unused, so they don't produce a compilation warning
#define UNUSED(x) (void)(x)

#define TOKENPASTE(x, y, z) x ## y ## z
#define TOKENPASTE2(x, y, z) TOKENPASTE(x, y, z)
#define SCOPE_GUARD(deleter_function) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard(deleter_function)
#define SCOPE_GUARD_CAPTURE_REFERENCES(deleter_function_code) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard([&](){ deleter_function_code; })
#define SCOPE_GUARD_CAPTURE_VALUES(deleter_function_code) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard([=](){ deleter_function_code; })

#define ACCESS_LOCK_GUARD(lock_name) \
	lock_name->lock(); \
	SCOPE_GUARD([this]() { lock_name->unlock(); });

} // vanillapdf

#endif /* _UTIL_H */
