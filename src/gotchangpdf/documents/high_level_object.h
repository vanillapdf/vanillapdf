#ifndef _HIGH_LEVEL_OBJECT
#define _HIGH_LEVEL_OBJECT

#include "intrusive.h"
#include "object_reference_wrapper.h"

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

			inline void SetParent(ObjectReferenceWrapper<Object> obj) { _obj->SetParent(obj); }
			inline ObjectReferenceWrapper<Object> GetParent() const { return _obj->GetParent(); }

			void Release() { boost::sp_adl_block::intrusive_ptr_release(this); }

			virtual ~HighLevelObject() {};

		protected:
			ObjectReferenceWrapper<T> _obj;
		};
	}
}

#endif /* _HIGH_LEVEL_OBJECT */
