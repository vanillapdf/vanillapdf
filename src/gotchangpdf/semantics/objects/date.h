#ifndef _DATE_H
#define _DATE_H

#include "semantics_fwd.h"
#include "high_level_object.h"

#include <ctime>

namespace gotchangpdf
{
	namespace semantics
	{
		class Date : public HighLevelObject<syntax::StringObjectPtr>
		{
		public:
			explicit Date(syntax::StringObjectPtr root);
			time_t Value(void);


		private:
			time_t m_time;
		};
	}
}

#endif /* _DATE_H */
