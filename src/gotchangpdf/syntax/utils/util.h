#ifndef _UTIL_H
#define _UTIL_H

#include "exception.h"

#include <climits>

namespace gotchangpdf
{
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
				auto ptr = obj.Content.get();
				auto converted = dynamic_cast<typename T *>(ptr);
				if (nullptr == converted)
					throw syntax::ConversionExceptionFactory<T>::Construct(obj);

				return Deferred<T>(converted);
			}

			static bool IsType(const BaseT& obj)
			{
				auto ptr = obj.Content.get();
				auto converted = dynamic_cast<typename T *>(ptr);
				return (nullptr != converted);
			}
		};
	};
}

#endif /* _UTIL_H */
