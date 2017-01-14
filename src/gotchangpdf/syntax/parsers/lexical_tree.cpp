#include "precompiled.h"
#include "syntax/parsers/lexical_tree.h"

namespace gotchangpdf {
namespace syntax {

void Tree::Insert(BufferPtr path, Token::Type type) {
	auto cur = _root;
	int len = path->size();
	for (int i = 0; i < len; ++i) {
		char ch = path->at(i);
		char converted = static_cast<unsigned char>(ch);
		if (!cur->childs[converted]) {
			cur->childs[converted] = std::make_shared<Node>(converted);
		}

		cur = cur->childs[path->at(i)];
	}

	cur->type = type;
}

bool Tree::PathExists(BufferPtr path) {
	auto found = NodeAtPath(path);
	if (!found)
		return false;

	return true;
}

Token::Type Tree::TokenType(BufferPtr path) {
	auto found = NodeAtPath(path);
	if (!found)
		return Token::Type::UNKNOWN;

	return found->type;
}

std::shared_ptr<Tree::Node> Tree::NodeAtPath(BufferPtr path) {
	auto cur = _root;
	auto len = path->size();
	for (decltype(len) i = 0; i < len; ++i) {
		auto ch = path->at(i);
		auto converted = static_cast<unsigned char>(ch);
		cur = cur->childs[converted];
		if (!cur) {
			return std::shared_ptr<Tree::Node>();
		}
	}

	return cur;
}

} // syntax
} // gotchangpdf
