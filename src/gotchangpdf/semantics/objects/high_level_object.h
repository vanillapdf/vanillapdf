#ifndef _HIGH_LEVEL_OBJECT_H
#define _HIGH_LEVEL_OBJECT_H

#include "utils/unknown_interface.h"
//#include "syntax/objects/object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		template <typename T>
		class HighLevelObject : public IUnknown
		{
		public:
			explicit HighLevelObject(const T& obj) : _obj(obj) {}
			T GetObject(void) const { return _obj; }

		protected:
			T _obj;
		};
	}
}

#endif /* _HIGH_LEVEL_OBJECT_H */
