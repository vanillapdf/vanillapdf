#ifndef _CONTENTS_H
#define _CONTENTS_H

#include "fwd.h"
#include "content_stream.h"
#include "direct_object.h"

#include <vector>

namespace gotchangpdf
{
	namespace documents
	{
		class Contents : public IUnknown
		{
		public:
			Contents() = default;
			explicit Contents(StreamObjectPtr obj);
			explicit Contents(ArrayObjectPtr<IndirectObjectReferencePtr> obj);

			lexical::ContentStreamOperationCollection Operations(void) const;

			inline types::integer GetContentStreamSize(void) const { return _contents.size(); }
			inline ContentStreamPtr GetContentStreamAt(types::uinteger at) const { return _contents.at(at); }

		private:
			std::vector<ContentStreamPtr> _contents;
		};
	}
}

#endif /* _CONTENTS_H */
