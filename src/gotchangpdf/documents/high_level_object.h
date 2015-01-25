#ifndef _HIGH_LEVEL_OBJECT_H
#define _HIGH_LEVEL_OBJECT_H

#include "object.h"

namespace gotchangpdf
{
	namespace documents
	{
		template <typename T>
		class HighLevelObject : public IUnknown
		{
		public:
			enum class Type : unsigned char
			{
				Unknown = 0,
				PageObject,
				ResourceDictionary,
				Catalog,
				PageTreeNode,
				PageTree,
				Rectangle
			};

			explicit HighLevelObject(T obj) : _obj(obj) {}

			inline T GetObject(void) const { return _obj; }
			virtual inline Type GetType(void) const = 0;

			virtual ~HighLevelObject() {};

		protected:
			T _obj;
		};
	}
}

#endif /* _HIGH_LEVEL_OBJECT_H */
