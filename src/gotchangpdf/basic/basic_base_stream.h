#ifndef _BASIC_BASE_STREAM_H
#define _BASIC_BASE_STREAM_H

#include "Buffer.h"

#include <memory>
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
			virtual std::shared_ptr<Buffer> Readline(void) = 0;

			virtual ~BaseStream() = 0;

		protected:
			struct BasicSettings
			{
				std::vector<Character> skip;
			};

			std::shared_ptr<BasicSettings> BasicSettingsGet(void) const;
			void BasicSettingsPush(void);
			std::shared_ptr<BasicSettings> BasicSettingsPop(void);

		private:
			mutable std::vector<std::shared_ptr<BasicSettings>> _setting_stack;
		};
	}
}

#endif /* _BASIC_BASE_STREAM_H */
