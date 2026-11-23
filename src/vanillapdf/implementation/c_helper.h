#ifndef _C_HELPER_H
#define _C_HELPER_H

#include "utils/log.h"
#include "utils/errors.h"
#include "utils/unknown_interface.h"

#include "vanillapdf/c_values.h"

#include <type_traits>

// Last error
//
// Every C API function that returns a code other than VANILLAPDF_ERROR_SUCCESS
// stores the same code and a message as the last error of the thread, so that
// Errors_GetLastError and Errors_GetLastErrorMessage describe this failure, and
// not an earlier one. CATCH_VANILLAPDF_EXCEPTIONS does this for exceptions. A
// failure that the function returns directly uses
//   return Errors::SetLastError(VANILLAPDF_ERROR_..., "message");
//
// Trivial functions and the Errors_* functions never change the last error, see
// "Trivial functions" below. They return the code directly and use the
// RETURN_ERROR_PARAM_VALUE_IF_NULL_TRIVIAL null check.
#define RETURN_ERROR_PARAM_VALUE_IF_NULL(var) \
    do { \
        if (var == nullptr) { \
            return vanillapdf::Errors::SetLastError(VANILLAPDF_ERROR_PARAMETER_VALUE, "Parameter \"" #var "\" is null"); \
        } \
    } while(0)

// Trivial and Errors_* functions: keeps the last error
#define RETURN_ERROR_PARAM_VALUE_IF_NULL_TRIVIAL(var) \
    do { \
        if (var == nullptr) { \
            return VANILLAPDF_ERROR_PARAMETER_VALUE; \
        } \
    } while(0)

#define CATCH_VANILLAPDF_EXCEPTIONS \
    catch (vanillapdf::ExceptionBase& e) \
    { \
        spdlog::error("{}", e.what()); \
        error_type code = static_cast<error_type>(e.code()); \
        return vanillapdf::Errors::SetLastError(code, e.what()); \
    } \
    catch (std::exception& e) \
    { \
        spdlog::error("{}", e.what()); \
        return vanillapdf::Errors::SetLastError(VANILLAPDF_ERROR_GENERAL, e.what()); \
    } \
    catch (...) \
    { \
        spdlog::error("Caught unknown exception"); \
        return vanillapdf::Errors::SetLastError(VANILLAPDF_ERROR_GENERAL, "Unknown exception"); \
    }

// Trivial functions
//
// A C API function tagged \trivial in its public header keeps the trivial
// function contract (doc/src/input/trivial_functions.dox): while it runs it does
// not lock, allocate or free, throw, log, call an application callback, do I/O or
// parsing, or set the thread error state, and it does a small, fixed amount of
// work. A failure is reported only through the return code.
//
// Language bindings rely on this. The .NET binding calls these functions with
// [SuppressGCTransition], which keeps the calling thread in managed mode, so the
// garbage collector cannot suspend it until the call returns:
//   - A lock can deadlock: the thread that holds it may already be suspended by
//     the garbage collector, which in turn waits for this thread. An allocation
//     counts as a lock, because the heap allocator takes its own locks.
//   - A log line can call the callback set with Logging_SetCallbackLogger, which
//     is managed code. Calling managed code without the transition is illegal.
//   - An exception would leave the extern "C" function: a trivial function has
//     no try/catch, because CATCH_VANILLAPDF_EXCEPTIONS logs.
//   - Errors::SetLastError breaks the documented promise that the last error of
//     the thread is kept. The error state is also thread_local, and the first
//     access to it on a new thread can allocate. A trivial function therefore
//     uses RETURN_ERROR_PARAM_VALUE_IF_NULL_TRIVIAL and never Errors::SetLastError.
//
// The tag is part of the API contract. Removing it, or adding work that breaks
// the contract, is allowed only in a new major version.

// Every conversion built on this template is a trivial function, see
// "Trivial functions" above. The body is null checks, a pointer dynamic_cast
// (never throws, takes no lock, allocates nothing) and the atomic increment in
// IUnknown::AddRef, and must stay that way.
template <
    typename SourceT,
    typename DestT,
    typename SourceHandleT,
    typename DestHandleT,
    typename = typename std::enable_if<std::is_polymorphic<SourceT>::value>::type,
    typename = typename std::enable_if<
        std::is_convertible<SourceT *, DestT *>::value ||
        std::is_base_of<SourceT, DestT>::value
    >::type
>
error_type SafeObjectConvert(SourceHandleT* from, DestHandleT** result) {
    SourceT* obj = reinterpret_cast<SourceT*>(from);
    RETURN_ERROR_PARAM_VALUE_IF_NULL_TRIVIAL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL_TRIVIAL(result);

    DestT* converted = dynamic_cast<DestT*>(obj);
    if (converted == nullptr) {
        return VANILLAPDF_ERROR_PARAMETER_VALUE;
    }

    // My friend, we need to talk and it won't be easy
    // Back in my days, we didn't have to increment the
    // reference counters when converting from one to another

    // When implementing .NET wrapper with usage of SafeHandle
    // the things get complicated. It would not be pretty
    // to increment the counters here, but not there.

    // Let's be nice and keep the interface consistent
    // AND
    // Increment the reference counter
    converted->AddRef();

    *result = reinterpret_cast<DestHandleT*>(converted);
    return VANILLAPDF_ERROR_SUCCESS;
}

// Not a trivial function, and must never be marked \trivial: when the counter
// reaches zero, Release deletes the object. The destructors free memory, take
// locks (~Document takes the document registry lock) and can log.
template <
    typename T,
    typename HandleT,
    typename = typename std::enable_if<std::is_base_of<vanillapdf::IUnknown, T>::value>::type
>
error_type ObjectRelease(HandleT* handle) {
    T* obj = reinterpret_cast<T*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->Release();
    return VANILLAPDF_ERROR_SUCCESS;
}


#endif /* _C_HELPER_H */
