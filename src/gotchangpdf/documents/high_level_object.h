#ifndef _HIGH_LEVEL_OBJECT
#define _HIGH_LEVEL_OBJECT

#include "intrusive.h"
#include "object.h"
#include "object_reference_wrapper.h"

#include "boost/static_assert.hpp"
//#include "boost/concept_check.hpp"

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
				SpecializedArrayObject,
				PageObject,
				ResourceDictionary,
				Catalog,
				PageTreeNode,
				PageTree
			};

			explicit HighLevelObject(ObjectReferenceWrapper<T> obj) : _obj(obj) {}

			//static ObjectReferenceWrapper<HighLevelObject> Create(ObjectReferenceWrapper<Object> low_level);
			virtual inline Type GetType(void) const = 0;

			inline void SetContainer(ObjectReferenceWrapper<Object> obj) { _obj->SetContainer(obj); }
			inline ObjectReferenceWrapper<Object> GetContainer() const { return _obj->GetContainer(); }

			inline void Release() { boost::sp_adl_block::intrusive_ptr_release(this); }

			virtual ~HighLevelObject() {};

		protected:
			BOOST_STATIC_ASSERT((std::is_base_of<Object, T>::value));

			ObjectReferenceWrapper<T> _obj;
		};
	}
}

#endif /* _HIGH_LEVEL_OBJECT */
