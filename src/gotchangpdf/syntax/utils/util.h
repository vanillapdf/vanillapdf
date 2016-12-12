#ifndef _UTIL_H
#define _UTIL_H

#include "exception.h"

#include <limits>
#include <type_traits>
#include <string>
#include <sstream>
#include <memory>
#include <type_traits>

namespace gotchangpdf
{
	template<template<typename...> class X, typename T> struct instantiation_of : public std::false_type {};
	template<template<typename...> class X, typename... Y> struct instantiation_of<X, X<Y...>> : public std::true_type {};

	class ValueConvertUtils
	{
	public:
		template <typename RangeType, typename ValueType>
		static bool IsInRange(ValueType value) { return Specializator<RangeType, ValueType>::IsInRange(value); }

		template <typename RangeType, typename SourceType>
		static RangeType SafeConvert(SourceType value)
		{
			if (!IsInRange<RangeType, SourceType>(value))
				throw ConversionExceptionFactory<RangeType>::Construct(value);

			return static_cast<RangeType>(value);
		}

	private:
		template <
			typename RangeType,
			typename ValueType,

			// This is to enable specialization for same and different singedness
			bool = (std::is_signed<ValueType>::value ^ std::is_signed<RangeType>::value),

			// I had to divide logic for different singedness furthermore, because
			// while the compiler sees a body where I do compare signed and unsigned
			// even when the logic does not allow control flow to execute the comparison,
			// the compiler warning was issued
			bool = (std::is_signed<ValueType>::value)
		>
		class Specializator
		{
		public:
			static bool IsInRange(ValueType value)
			{
				return (value >= std::numeric_limits<RangeType>::min()) &&
					(value <= std::numeric_limits<RangeType>::max());
			}
		};

		template <
			typename RangeType,
			typename ValueType
		>
		class Specializator<RangeType, ValueType, true, true>
		{
		public:
			static_assert(std::is_signed<ValueType>::value, "Value type shall be signed");

		public:
			static bool IsInRange(ValueType value)
			{
				if (value < 0) {
					return false;
				}

				using unsigned_value_type = typename std::make_unsigned<ValueType>::type;
				unsigned_value_type unsigned_value = static_cast<unsigned_value_type>(value);
				return Specializator<RangeType, unsigned_value_type>::IsInRange(unsigned_value);
			}
		};

		template <
			typename RangeType,
			typename ValueType
		>
		class Specializator<RangeType, ValueType, true, false>
		{
		public:
			static_assert(std::is_signed<RangeType>::value, "Range type shall be signed");

		public:
			static bool IsInRange(ValueType value)
			{
				using unsigned_range_type = typename std::make_unsigned<RangeType>::type;
				RangeType range_max = std::numeric_limits<RangeType>::max();

				// This operation shall be safe
				// Converting maximum signed value shall be in range of unsigned type of the same size
				unsigned_range_type unsigned_range_max = static_cast<unsigned_range_type>(range_max);
				return unsigned_range_max > value;
			}
		};
	};

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
				auto converted = dynamic_cast<T>(obj);
				if (nullptr == converted)
					throw ConversionExceptionFactory<T>::Construct(obj);

				return converted;
			}

			static bool IsType(const BaseT& obj)
			{
				auto converted = dynamic_cast<T>(obj);
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
				auto converted = dynamic_cast<T*>(ptr);
				if (nullptr == converted)
					throw ConversionExceptionFactory<T>::Construct(obj);

				return Deferred<T>(converted);
			}

			static bool IsType(const BaseT& obj)
			{
				auto ptr = obj.get();
				auto converted = dynamic_cast<T*>(ptr);
				return (nullptr != converted);
			}
		};
	};

	template <typename Result, typename ValueFirst, typename ValueSecond = ValueFirst>
	Result SafeAddition(ValueFirst number, ValueSecond addend)
	{
		Result result = number + addend;

		if (number < 0) {
			if (addend < 0) {
				if (result > number) {
					std::stringstream ss;
					ss << "Could not compute sum of " << number << " and " << addend;
					throw std::out_of_range(ss.str());
				}
			}

			return result;
		}

		if (result < number) {
			std::stringstream ss;
			ss << "Could not compute sum of " << number << " and " << addend;
			throw std::out_of_range(ss.str());
		}

		return result;
	}

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

	inline std::string extract_filename(const std::string& path)
	{
		auto pos = path.rfind('\\');
		if (pos == std::string::npos) {
			pos = path.rfind('/');
			if (pos == std::string::npos) {
				return path;
			}
		}

		return std::string(path.begin() + (pos + 1), path.end());
	}

#if (__cplusplus < 201402L) && !defined(_MSC_VER)
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

#define TOKENPASTE(x, y, z) x ## y ## z
#define TOKENPASTE2(x, y, z) TOKENPASTE(x, y, z)
#define SCOPE_GUARD(deleter_function) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard(deleter_function);
#define SCOPE_GUARD_CAPTURE_REFERENCES(deleter_function_code) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard([&](){ deleter_function_code; });
#define SCOPE_GUARD_CAPTURE_VALUES(deleter_function_code) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = ScopeGuardFactory::CreateGuard([=](){ deleter_function_code; });

}

#endif /* _UTIL_H */
