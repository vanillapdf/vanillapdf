#ifndef _FILE_WRITER_H
#define _FILE_WRITER_H

#include "syntax_fwd.h"

#include <memory>

namespace gotchangpdf
{
	namespace syntax
	{
		class FileWriter : public IUnknown
		{
		public:
			void Write(const std::shared_ptr<File> source, std::shared_ptr<File> destination);
			void WriteIncremental(const std::shared_ptr<File> source, std::shared_ptr<File> destination);

			bool GetRecalculateOffsetFlag(void) const noexcept { return m_recalculate_offset; }
			void SetRecalculateOffsetFlag(bool flag) noexcept { m_recalculate_offset = flag; }

		private:
			XrefBasePtr WriteXrefObjects(std::shared_ptr<File> destination, XrefBasePtr source);
			XrefBasePtr CreateIncrementalXref(std::shared_ptr<File> source, std::shared_ptr<File> destination);
			void WriteXref(std::iostream& output, XrefBasePtr xref);
			void WriteXrefTable(std::iostream& output, XrefTablePtr xref_table);
			void WriteXrefStream(std::iostream& output, XrefStreamPtr xref_stream);
			void WriteXrefOffset(std::ostream& output, types::stream_offset offset);
			void WriteObject(std::ostream& output, const Object& obj);

			// flags
			bool m_recalculate_offset = true;
		};
	}
}

#endif /* _FILE_WRITER_H */
