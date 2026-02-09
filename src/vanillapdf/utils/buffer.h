#ifndef _BUFFER_H
#define _BUFFER_H

#include "utils/byte_order.h"
#include "utils/character.h"
#include "utils/conversion_utils.h"
#include "utils/unknown_interface.h"
#include "utils/modify_observer_interface.h"

#include "utils/streams/input_stream_interface.h"

#include <vector>
#include <string>
#include <ostream>
#include <cassert>
#include <type_traits>

namespace vanillapdf {

class Buffer : public IUnknown, public IModifyObservable {
public:
    using storage_type = std::vector<char>;

public:
    using value_type = storage_type::value_type;
    using allocator_type = storage_type::allocator_type;
    using iterator = storage_type::iterator;
    using size_type = storage_type::size_type;
    using const_iterator = storage_type::const_iterator;
    using reference = storage_type::reference;
    using const_reference = storage_type::const_reference;
    using difference_type = storage_type::difference_type;

    static_assert(sizeof(size_t) <= sizeof(storage_type::size_type), "Buffer size type is too small");
    static_assert(std::is_unsigned<storage_type::size_type>::value, "Buffer size type should be unsigned");

public:
    Buffer() = default;

    explicit Buffer(const char* chars) : m_data(chars, chars + std::strlen(chars)) {}
    explicit Buffer(std::string_view data) : m_data(data.begin(), data.end()) {}

    Buffer(const char* begin, const char* end) : m_data(begin, end) {}
    Buffer(const char* begin, size_type len) : m_data(begin, begin + len) {}
    Buffer(const unsigned char* begin, size_type len) : m_data(begin, begin + len) {}

    template <
        typename T,
        typename = typename std::enable_if<std::is_integral<T>::value>::type
    >
    explicit Buffer(T count) {
        auto count_converted = ValueConvertUtils::SafeConvert<uint32_t>(count);
        m_data.resize(count_converted);
    }

    template <typename InputIterator>
    Buffer(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()) : m_data(first, last, alloc) {}

    size_t Hash() const;
    BufferPtr Clone(void) const { return make_deferred_container<Buffer>(begin(), end()); }

    /**
     * @brief Create a null-terminated buffer from a string view
     *
     * Use this when the buffer will be used as a C string (e.g., with printf).
     * The returned buffer includes the null terminator in its size.
     */
    static BufferPtr CreateFromString(std::string_view str) {
        auto buffer = make_deferred_container<Buffer>(str.begin(), str.end());
        buffer->push_back('\0');
        return buffer;
    }

    std::string ToString(void) const { return std::string(begin(), end()); }
    std::string_view ToStringView(void) const { return std::string_view(data(), size()); }
    std::string ToHexString(void) const;
    IInputStreamPtr ToInputStream(void) const;
    bool Equals(const Buffer& other) const;
    bool LessThan(const Buffer& other) const;

    /**
     * @brief Interpret buffer bytes as an integer of type T.
     */
    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    T ToInteger(endian order) const {
        assert(m_data.size() <= sizeof(T));

        T result = 0;
        if (order == endian::big) {
            for (size_type i = 0; i < m_data.size(); ++i) {
                result = static_cast<T>((result << 8) | static_cast<uint8_t>(m_data[i]));
            }
        } else if (order == endian::little) {
            for (size_type i = 0; i < m_data.size(); ++i) {
                result |= static_cast<T>(static_cast<uint8_t>(m_data[i])) << (i * 8);
            }
        } else {
            throw GeneralException("Unsupported byte order");
        }

        return result;
    }

    /**
     * @brief Create a buffer from an integer value with the specified byte width.
     */
    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    static BufferPtr FromInteger(T value, size_t byte_width, endian order) {
        auto buf = make_deferred_container<Buffer>();

        if (order == endian::big) {
            for (int i = static_cast<int>(byte_width) - 1; i >= 0; --i) {
                buf->push_back(static_cast<char>((value >> (i * 8)) & 0xFF));
            }
        } else if (order == endian::little) {
            for (size_t i = 0; i < byte_width; ++i) {
                buf->push_back(static_cast<char>((value >> (i * 8)) & 0xFF));
            }
        } else {
            throw GeneralException("Unsupported byte order");
        }

        return buf;
    }

    // stl compatibility
    bool empty(void) const noexcept { return m_data.empty(); }
    value_type * data(void) noexcept { return m_data.data(); }
    const value_type * data(void) const noexcept { return m_data.data(); }
    reference at(size_type pos) { return m_data.at(pos); }
    const_reference at(size_type pos) const { return m_data.at(pos); }
    size_type size(void) const noexcept { return m_data.size(); }
    iterator begin(void) noexcept { return m_data.begin(); }
    const_iterator begin(void) const noexcept { return m_data.begin(); }
    iterator end(void) noexcept { return m_data.end(); }
    const_iterator end(void) const noexcept { return m_data.end(); }
    reference front(void) { return m_data.front(); }
    const_reference front(void) const { return m_data.front(); }
    reference back(void) { return m_data.back(); }
    const_reference back(void) const { return m_data.back(); }
    reference operator[](size_type pos) { return m_data[pos]; }
    const_reference operator[](size_type pos) const { return m_data[pos]; }

    // size_type might not be of type size_t
    // size_t is often needed from other standard functions
    size_t std_size(void) const { return ValueConvertUtils::SafeConvert<size_t>(m_data.size()); }

    // Modifying operations
    void resize(size_type new_size) { m_data.resize(new_size); OnChanged(); }
    void reserve(size_type count) { m_data.reserve(count); OnChanged(); }
    void push_back(const_reference val) { m_data.push_back(val); OnChanged(); }
    void push_back(value_type&& val) { m_data.push_back(val); OnChanged(); }
    void push_back(WhiteSpace val) { push_back(static_cast<char>(val)); }
    void push_back(Delimiter val) { push_back(static_cast<char>(val)); }

    iterator insert(const_iterator where, const value_type& val) {
        auto result = m_data.insert(where, val);
        OnChanged();
        return result;
    }

    iterator insert(const_iterator where, value_type&& val) {
        auto result = m_data.insert(where, val);
        OnChanged();
        return result;
    }

    iterator insert(iterator pos, size_type count, const_reference val) {
        auto result = m_data.insert(pos, count, val);
        OnChanged();
        return result;
    }

    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        m_data.assign(first, last);
        OnChanged();
    }

    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last) {
        m_data.insert(position, first, last);
        OnChanged();
    }

private:
    storage_type m_data;
};

inline bool operator==(const Buffer& left, const Buffer& right) { return left.Equals(right); }
inline bool operator!=(const Buffer& left, const Buffer& right) { return !left.Equals(right); }
inline bool operator<(const Buffer& left, const Buffer& right) { return left.LessThan(right); }

bool operator==(const char * left, const Buffer& right);
bool operator==(const Buffer& left, const char * right);
bool operator!=(const char * left, const Buffer& right);
bool operator!=(const Buffer& left, const char * right);

std::ostream& operator<<(std::ostream& os, const Buffer& value);

} // vanillapdf

#endif /* _BUFFER_H */
