#ifndef _LEXICAL_TREE_H
#define _LEXICAL_TREE_H

#include "buffer.h"
#include "token.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class Tree
		{
		public:
			bool PathExists(BufferPtr path);
			Token::Type TokenType(BufferPtr path);

			void Insert(BufferPtr path, Token::Type type);

			// TODO later
			//void Delete(const char *path, int len);

		private:
			struct Node
			{
				unsigned char value;
				Token::Type type;
				Node *childs[255];
			};

			Node* _root;

			Node* NodeAtPath(BufferPtr path);
		};
	}
}

#endif /* _LEXICAL_TREE_H */
