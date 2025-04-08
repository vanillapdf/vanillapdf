#ifndef _BUFFER_ARRAY_H
#define _BUFFER_ARRAY_H

#include "utils/buffer.h"
#include "utils/unknown_interface.h"

#include <list>

namespace vanillapdf {

class BufferArray : public IUnknown {
public:
    typedef std::vector<BufferPtr> list_type;
    typedef list_type::value_type value_type;
    typedef list_type::size_type size_type;
    typedef list_type::iterator iterator;
    typedef list_type::const_iterator const_iterator;
    typedef list_type::reference reference;
    typedef list_type::const_reference const_reference;
    typedef list_type::difference_type difference_type;

public:
    void Append(BufferPtr value);
    void Insert(BufferPtr value, size_type at);
    bool Remove(size_type at);

    size_type GetSize(void) const noexcept { return m_data.size(); }
    const BufferPtr& operator[](size_type i) const { return m_data[i]; }
    BufferPtr& operator[](size_type i) { return m_data[i]; }
    const BufferPtr& At(size_type at) const { return m_data.at(at); }
    BufferPtr& At(size_type at) { return m_data.at(at); }

    // stl compatibility
    void push_back(BufferPtr value) { m_data.push_back(value); }

    iterator begin() noexcept { return m_data.begin(); }
    const_iterator begin() const noexcept { return m_data.begin(); }
    iterator end() noexcept { return m_data.end(); }
    const_iterator end() const noexcept { return m_data.end(); }

private:
    list_type m_data;
};

} // vanillapdf

#endif /* _BUFFER_ARRAY_H */
