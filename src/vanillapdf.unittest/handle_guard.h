#ifndef _VANILLAPDF_UNITTEST_HANDLE_GUARD_H
#define _VANILLAPDF_UNITTEST_HANDLE_GUARD_H

#include "vanillapdf/c_vanillapdf_api.h"

#include <utility>

/**
 * RAII wrapper for C API handle lifetime management in tests.
 *
 * Automatically calls the release function when the guard goes out of scope.
 * Eliminates manual cleanup blocks and ensures handles are released even when
 * assertions fail mid-test.
 *
 * Usage:
 *   HandleGuard<BufferHandle, Buffer_Release> buffer;
 *   ASSERT_EQ(Buffer_Create(buffer.out()), VANILLAPDF_ERROR_SUCCESS);
 *   // buffer is automatically released at scope exit
 */
template<typename Handle, error_type (CALLING_CONVENTION *ReleaseFn)(Handle*)>
class HandleGuard {
    Handle* handle_ = nullptr;

public:
    HandleGuard() = default;
    explicit HandleGuard(Handle* h) : handle_(h) {}

    ~HandleGuard() {
        reset();
    }

    // Move-only semantics to prevent double-release
    HandleGuard(HandleGuard&& other) noexcept : handle_(other.handle_) {
        other.handle_ = nullptr;
    }

    HandleGuard& operator=(HandleGuard&& other) noexcept {
        if (this != &other) {
            reset();
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    HandleGuard(const HandleGuard&) = delete;
    HandleGuard& operator=(const HandleGuard&) = delete;

    /**
     * Returns a pointer to the internal handle pointer.
     * Used with C API Create/Get functions: Create(guard.out()).
     * A handle already owned by the guard is released first, so a guard can be
     * reused for a second lookup without leaking the first result.
     */
    Handle** out() {
        reset();
        return &handle_;
    }

    /**
     * Returns the raw handle pointer.
     * Used when passing to C API functions that take Handle*.
     */
    Handle* get() const { return handle_; }

    /**
     * Implicit conversion to raw handle pointer.
     * Allows passing directly to C API functions.
     */
    operator Handle*() const { return handle_; }

    /**
     * Explicit bool conversion for null checks.
     */
    explicit operator bool() const { return handle_ != nullptr; }

    /**
     * Releases the owned handle now. The release result is deliberately
     * ignored, matching the cleanup blocks it replaces.
     */
    void reset() {
        if (handle_) {
            ReleaseFn(handle_);
            handle_ = nullptr;
        }
    }
};

#endif /* _VANILLAPDF_UNITTEST_HANDLE_GUARD_H */
