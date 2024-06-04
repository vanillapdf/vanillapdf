#include "precompiled.h"

#include "contents/content_stream_objects.h"
#include "contents/content_stream_operators.h"

#include <sstream>

namespace vanillapdf {
namespace contents {

TextObject::TextObject(const contents::BaseOperationCollection& ops)
	: _operations(ops) {
}

std::string TextObject::ToPdf() const {
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

std::string InlineImageObject::ToPdf() const {
	std::stringstream ss;

	BeginInlineImageObjectOperatorPtr bi;
	ss << bi->Value();
	ss << std::endl;

	// Image dictionary
	for (auto item : m_dictionary) {
		ss << item.first->ToPdf();
		ss << " ";
		ss << item.second->ToPdf();
		ss << std::endl;
	}

	BeginInlineImageDataOperatorPtr id;
	ss << id->Value();
	ss << std::endl;
	ss << m_data;
	ss << std::endl;

	EndInlineImageObjectOperatorPtr ei;
	ss << ei->Value();

	return ss.str();
}

} // contents
} // vanillapdf
