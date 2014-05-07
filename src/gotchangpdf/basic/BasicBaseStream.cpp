#include "BasicBaseStream.h"

namespace gotchangpdf
{
	namespace basic
	{
		BaseStream::~BaseStream() {}

		std::shared_ptr<BaseStream::BasicSettings> BaseStream::BasicSettingsGet(void) const
		{
			if (0 == _setting_stack.size())
				_setting_stack.push_back(std::shared_ptr<BaseStream::BasicSettings>(new BasicSettings()));

			return _setting_stack.back();
		}

		void BaseStream::BasicSettingsPush(void)
		{
			_setting_stack.push_back(std::shared_ptr<BaseStream::BasicSettings>(new BasicSettings()));
		}

		std::shared_ptr<BaseStream::BasicSettings> BaseStream::BasicSettingsPop(void)
		{
			auto result = BasicSettingsGet();
			_setting_stack.pop_back();

			return result;
		}
	}
}
