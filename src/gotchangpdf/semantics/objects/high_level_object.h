#ifndef _HIGH_LEVEL_OBJECT_H
#define _HIGH_LEVEL_OBJECT_H

#include "semantics_fwd.h"
#include "object.h"

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

			WeakReference<Document> GetDocument() const { return _document; }
			void SetDocument(WeakReference<Document> document) { _document = document; }

		protected:
			T _obj;

			WeakReference<Document> _document;
		};
	}
}

#endif /* _HIGH_LEVEL_OBJECT_H */
