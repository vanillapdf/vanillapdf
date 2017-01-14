#ifndef _CHARACTER_MAP_DATA_H
#define _CHARACTER_MAP_DATA_H

#include "utils/constants.h"

#include "syntax/objects/objects.h"
#include "syntax/utils/object_utils.h"

#include <vector>

namespace gotchangpdf {
namespace syntax {

class BaseFontRange {
public:
	HexadecimalStringObjectPtr GetRangeLow() const { return m_low; }
	void SetRangeLow(HexadecimalStringObjectPtr low) { m_low = low; }

	HexadecimalStringObjectPtr GetRangeHigh() const { return m_high; }
	void SetRangeHigh(HexadecimalStringObjectPtr high) { m_high = high; }

	ObjectPtr GetDestination() const { return m_dest; }
	void SetDestination(ObjectPtr dest) { m_dest = dest; }

	bool Contains(BufferPtr key) const;
	BufferPtr GetMappedValue(BufferPtr key) const;

private:
	HexadecimalStringObjectPtr m_low;
	HexadecimalStringObjectPtr m_high;
	ObjectPtr m_dest;

	bool ValueEqualLessThan(BufferPtr source, BufferPtr dest) const;
	BufferPtr Increment(BufferPtr data, types::native_uint count) const;
	types::integer Difference(BufferPtr source, BufferPtr dest) const;
	uint8_t Difference(uint8_t source, uint8_t dest, bool& borrow) const;
};

struct CodeSpaceRange {
	HexadecimalStringObjectPtr Begin;
	HexadecimalStringObjectPtr End;
};

struct CIDSystemInfo {
	StringObjectPtr Registry;
	StringObjectPtr Ordering;
	IntegerObjectPtr Supplement;
};

struct CharacterMapData {
	CIDSystemInfo SystemInfo;
	NameObjectPtr CMapName;
	IntegerObjectPtr CMapType;

	std::vector<CodeSpaceRange> CodeSpaceRanges;
	std::vector<BaseFontRange> BaseFontRanges;
};

} // syntax
} // gotchangpdf

#endif /* _CHARACTER_MAP_DATA_H */
