#ifndef _CONTENT_STREAM_H
#define _CONTENT_STREAM_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "stream_object.h"

#include "content_stream_operations.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class ContentStream : public HighLevelObject<syntax::StreamObjectPtr>
		{
		public:
			explicit ContentStream(syntax::StreamObjectPtr obj);
			virtual inline Type GetType() const override { return HighLevelObject::Type::ContentStream; }

			syntax::contents::OperationCollection Operations(void) const;
		};
	}
}

#endif /* _CONTENT_STREAM_H */
