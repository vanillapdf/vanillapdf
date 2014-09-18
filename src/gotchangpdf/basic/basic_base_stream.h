#ifndef _BASIC_BASE_STREAM_H
#define _BASIC_BASE_STREAM_H

#include "buffer.h"

#include <vector>

namespace gotchangpdf
{
	namespace basic
	{
		class BaseStream
		{
		public:
			virtual Character Peek() = 0;
			virtual Character Get() = 0;
			virtual Character GetHex() = 0;
			virtual void Unget(void) = 0;
			virtual Buffer Readline(void) = 0;

			virtual ~BaseStream() = 0;

		protected:
			struct BasicSettings
			{
				std::vector<Character> skip;
			};

			BasicSettings& BasicSettingsGet(void) const;
			void BasicSettingsPush(void);
			BasicSettings& BasicSettingsPop(void);

		private:
			mutable std::vector<BasicSettings> _setting_stack;
		};
	}
}

#endif /* _BASIC_BASE_STREAM_H */
