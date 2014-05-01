#include "BasicBaseStream.h"

namespace Pdf
{
	namespace Streams
	{
		namespace Basic
		{
			BaseStream::~BaseStream() {}

			std::shared_ptr<BaseStream::Settings> BaseStream::SettingsGet(void) const
			{
				if (0 == _setting_stack.size())
					_setting_stack.push_back(std::shared_ptr<BaseStream::Settings>(new Settings()));

				return _setting_stack.back();
			}

			void BaseStream::SettingsPush(void)
			{
				_setting_stack.push_back(std::shared_ptr<BaseStream::Settings>(new Settings()));
			}

			std::shared_ptr<BaseStream::Settings> BaseStream::SettingsPop(void)
			{
				auto result = SettingsGet();
				_setting_stack.pop_back();

				return result;
			}
		}
	}
}
