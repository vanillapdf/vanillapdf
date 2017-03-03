#ifndef _RANGED_STREAM_BUFFER_H
#define _RANGED_STREAM_BUFFER_H

#include "utils/constants.h"
#include "utils/util.h"

#include <streambuf>
#include <memory>

namespace gotchangpdf {
namespace syntax {

class RangedStreamBuffer : public std::streambuf {
public:
	explicit RangedStreamBuffer(std::shared_ptr<std::istream> stream, types::stream_offset start, types::stream_size size) {
		m_stream = stream;
		m_start = start;
		m_size = size;
	}

	virtual pos_type seekoff(off_type offset,
		std::ios_base::seekdir dir,
		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) override {
		pos_type result = 0;
		if (dir == std::ios_base::end) {
			return (m_start + m_size - offset);
		}

		if (dir == std::ios_base::cur) {
			auto current_offset = gptr() - eback();
			if (current_offset + offset > m_start + m_size) {
				return traits_type::eof();
			}

			return (current_offset + offset);
		}

		if (dir == std::ios_base::beg) {
			if (offset > m_size) {
				return traits_type::eof();
			}

			return (m_start + offset);
		}

		return traits_type::eof();
	}

	virtual pos_type seekpos(pos_type pos,
		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) override {
		if (pos > m_size) {
			return traits_type::eof();
		}

		pos_type offset = SafeAddition<types::stream_offset, pos_type>(m_start, pos);
		m_stream->seekg(pos, offset);
		return pos;
	}

	virtual int_type underflow() override {
		auto current_offset = gptr() - eback();

		if (current_offset > m_size) {
			return traits_type::eof();
		}

		// buffer not exhausted
		if (gptr() > egptr()) {
			return traits_type::to_int_type(*gptr());
		}
	}

	virtual int_type uflow() override {
		auto result = underflow();

		// If underflow failed return immediately
		if (traits_type::eq_int_type(result, traits_type::eof())) {
			return traits_type::eof();
		}

		// Advance the input pointer
		gbump(1);

		// Return underflow result
		return result;
	}

	virtual std::streamsize showmanyc() override {
		auto current_offset = gptr() - eback();
		return m_size - current_offset;
	}

private:
	std::shared_ptr<std::istream> m_stream;
	types::stream_offset m_start;
	types::stream_size m_size;
};

} // syntax
} // gotchangpdf

#endif /* _RANGED_STREAM_BUFFER_H*/
