#ifndef _BASIC_BASE_STREAM_H
#define _BASIC_BASE_STREAM_H

#include "BaseObjects/CharacterSet.h"

#include <memory>

namespace Pdf
{
	namespace Streams
	{
		namespace Basic
		{
			class BaseStream
			{
			public:
				virtual Character Peek() = 0;
				virtual Character Get() = 0;
				virtual void Unget(void) = 0;
				virtual std::unique_ptr<CharacterSet> Readline(void) = 0;

				virtual ~BaseStream() = 0;

			protected:
				struct Settings
				{
					std::vector<Character> skip;
				};

				std::shared_ptr<Settings> SettingsGet(void) const;
				void SettingsPush(void);
				std::shared_ptr<Settings> SettingsPop(void);

			private:
				mutable std::vector<std::shared_ptr<Settings>> _setting_stack;
			};
		}
	}
}

#endif /* _BASIC_BASE_STREAM_H */
