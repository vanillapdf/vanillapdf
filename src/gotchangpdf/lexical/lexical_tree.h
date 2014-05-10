#ifndef _LEXICAL_TREE_H
#define _LEXICAL_TREE_H

#include "buffer.h"
#include "Token.h"

namespace gotchangpdf
{
	namespace lexical
	{
		class Tree
		{
		public:
			bool PathExists(const Buffer& path);
			Token::Type TokenType(const Buffer& path);

			void Insert(const Buffer& path, Token::Type type);

			// TODO later
			//void Delete(const char *path, int len);

		private:
			struct Node
			{
				unsigned char value;
				Token::Type type;
				Node *childs[255];
			};

			Node *_root;

			Node* NodeAtPath(const Buffer& path);
		};
	}
}

#endif /* _LEXICAL_TREE_H */
