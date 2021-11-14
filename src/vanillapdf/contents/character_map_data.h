#ifndef _CHARACTER_MAP_DATA_H
#define _CHARACTER_MAP_DATA_H

#include "utils/constants.h"
#include "contents/content_fwd.h"

#include <vector>

namespace vanillapdf {
namespace contents {

class BaseFontRange : public IUnknown {
public:
	syntax::HexadecimalStringObjectPtr GetRangeLow() const { return m_low; }
	void SetRangeLow(syntax::HexadecimalStringObjectPtr low) { m_low = low; }

	syntax::HexadecimalStringObjectPtr GetRangeHigh() const { return m_high; }
	void SetRangeHigh(syntax::HexadecimalStringObjectPtr high) { m_high = high; }

	syntax::ObjectPtr GetDestination() const { return m_dest; }
	void SetDestination(syntax::ObjectPtr dest) { m_dest = dest; }

	bool Contains(BufferPtr key) const;
	BufferPtr GetMappedValue(BufferPtr key) const;

private:
	syntax::HexadecimalStringObjectPtr m_low;
	syntax::HexadecimalStringObjectPtr m_high;
	syntax::ObjectPtr m_dest;

	bool ValueEqualLessThan(BufferPtr source, BufferPtr dest) const;
	uint32_t Difference(BufferPtr source, BufferPtr dest) const;
	uint8_t Difference(uint8_t source, uint8_t dest, bool& borrow) const;

	BufferPtr Increment(BufferPtr data, uint32_t count) const;
	BufferPtr IncrementInternal(BufferPtr data, uint32_t count) const;

	BufferPtr GetMappedValueInternal(BufferPtr key) const;
};

struct CodeSpaceRange {
	syntax::HexadecimalStringObjectPtr Begin;
	syntax::HexadecimalStringObjectPtr End;
};

struct FontCharMapping {
	syntax::HexadecimalStringObjectPtr Source;
	syntax::HexadecimalStringObjectPtr Destination;
};

struct CIDSystemInfo {
	syntax::StringObjectPtr Registry;
	syntax::StringObjectPtr Ordering;
	syntax::IntegerObjectPtr Supplement;
};

struct CharacterMapData {
	CIDSystemInfo SystemInfo;
	syntax::NameObjectPtr CMapName;
	syntax::IntegerObjectPtr CMapType;

	std::vector<CodeSpaceRange> CodeSpaceRanges;

	std::vector<BaseFontRange> BaseFontRanges;
	std::vector<FontCharMapping> BaseFontCharMapping;

	std::vector<BaseFontRange> NotDefinedRanges;
	std::vector<FontCharMapping> NotDefinedCharMapping;

	std::vector<BaseFontRange> CIDRanges;
	std::vector<FontCharMapping> CIDCharMapping;
};

} // contents
} // vanillapdf

#endif /* _CHARACTER_MAP_DATA_H */
