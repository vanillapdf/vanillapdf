#ifndef _FILE_WRITER_H
#define _FILE_WRITER_H

#include "syntax/utils/syntax_fwd.h"

#include <memory>

namespace gotchangpdf {
namespace syntax {

class FileWriter : public IUnknown {
public:
	void Write(const FilePtr source, FilePtr destination);
	void WriteIncremental(const FilePtr source, FilePtr destination);

	bool GetRecalculateOffsetFlag(void) const noexcept { return m_recalculate_offset; }
	void SetRecalculateOffsetFlag(bool flag) noexcept { m_recalculate_offset = flag; }

	bool GetRecalculateStreamSizeFlag(void) const noexcept { return m_recalculate_stream_size; }
	void SetRecalculateStreamSizeFlag(bool flag) noexcept { m_recalculate_stream_size = flag; }

	bool GetRecalculateXrefSizeFlag(void) const noexcept { return m_recalculate_xref_size; }
	void SetRecalculateXrefSizeFlag(bool flag) noexcept { m_recalculate_xref_size = flag; }

private:
	void WriteXrefObjects(FilePtr destination, XrefBasePtr source);
	XrefBasePtr CloneXref(FilePtr destination, XrefBasePtr source);
	XrefBasePtr CreateIncrementalXref(FilePtr source, FilePtr destination);
	void WriteXref(std::iostream& output, XrefBasePtr xref);
	void WriteXrefTable(std::iostream& output, XrefTablePtr xref_table);
	void WriteXrefOffset(std::iostream& output, types::stream_offset offset);
	void WriteEntry(std::iostream& output, XrefUsedEntryBasePtr entry);
	void WriteObject(std::iostream& output, ObjectPtr obj);

	void RecalculateStreamLength(ObjectPtr obj);
	void RecalculateStreamsLength(XrefBasePtr source);

	// flags
	bool m_recalculate_offset = true;
	bool m_recalculate_stream_size = true;
	bool m_recalculate_xref_size = true;
};

} // syntax
} // gotchangpdf

#endif /* _FILE_WRITER_H */
