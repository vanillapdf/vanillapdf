#ifndef _LEXICAL_TREE_H
#define _LEXICAL_TREE_H

#include "CharacterSet.h"
#include "Lexical/Token.h"

namespace Pdf
{
	namespace Lexical
	{
		class Tree
		{
		public:
			bool PathExists(const CharacterSet& path);
			Token::Type TokenType(const CharacterSet& path);

			void Insert(const CharacterSet& path, Token::Type type);

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

			Node* NodeAtPath(const CharacterSet& path);
		};
	}
}

#endif /* _LEXICAL_TREE_H */
