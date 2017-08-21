#ifndef _TEMPLATE_UTILS_H
#define _TEMPLATE_UTILS_H

#include <type_traits>

namespace gotchangpdf {

template<template<typename...> class X, typename T>
struct instantiation_of : public std::false_type {
};

template<template<typename...> class X, typename... Y>
struct instantiation_of<X, X<Y...>> : public std::true_type {
};

template <typename T, typename = void>
struct is_defined : std::false_type {
};

template <typename T>
struct is_defined<T,
	typename std::enable_if<std::is_object<T>::value &&
	!std::is_pointer<T>::value &&
	(sizeof(T) > 0)>::type>
	: std::true_type {
};

} // gotchangpdf

#endif /* _TEMPLATE_UTILS_H */
