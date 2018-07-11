#ifndef _LEXICAL_TREE_H
#define _LEXICAL_TREE_H

#include "syntax/parsers/token.h"

#include <limits>

namespace vanillapdf {
namespace syntax {

class Tree {
public:
	bool PathExists(BufferPtr path);
	Token::Type TokenType(BufferPtr path);
	void Insert(BufferPtr path, Token::Type type);

private:
	struct Node {
		Node() = default;
		explicit Node(unsigned char val) : value(val) {}
		unsigned char value = 0;
		Token::Type type = Token::Type::UNKNOWN;
		std::shared_ptr<Node> childs[std::numeric_limits<decltype(value)>::max() + 1];
	};

	std::shared_ptr<Node> _root = std::make_shared<Node>();
	std::shared_ptr<Node> NodeAtPath(BufferPtr path);
};

} // syntax
} // vanillapdf

#endif /* _LEXICAL_TREE_H */
