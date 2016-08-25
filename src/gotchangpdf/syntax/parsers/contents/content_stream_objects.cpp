#include "precompiled.h"
#include "content_stream_objects.h"
#include "content_stream_operators.h"

#include <sstream>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			std::string TextObject::ToPdf() const
			{
				std::stringstream ss;

				BeginTextOperatorPtr bt;
				ss << bt->Value();
				ss << std::endl;

				for (auto op : _operations) {
					ss << op->ToPdf() << std::endl;
				}

				EndTextOperatorPtr et;
				ss << et->Value();

				return ss.str();
			}

			std::string InlineImageObject::ToPdf() const
			{
				std::stringstream ss;

				BeginInlineImageObjectOperatorPtr bi;
				ss << bi->Value();
				ss << std::endl;

				// Image dictionary
				ss << m_dictionary->ToPdf();

				BeginInlineImageDataOperatorPtr id;
				ss << id->Value();
				ss << std::endl;
				ss << m_data;

				EndTextOperatorPtr ei;
				ss << ei->Value();

				return ss.str();
			}
		}
	}
}
