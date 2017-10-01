#ifndef _CONTENT_STREAM_PARSER_H
#define _CONTENT_STREAM_PARSER_H

#include "syntax/parsers/parser.h"

#include "contents/content_stream_instruction_base.h"

namespace gotchangpdf {
namespace contents {

class IContentStreamParser {
public:
	virtual BaseInstructionCollectionPtr ReadContentStreamInstructions(void) = 0;

	virtual ~IContentStreamParser() {}
};

class ContentStreamParser : public syntax::ParserBase, public IContentStreamParser {
public:
	ContentStreamParser(WeakReference<syntax::File> file, IInputStreamPtr stream);
	virtual BaseInstructionCollectionPtr ReadContentStreamInstructions(void) override;

private:
	InstructionBasePtr ReadContentStreamInstruction(void);
	OperationBasePtr ReadContentStreamOperation(void);
	bool IsOperand(syntax::Token::Type type);
	syntax::ObjectPtr ReadOperand();
	OperationBasePtr ReadOperatorReturnOperation(const std::vector<syntax::ObjectPtr>& operands);
	InlineImageObjectPtr ReadInlineImageObject(void);
};

} // contents
} // gotchangpdf

#endif /* _CONTENT_STREAM_PARSER_H */