#ifndef _CONTENT_STREAM_PARSER_H
#define _CONTENT_STREAM_PARSER_H

#include "syntax/parsers/parser.h"

#include "contents/content_stream_instruction_base.h"

namespace vanillapdf {
namespace contents {

class ContentStreamParser : public syntax::ParserBase {
public:
	ContentStreamParser(WeakReference<syntax::File> file, IInputStreamPtr stream);
	BaseInstructionCollectionPtr ReadInstructions(void);

	syntax::ObjectPtr ReadDirectObject() override;

private:
	InstructionBasePtr ReadInstruction(void);
	OperationBasePtr ReadOperation(void);
	bool IsOperand(syntax::Token::Type type);
	OperationBasePtr ReadOperatorReturnOperation(const std::vector<syntax::ObjectPtr>& operands);
	InlineImageObjectPtr ReadInlineImageObject(void);
};

} // contents
} // vanillapdf

#endif /* _CONTENT_STREAM_PARSER_H */