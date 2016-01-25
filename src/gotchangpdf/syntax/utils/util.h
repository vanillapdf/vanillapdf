#ifndef _UTIL_H
#define _UTIL_H

#include "exception.h"

#include <climits>
#include <type_traits>

namespace gotchangpdf
{
	template<template<typename...> class X, typename T> struct instantiation_of : public std::false_type {};
	template<template<typename...> class X, typename... Y> struct instantiation_of<X, X<Y...>> : public std::true_type {};

	template <typename ValueType, typename RangeType>
	bool IsInRange(ValueType value) _NOEXCEPT {
		return (value >= std::numeric_limits<RangeType>::min()) &&
			(value <= std::numeric_limits<RangeType>::max());
	}

	template <typename DestType, typename SourceType>
	DestType SafeConvert(SourceType value)
	{
		if (!IsInRange<SourceType, DestType>(value))
			throw syntax::ConversionExceptionFactory<DestType>::Construct(value);

		return static_cast<DestType>(value);
	}

	template <typename BaseT>
	class ConvertUtils
	{
	public:
		template <typename T>
		static T ConvertTo(const BaseT& obj) { return Specializator<T>::ConvertTo(obj); }

		template <typename T>
		static bool IsType(const BaseT& obj) { return Specializator<T>::IsType(obj); }

	private:
		template <typename T>
		class Specializator
		{
		public:
			static T ConvertTo(const BaseT& obj)
			{
				auto converted = dynamic_cast<typename T>(obj);
				if (nullptr == converted)
					throw syntax::ConversionExceptionFactory<T>::Construct(obj);

				return converted;
			}

			static bool IsType(const BaseT& obj)
			{
				auto converted = dynamic_cast<typename T>(obj);
				return (nullptr != converted);
			}
		};

		template <typename T>
		class Specializator<Deferred<T>>
		{
		public:
			static Deferred<T> ConvertTo(const BaseT& obj)
			{
				auto ptr = obj.get();
				auto converted = dynamic_cast<typename T *>(ptr);
				if (nullptr == converted)
					throw syntax::ConversionExceptionFactory<T>::Construct(obj);

				return Deferred<T>(converted);
			}

			static bool IsType(const BaseT& obj)
			{
				auto ptr = obj.get();
				auto converted = dynamic_cast<typename T *>(ptr);
				return (nullptr != converted);
			}
		};
	};

	template <typename FunctionType>
	class ScopeGuard
	{
	public:
		explicit ScopeGuard(FunctionType deleter) : _deleter(deleter) {}
		ScopeGuard(const ScopeGuard& other) = delete;
		ScopeGuard& operator=(const ScopeGuard& other) = delete;
		ScopeGuard(ScopeGuard&& other) : _deleter(std::move(other._deleter)) { other.Release(); }
		ScopeGuard& operator=(ScopeGuard&& other) { _deleter = std::move(other._deleter); other.Release(); return *this; }
		void Release() { _released = true; }
		~ScopeGuard() { if (!_released) _deleter(); }

	private:
		bool _released = false;
		FunctionType _deleter;
	};

	class ScopeGuardFactory
	{
	public:
		template <typename FunctionType>
		static ScopeGuard<FunctionType> CreateGuard(FunctionType&& f) { return ScopeGuard<FunctionType>(std::forward<FunctionType>(f)); }

	private:
		ScopeGuardFactory();
	};

#define TOKENPASTE(x, y, z) x ## y ## z
#define TOKENPASTE2(x, y, z) TOKENPASTE(x, y, z)
#define SCOPE_GUARD(deleter_function) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard(deleter_function);
#define SCOPE_GUARD_CAPTURE_REFERENCES(deleter_function_code) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard([&](){ deleter_function_code; });
#define SCOPE_GUARD_CAPTURE_VALUES(deleter_function_code) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard([=](){ deleter_function_code; });

}

#endif /* _UTIL_H */
