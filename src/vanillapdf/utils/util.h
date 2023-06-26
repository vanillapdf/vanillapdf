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
#define SCOPE_GUARD(deleter_function) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard(deleter_function);
#define SCOPE_GUARD_CAPTURE_REFERENCES(deleter_function_code) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard([&](){ deleter_function_code; });
#define SCOPE_GUARD_CAPTURE_VALUES(deleter_function_code) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard([=](){ deleter_function_code; });

#define ACCESS_LOCK_GUARD(lock_name) \
	lock_name->lock(); \
	SCOPE_GUARD([this]() { lock_name->unlock(); });

} // vanillapdf

#endif /* _UTIL_H */
