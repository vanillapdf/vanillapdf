#ifndef _HIGH_LEVEL_OBJECT_H
#define _HIGH_LEVEL_OBJECT_H

#include "object.h"
#include "deferred.h"

#include <boost/static_assert.hpp>

#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace gotchangpdf
{
	namespace documents
	{
		template <typename T>
		class HighLevelObject : public boost::intrusive_ref_counter<HighLevelObject<T>>
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

			inline void AddRef() { boost::sp_adl_block::intrusive_ptr_add_ref(this); }
			inline void Release() { boost::sp_adl_block::intrusive_ptr_release(this); }

			virtual ~HighLevelObject() {};

		protected:
			T _obj;
		};
	}
}

#endif /* _HIGH_LEVEL_OBJECT_H */
