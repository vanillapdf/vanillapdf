#include "precompiled.h"
#include "lexical_tree.h"

namespace gotchangpdf
{
	namespace syntax
	{
		void Tree::Insert(BufferPtr path, Token::Type type)
		{
			if (nullptr == _root)
				_root = new Node();

			Node **cur = &_root;
			int len = path->size();
			for (int i = 0; i < len; ++i)
			{
				cur = &(*cur)->childs[path->at(i)];
				if (nullptr == *cur)
				{
					*cur = new Node();
					(*cur)->value = path->at(i);
				}
			}

			(*cur)->type = type;
		}

		bool Tree::PathExists(BufferPtr path)
		{
			Node *found = NodeAtPath(path);
			if (nullptr == found)
				return false;

			return true;
		}

		Token::Type Tree::TokenType(BufferPtr path)
		{
			Node *found = NodeAtPath(path);
			if (nullptr == found)
				return Token::Type::UNKNOWN;

			return found->type;
		}

		Tree::Node* Tree::NodeAtPath(BufferPtr path)
		{
			if (nullptr == _root)
				return nullptr;

			Node *cur = _root;
			int len = path->size();
			for (int i = 0; i < len; ++i)
			{
				cur = cur->childs[path->at(i)];
				if (nullptr == cur)
					return nullptr;
			}

			return cur;
		}
	}
}
