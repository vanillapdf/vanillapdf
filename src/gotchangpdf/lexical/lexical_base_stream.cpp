#include "lexical_base_stream.h"
#include "Exceptions/Exception.h"

#include <sstream>

namespace gotchangpdf
{
	namespace lexical
	{
		using namespace std;
		using namespace exceptions;

		BaseStream::~BaseStream() {}

		Token BaseStream::ReadTokenWithType(Token::Type type)
		{
			auto token = ReadToken();
			if (token.GetType() != type)
			{
				stringstream buffer;
				buffer << "Token type does not correspond to required type: " << Token::GetTypeValueName(type);
				throw Exception(buffer.str());
			}

			return token;
		}

		Token::Type BaseStream::PeekTokenType()
		{
			// TODO optimize
			auto token = PeekToken();
			return token.GetType();
		}

		std::shared_ptr<BaseStream::LexicalSettings> BaseStream::LexicalSettingsGet(void) const
		{
			if (0 == _setting_stack.size())
				_setting_stack.push_back(std::shared_ptr<BaseStream::LexicalSettings>(new LexicalSettings()));

			return _setting_stack.back();
		}

		void BaseStream::LexicalSettingsPush(void)
		{
			_setting_stack.push_back(std::shared_ptr<BaseStream::LexicalSettings>(new LexicalSettings()));
		}

		std::shared_ptr<BaseStream::LexicalSettings> BaseStream::LexicalSettingsPop(void)
		{
			auto result = LexicalSettingsGet();
			_setting_stack.pop_back();

			return result;
		}
	}
}
