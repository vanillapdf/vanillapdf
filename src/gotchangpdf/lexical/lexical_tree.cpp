#include "lexical_tree.h"

namespace gotchangpdf
{
	namespace lexical
	{
		void Tree::Insert(const Buffer& path, Token::Type type)
		{
			if (nullptr == _root)
				_root = new Node();

			Node **cur = &_root;
			int len = path.Size();
			for (int i = 0; i < len; ++i)
			{
				cur = &(*cur)->childs[path[i]];
				if (nullptr == *cur)
				{
					*cur = new Node();
					(*cur)->value = path[i];
				}
			}

			(*cur)->type = type;
		}

		bool Tree::PathExists(const Buffer& path)
		{
			Node *found = NodeAtPath(path);
			if (nullptr == found)
				return false;

			return true;
		}

		Token::Type lexical::Tree::TokenType(const Buffer& path)
		{
			Node *found = NodeAtPath(path);
			if (nullptr == found)
				return Token::Type::UNKNOWN;

			return found->type;
		}

		Tree::Node* lexical::Tree::NodeAtPath(const Buffer& path)
		{
			if (nullptr == _root)
				return nullptr;

			Node *cur = _root;
			int len = path.Size();
			for (int i = 0; i < len; ++i)
			{
				cur = cur->childs[path[i]];
				if (nullptr == cur)
					return nullptr;
			}

			return cur;
		}
	}
}
