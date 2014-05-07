#ifndef _LEXICAL_TREE_H
#define _LEXICAL_TREE_H

#include "i_buffer.h"
#include "Lexical/Token.h"

namespace gotchangpdf
{
	namespace lexical
	{
		class Tree
		{
		public:
			bool PathExists(const IBuffer& path);
			Token::Type TokenType(const IBuffer& path);

			void Insert(const IBuffer& path, Token::Type type);

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

			Node* NodeAtPath(const IBuffer& path);
		};
	}
}

#endif /* _LEXICAL_TREE_H */
