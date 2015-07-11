#ifndef _NULL_OBJECT_H
#define _NULL_OBJECT_H

#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	class NullObject : public Containable, public Object
	{
	public:
		virtual inline Object::Type GetType(void) const _NOEXCEPT override{ return Object::Type::Null; }
		virtual inline void SetContainer(ContainerPtr obj) override { _container = obj; }
		virtual inline ContainerPtr GetContainer() const override { return _container; }

		static NullObjectPtr GetInstance(void)
		{
			static NullObject instance;
			return instance;
		}

		inline bool Equals(const NullObject&) const { return true; }

		inline bool operator==(const NullObject& other) const { return Equals(other); }
		inline bool operator!=(const NullObject& other) const { return !Equals(other); }
		inline bool operator<(const NullObject&) const { return false; }

	public:
		NullObject() = default;
		NullObject(const NullObject&) = default;
		NullObject& operator=(const NullObject&) = default;

	private:
		ContainerPtr _container;
	};
}

#endif /* _NULL_OBJECT_H */
