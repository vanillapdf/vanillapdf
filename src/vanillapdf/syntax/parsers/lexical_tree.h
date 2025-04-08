#ifndef _LEXICAL_TREE_H
#define _LEXICAL_TREE_H

#include "syntax/parsers/token.h"

#include <limits>

namespace vanillapdf {
namespace syntax {

class Tree {
public:
    bool PathExists(std::string_view path);
    Token::Type TokenType(std::string_view path);
    void Insert(std::string_view path, Token::Type type);

private:
    struct Node {
        Node() = default;
        explicit Node(unsigned char val) : value(val) {}
        unsigned char value = 0;
        Token::Type type = Token::Type::UNKNOWN;
        std::shared_ptr<Node> childs[std::numeric_limits<decltype(value)>::max() + 1];
    };

    std::shared_ptr<Node> _root = std::make_shared<Node>();
    std::shared_ptr<Node> NodeAtPath(std::string_view path);
};

} // syntax
} // vanillapdf

#endif /* _LEXICAL_TREE_H */
