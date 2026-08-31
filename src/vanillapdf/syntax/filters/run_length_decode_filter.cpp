#include "precompiled.h"

#include "syntax/filters/run_length_decode_filter.h"
#include "syntax/exceptions/syntax_exceptions.h"

namespace vanillapdf {
namespace syntax {

// PDF 32000-1:2008, 7.4.5 - RunLengthDecode Filter
//
// The encoded data is a sequence of runs, each introduced by a single length byte:
// 0 to 127   - the following length + 1 bytes are copied literally
// 128        - end of data
// 129 to 255 - the following single byte is repeated 257 - length times
const unsigned char RUN_LENGTH_END_OF_DATA = 128;

// Both a literal sequence and a repeated run encode at most 128 source bytes:
// a literal length of 0 to 127 stands for 1 to 128 bytes, and a repeated run
// of 129 to 255 stands for 257 - length, which is 128 down to 2 repetitions
const Buffer::size_type RUN_LENGTH_MAXIMUM_LENGTH = 128;

// The constant the specification subtracts the length byte from
// to arrive at the number of repetitions
const Buffer::size_type RUN_LENGTH_REPEAT_COUNT_BIAS = 257;

// Breaking a literal sequence to emit a run costs the two bytes of the run plus
// one more to reopen the literal afterwards, against the one byte per repetition
// the same bytes cost by staying inside the literal. Four bytes against
// repetitions + 1 turns even at three, so shorter repetitions stay literal.
const Buffer::size_type RUN_LENGTH_MINIMUM_RUN = 3;

// Counts how many times the byte at the given position repeats,
// capped at the longest run a single length byte can express
static Buffer::size_type GetRunLength(const BufferPtr& src, Buffer::size_type index) {
    Buffer::size_type run_length = 1;

    while (index + run_length < src->size()
        && run_length < RUN_LENGTH_MAXIMUM_LENGTH
        && src[index + run_length] == src[index]) {
        run_length += 1;
    }

    return run_length;
}

BufferPtr RunLengthDecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr /* = DictionaryObjectPtr() */, AttributeListPtr /* = AttributeListPtr() */) const {
    BufferPtr result;

    Buffer::size_type index = 0;
    while (index < src->size()) {
        auto run_length = GetRunLength(src, index);

        // Long enough repetition to be worth a run of its own
        if (run_length >= RUN_LENGTH_MINIMUM_RUN) {

            // Inverting the repeat count maps a run of 2 to 128 bytes
            // onto a length byte of 255 down to 129
            auto length_byte = ValueConvertUtils::SafeConvert<unsigned char>(RUN_LENGTH_REPEAT_COUNT_BIAS - run_length);

            result->push_back(static_cast<char>(length_byte));
            result->push_back(src[index]);
            index += run_length;
            continue;
        }

        // Otherwise gather the following bytes into a literal sequence,
        // ending it as soon as a repetition worth encoding separately begins
        Buffer::size_type literal_length = 1;
        while (index + literal_length < src->size()
            && literal_length < RUN_LENGTH_MAXIMUM_LENGTH
            && GetRunLength(src, index + literal_length) < RUN_LENGTH_MINIMUM_RUN) {
            literal_length += 1;
        }

        result->push_back(static_cast<char>(literal_length - 1));
        result->insert(result.end(), src.begin() + index, src.begin() + index + literal_length);
        index += literal_length;
    }

    result->push_back(static_cast<char>(RUN_LENGTH_END_OF_DATA));

    return result;
}

BufferPtr RunLengthDecodeFilter::Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr /* = DictionaryObjectPtr() */, AttributeListPtr /* = AttributeListPtr() */) const {
    BufferPtr result;

    types::stream_size consumed = 0;
    while (consumed < length) {
        auto meta = src->Get();
        if (meta == std::char_traits<char>::eof()) {
            LOG_ERROR_AND_THROW(ParseException, "Unexpected end of file inside stream");
        }

        consumed += 1;

        auto run_length = ValueConvertUtils::SafeConvert<unsigned char>(meta);

        // End of data marker
        if (run_length == RUN_LENGTH_END_OF_DATA) {
            break;
        }

        // Copy the following run_length + 1 bytes literally
        if (run_length < RUN_LENGTH_END_OF_DATA) {
            types::stream_size literal_length = static_cast<types::stream_size>(run_length) + 1;

            auto literal = src->Read(literal_length);
            auto literal_size = ValueConvertUtils::SafeConvert<types::stream_size>(literal->size());
            if (literal_size != literal_length) {
                LOG_ERROR_AND_THROW(ParseException, "Truncated run length literal sequence: read {} of {} bytes", literal_size, literal_length);
            }

            result->insert(result.end(), literal.begin(), literal.end());
            consumed += literal_length;
            continue;
        }

        // Repeat the following single byte 257 - run_length times
        auto repeated_meta = src->Get();
        if (repeated_meta == std::char_traits<char>::eof()) {
            LOG_ERROR_AND_THROW(ParseException, "Unexpected end of file inside stream before the repeated byte");
        }

        consumed += 1;

        auto repeated = ValueConvertUtils::SafeConvert<unsigned char>(repeated_meta);
        auto repeat_count = ValueConvertUtils::SafeConvert<Buffer::size_type>(RUN_LENGTH_REPEAT_COUNT_BIAS - run_length);
        result->insert(result.end(), repeat_count, static_cast<char>(repeated));
    }

    return result;
}

BufferPtr RunLengthDecodeFilter::Encode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {

    // Encoding needs to look ahead for repetitions, so the source is materialized first
    auto data = src->Read(length);

    auto data_size = ValueConvertUtils::SafeConvert<types::stream_size>(data->size());
    if (data_size != length) {
        LOG_ERROR_AND_THROW(ParseException, "Unexpected end of file inside stream: read {} of {} bytes", data_size, length);
    }

    return Encode(data, parameters, object_attributes);
}

BufferPtr RunLengthDecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {
    auto stream = src->ToInputStream();
    return Decode(stream, src->size(), parameters, object_attributes);
}

} // syntax
} // vanillapdf
