#include "basic_base_stream.h"

namespace gotchangpdf
{
	namespace basic
	{
		BaseStream::~BaseStream() {}

		BaseStream::BasicSettings& BaseStream::BasicSettingsGet(void) const
		{
			if (0 == _setting_stack.size())
				_setting_stack.push_back(BasicSettings());

			return _setting_stack.back();
		}

		void BaseStream::BasicSettingsPush(void)
		{
			_setting_stack.push_back(BasicSettings());
		}

		BaseStream::BasicSettings& BaseStream::BasicSettingsPop(void)
		{
			BaseStream::BasicSettings& result = BasicSettingsGet();
			_setting_stack.pop_back();

			return result;
		}
	}
}
