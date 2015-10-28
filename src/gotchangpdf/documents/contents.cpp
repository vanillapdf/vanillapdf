#include "precompiled.h"
#include "contents.h"

namespace gotchangpdf
{
	namespace documents
	{
		Contents::Contents(StreamObjectPtr obj)
		{
			ContentStreamPtr stream(obj);
			_contents.push_back(obj);
		}

		Contents::Contents(ArrayObjectPtr<IndirectObjectReferencePtr> obj)
		{
			for (auto& ref : *obj) {
				auto item = ref->GetReferencedObjectAs<StreamObjectPtr>();
				_contents.push_back(item);
			}
		}

		lexical::ContentStreamOperationCollection Contents::Operations(void) const
		{
			lexical::ContentStreamOperationCollection result;

			for (auto item : _contents) {
				auto operations = item->Operations();
				result.insert(result.end(), operations.begin(), operations.end());
			}

			return result;
		}
	}
}

