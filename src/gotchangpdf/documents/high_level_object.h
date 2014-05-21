#ifndef _HIGH_LEVEL_OBJECT
#define _HIGH_LEVEL_OBJECT

#include "intrusive.h"

namespace gotchangpdf
{
	namespace documents
	{
		class HighLevelObject : public boost::intrusive_ref_counter<HighLevelObject>
		{
		public:
			enum class Type : unsigned char
			{
				Unknown = 0,
				SpecializedArrayObject
			};

			//static const char* TypeName(Type type);

			virtual inline Type GetType(void) const = 0;

			void Release();

			virtual ~HighLevelObject() = 0;
		};
	}
}

#endif /* _HIGH_LEVEL_OBJECT */
