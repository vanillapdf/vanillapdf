#ifndef _LEXICAL_BASE_STREAM_H
#define _LEXICAL_BASE_STREAM_H

#include "Lexical/Token.h"

#include <memory>

namespace gotchangpdf
{
	namespace lexical
	{
		class BaseStream
		{
		public:
			virtual lexical::Token ReadToken() = 0;
			virtual lexical::Token PeekToken() = 0;
			virtual Token::Type PeekTokenType();
			virtual lexical::Token ReadTokenWithType(Token::Type type);
			virtual ~BaseStream() = 0;

			struct LexicalSettings
			{
				std::vector<Token::Type> skip;
			};

			std::shared_ptr<LexicalSettings> LexicalSettingsGet(void) const;
			void LexicalSettingsPush(void);
			std::shared_ptr<LexicalSettings> LexicalSettingsPop(void);

		private:
			mutable std::vector<std::shared_ptr<LexicalSettings>> _setting_stack;
		};
	}
}

#endif /* _LEXICAL_BASE_STREAM_H */
