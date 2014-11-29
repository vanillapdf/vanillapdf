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

			explicit HighLevelObject(Deferred<T> obj) : _obj(obj) {}

			inline Deferred<T> Get(void) const { return _obj; }

			//static ObjectReferenceWrapper<HighLevelObject> Create(ObjectReferenceWrapper<Object> low_level);
			virtual inline Type GetType(void) const = 0;

			inline void SetContainer(DirectObject obj)
			{
				//DirectToBaseVisitor visitor;
				//auto base = obj->apply_visitor(visitor);
				_obj->SetContainer(obj);
			}

			inline DirectObject GetContainer() const
			{
				return _obj->GetContainer();
				/*
				auto container = _obj->GetContainer();
				auto type = container->GetType();
				switch (type)
				{
				case gotchangpdf::Object::Type::ArrayObject:
					return MixedArrayObjectPtr(*container.GetAs<MixedArrayObject>());
				case gotchangpdf::Object::Type::DictionaryObject:
					return DictionaryObjectPtr(*container.GetAs<DictionaryObject>());
				case gotchangpdf::Object::Type::StreamObject:
					return StreamObjectPtr(*container.GetAs<StreamObject>());
				default:
					throw exceptions::Exception("TODO");
				}
				*/
			}

			inline void Release() { boost::sp_adl_block::intrusive_ptr_release(this); }

			virtual ~HighLevelObject() {};

		protected:
			BOOST_STATIC_ASSERT((std::is_base_of<Object, T>::value));

			Deferred<T> _obj;
		};
	}
}

#endif /* _HIGH_LEVEL_OBJECT_H */
