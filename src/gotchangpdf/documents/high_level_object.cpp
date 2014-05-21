#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		HighLevelObject::~HighLevelObject() {}

		void HighLevelObject::Release()
		{
			boost::sp_adl_block::intrusive_ptr_release(this);
		}

	}
}

