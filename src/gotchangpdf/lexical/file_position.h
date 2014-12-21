#ifndef _FILE_POSITION_H
#define _FILE_POSITION_H

#include "constants.h"

#include <boost/spirit/include/classic_position_iterator.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace classic = boost::spirit::classic;

		struct pdf_position : public classic::file_position {
			types::stream_offset offset;

			pdf_position(std::string const& file_ = std::string(), int line_ = 1, int column_ = 1, types::stream_offset offset_ = 0) :
				classic::file_position(file_, line_, column_), offset(offset_) {}

			bool operator==(const pdf_position& fp) const { return offset == fp.offset && this->file == fp.file; }
		};
	}
}

namespace boost
{
	namespace spirit
	{
		namespace classic
		{
			using namespace gotchangpdf::lexical;

			template<>
			class position_policy<pdf_position> {
			public:
				position_policy()
					: m_CharsPerTab(4)
				{}

				void next_line(pdf_position& pos)
				{
					++pos.line;
					pos.column = 1;
				}

				void set_tab_chars(unsigned int chars)
				{
					m_CharsPerTab = chars;
				}

				void next_char(pdf_position& pos)
				{
					++pos.column;
					++pos.offset;
				}

				void tabulation(pdf_position& pos)
				{
					pos.column += m_CharsPerTab - (pos.column - 1) % m_CharsPerTab;
				}

			private:
				unsigned int m_CharsPerTab;
			};
		}
	}
}

#endif /* _FILE_POSITION_H */
