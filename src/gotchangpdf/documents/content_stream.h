#ifndef _CONTENT_STREAM_H
#define _CONTENT_STREAM_H

#include "fwd.h"
#include "high_level_object.h"
#include "stream_object.h"

#include "content_stream_operations.h"

namespace gotchangpdf
{
	namespace documents
	{
		class ContentStream : public HighLevelObject<StreamObjectPtr>
		{
		public:
			explicit ContentStream(StreamObjectPtr obj);
			virtual inline Type GetType() const override { return HighLevelObject::Type::ContentStream; }

			lexical::ContentStreamOperationCollection Operations(void) const;
		};
	}
}

#endif /* _CONTENT_STREAM_H */
