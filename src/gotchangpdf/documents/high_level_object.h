#ifndef _HIGH_LEVEL_OBJECT_H
#define _HIGH_LEVEL_OBJECT_H

#include "object.h"
#include "smart_ptr.h"

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

			explicit HighLevelObject(SmartPtr<T> obj) : _obj(obj) {}

			inline SmartPtr<T> Get(void) const { return _obj; }

			//static ObjectReferenceWrapper<HighLevelObject> Create(ObjectReferenceWrapper<Object> low_level);
			virtual inline Type GetType(void) const = 0;

			inline void SetContainer(SmartPtr<Object> obj) { _obj->SetContainer(obj); }
			inline SmartPtr<Object> GetContainer() const { return _obj->GetContainer(); }

			inline void Release() { boost::sp_adl_block::intrusive_ptr_release(this); }

			virtual ~HighLevelObject() {};

		protected:
			BOOST_STATIC_ASSERT((std::is_base_of<Object, T>::value));

			SmartPtr<T> _obj;
		};
	}
}

#endif /* _HIGH_LEVEL_OBJECT_H */
