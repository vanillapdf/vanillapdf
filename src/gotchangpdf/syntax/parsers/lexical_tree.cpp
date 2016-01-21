#include "precompiled.h"
#include "lexical_tree.h"

namespace gotchangpdf
{
	namespace syntax
	{
		void Tree::Insert(BufferPtr path, Token::Type type)
		{
			auto cur = _root;
			int len = path->size();
			for (int i = 0; i < len; ++i) {
				if (!cur->childs[path->at(i)]) {
					cur->childs[path->at(i)] = std::make_shared<Node>(path->at(i));
				}

				cur = cur->childs[path->at(i)];
			}

			cur->type = type;
		}

		bool Tree::PathExists(BufferPtr path)
		{
			auto found = NodeAtPath(path);
			if (!found)
				return false;

			return true;
		}

		Token::Type Tree::TokenType(BufferPtr path)
		{
			auto found = NodeAtPath(path);
			if (!found)
				return Token::Type::UNKNOWN;

			return found->type;
		}

		std::shared_ptr<Tree::Node> Tree::NodeAtPath(BufferPtr path)
		{
			auto cur = _root;
			auto len = path->size();
			for (decltype(len) i = 0; i < len; ++i) {
				cur = cur->childs[path->at(i)];
				if (!cur) {
					return std::shared_ptr<Tree::Node>();
				}
			}

			return cur;
		}
	}
}
